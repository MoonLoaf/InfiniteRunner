#include "InfiniteRunner/Public/MyCharacter.h"

#include "DamageObstacle.h"
#include "EnhancedInputComponent.h"
#include "MyUIclass.h"
#include "Blueprint/UserWidget.h"
#include "EnhancedInputSubsystems.h"
#include "Components/CapsuleComponent.h"

AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Lane Array
	LanePositions.Add(0.f);
	LanePositions.Add(0.f);
	LanePositions.Add(0.f);

	//HUD
	MyHudClass = nullptr;
	MyHud = nullptr;
}


void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AMyCharacter::OnCapsuleHit);
	
	SetupLanes();
	GetWorldTimerManager().SetTimer(ScoreModifierTimerHandle, this, &AMyCharacter::IncrementScoreModifier, 5.f, true);
	InitialLocation = GetActorLocation();
	bIsJumping = false;
	bIsMoving = false;
	bCanBeDamaged = true;

	if(MyHud)
	{
		MyHud->UpdateHealthText(HealthAmount);
		MyHud->UpdateScoreText(Score);
	}
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

void AMyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	UE_LOG(LogTemp, Warning, TEXT("Player possessed by %d"), Cast<APlayerController>(NewController)->GetLocalPlayer()->GetLocalPlayerIndex());
	
	MyInputComponent = CreatePlayerInputComponent();
	if(MyInputComponent)
	{
		SetupPlayerInputComponent(MyInputComponent);
	}
	
	if(MyHudClass && bHudEnabled)
	{
		APlayerController* MyPlayerController = GetController<APlayerController>();
		MyHud = CreateWidget<UMyUIclass>(MyPlayerController, MyHudClass);
		check(MyHud);
		MyHud->AddToViewport();
	}
}

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bHudEnabled && MyHud)
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
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, "Collided");
	if(bCanBeDamaged && OtherActor->IsA<ADamageObstacle>())
	{
		bCanBeDamaged = false;
		HealthAmount--;

		OtherComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		if(bHudEnabled)
		{
			MyHud->UpdateHealthText(HealthAmount);
		}
		if(HealthAmount <= 0)
		{
			HealthAmount = 0;
			SetActorHiddenInGame(true);
			SetActorTickEnabled(false);

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

		GetWorldTimerManager().SetTimer(IFrameTimerHandle, this, &AMyCharacter::ResetIframe, IFrameTime, false);
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

    GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, "Current Lane: " + FString::FromInt(LaneIndex));

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
    	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Movement complete");
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
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Magenta, "Landed");
  
	bIsJumping = false;
}
