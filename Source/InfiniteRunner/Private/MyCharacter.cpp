#include "InfiniteRunner/Public/MyCharacter.h"

#include "ChunkSpawner.h"
#include "DamageObstacle.h"
#include "EmptyObstacle.h"
#include "EnhancedInputComponent.h"
#include "MyUIclass.h"
#include "Blueprint/UserWidget.h"
#include "EnhancedInputSubsystems.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"

AMyCharacter::AMyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	//Lane Array
	LanePositions.Add(0.f);
	LanePositions.Add(0.f);
	LanePositions.Add(0.f);

	DodgeDetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Dodge Detection Sphere"));
	DodgeDetectionSphere->SetupAttachment(RootComponent);

	//HUD
	MyHudClass = nullptr;
	MyHud = nullptr;
}


void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AMyCharacter::OnCapsuleHit);
	DodgeDetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AMyCharacter::OnCapsuleOverlap);
	
	SetupLanes();
	GetWorldTimerManager().SetTimer(ScoreModifierTimerHandle, this, &AMyCharacter::IncrementScoreModifier, 5.f, true);
	InitialLocation = GetActorLocation();
	bIsJumping = false;
	bIsMoving = false;
	bCanBeDamaged = true;

}

void AMyCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if(MyHud)
	{
		MyHud->RemoveFromParent();
		MyHud = nullptr;
	}
}

void AMyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ChunkSpawner = nullptr;

	ULevel* MyLevel = GetLevel();
	
	for (AActor* Actor : MyLevel->Actors)
	{
		if (AChunkSpawner* Spawner = Cast<AChunkSpawner>(Actor))
		{
			ChunkSpawner = Spawner;
			break;
		}
	}
}

void AMyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if(MyHudClass)
	{
		APlayerController* MyPlayerController = GetController<APlayerController>();
		MyHud = CreateWidget<UMyUIclass>(MyPlayerController, MyHudClass);
		check(MyHud);
		MyHud->AddToViewport();
	}
	
	if(MyHud)
	{
		MyHud->UpdateScoreText(Score);
		MyHud->UpdateHealthText(HealthAmount);
	}
}

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(MyHud)
	{
		Score += DeltaTime * ScoreModifier;
		MyHud->UpdateScoreText(Score);
	}
}

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	MyEnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	//Setup PlayerController, Subsytem and input mapping
	if(const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

		if(Subsystem)
		{
			Subsystem->AddMappingContext(InputMapping, 0);
		}
	}
	if(MyEnhancedInputComponent)
	{
		//Add Bindings here >>
		//Switching lanes
		MyEnhancedInputComponent->BindAction(IA_SwitchLane, ETriggerEvent::Started, this, &AMyCharacter::SwitchLane);
		//Jumping
		MyEnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &AMyCharacter::Jump);

		UE_LOG(LogTemp,Warning,TEXT("Setup Bindings Succesfully"));
	}
}

void AMyCharacter::SetupLanes()
{
	//Fill array with proper values
	LanePositions[1] = GetActorLocation().Y;
	LanePositions[0] = LanePositions[1] - 175.f;
	LanePositions[2] = LanePositions[1] + 175.f;
	//"Middle position"
	LaneIndex = 1;
}

void AMyCharacter::OnCapsuleHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(bCanBeDamaged && OtherActor->IsA<ADamageObstacle>())
	{
		bCanBeDamaged = false;
		HealthAmount--;

		OtherComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MyHud->UpdateHealthText(HealthAmount);
		
		if(HealthAmount <= 0)
		{
			HealthAmount = 0;
			SetActorHiddenInGame(true);
			SetActorTickEnabled(false);

			DodgeDetectionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		GetWorldTimerManager().SetTimer(IFrameTimerHandle, this, &AMyCharacter::ResetIframe, IFrameTime, false);
	}
}

void AMyCharacter::OnCapsuleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AEmptyObstacle* EmptyObstacle = Cast<AEmptyObstacle>(OtherActor))
	{
		float SpawnProbability = FMath::RandRange(0.f,1.f);
		
		if(SpawnProbability < 0.25f && EmptyObstacle->CheckActorAdjacency())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f,FColor::Red, "ObstacleAdjecent");
			ChunkSpawner->RemoveRandomObstacle(EmptyObstacle);
		}
	}
}

void AMyCharacter::ResetIframe()
{
	bCanBeDamaged = true;
}

void AMyCharacter::IncrementScoreModifier()
{
	ScoreModifier++;
}

void AMyCharacter::SwitchLane(const FInputActionInstance& Instance)
{
    if (bIsMoving || bIsJumping){return;}

    bIsMoving = true;

    float SwitchValue = Instance.GetValue().Get<float>();
    int NewLaneValue = FMath::RoundToInt(SwitchValue);

    LaneIndex += NewLaneValue;
    LaneIndex = FMath::Clamp(LaneIndex, 0, LanePositions.Num() - 1);

    FVector CurrentLocation = GetActorLocation();
    FVector TargetLocation = FVector(CurrentLocation.X, LanePositions[LaneIndex], CurrentLocation.Z);

    GetWorldTimerManager().SetTimer(MovementUpdateHandle, FTimerDelegate::CreateUObject(this, &AMyCharacter::OnMoveUpdate, TargetLocation), 0.001, true);
}

void AMyCharacter::OnMoveUpdate(FVector TargetLocation)
{
    FVector CurrentLocation = GetActorLocation();
    FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, GetWorld()->DeltaTimeSeconds, LaneSwitchSpeed);
    SetActorLocation(NewLocation);
	
    if (FMath::Abs(CurrentLocation.Y - TargetLocation.Y) < 1.f)
    {
        GetWorldTimerManager().ClearTimer(MovementUpdateHandle);
        OnMovementComplete();
    	SetActorLocation(TargetLocation);
    }
}

void AMyCharacter::OnMovementComplete()
{
    bIsMoving = false;
}


void AMyCharacter::Jump()
{
	if(bIsJumping || bIsMoving){return;}
	bIsJumping = true;
	Super::Jump();

	FVector JumpVector = FVector(0.f, 0.f, JumpVelocity);
	LaunchCharacter(JumpVector, false, true);
}

void AMyCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
  
	bIsJumping = false;
}
