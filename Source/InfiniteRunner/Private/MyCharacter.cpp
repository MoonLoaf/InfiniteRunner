#include "InfiniteRunner/Public/MyCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//SpringArm Setup
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 500.f;
	SpringArm->TargetOffset.Z = 100.f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 5.f;

	//Camera Setup
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->FieldOfView = 90.f;
	//Camera->SetRelativeLocation(FVector(-1100.f, 0.f, 350.f));
	//Camera->SetRelativeRotation(FQuat(0.f, -20.f, 0.f, 0.f));

	//Lane Array
	LanePositions.Add(0.f);
	LanePositions.Add(0.f);
	LanePositions.Add(0.f);
}


void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Setup PlayerController, Subsytem and input mapping
	if(const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

		if(Subsystem)
		{
			Subsystem->AddMappingContext(InputMapping, 0);
			UE_LOG(LogTemp,Warning,TEXT("Setup Map Succesfully"))
		}
		else{UE_LOG(LogTemp,Warning,TEXT("No Subsystem"))}
	}
	else{UE_LOG(LogTemp,Warning,TEXT("No Valid Player Controller"))}
	
	MyInputComponent = CreatePlayerInputComponent();
	if(MyInputComponent)
	{
		SetupPlayerInputComponent(MyInputComponent);
	}
	else{UE_LOG(LogTemp,Warning,TEXT("No Input Component"))}

	SetupLanes();
	InitialLocation = GetActorLocation();
	bIsJumping = false;
}
	
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsJumping)
	{
		FVector CurrentLocation = GetActorLocation();
		FVector NewLocation = CurrentLocation + FVector(0.f, 0.f, JumpVelocity * DeltaTime);
		SetActorLocation(NewLocation);
    
		JumpVelocity -= 2000.f * DeltaTime;
    
		if (JumpVelocity < 0.f && FMath::Abs(GetActorLocation().Z - InitialLocation.Z) <= 1.f)
		{
			bIsJumping = false;
			JumpVelocity = 0.f;
			SetActorLocation(InitialLocation);
      
			// // Play the landing animation
			// if (LandingAnimation != nullptr)
			// {
			// 	PlayAnimMontage(LandingAnimation);
			// }
		}
	}
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	MyEnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if(MyEnhancedInputComponent)
	{
		//Add Bindings here >>
		//Switching lanes
		MyEnhancedInputComponent->BindAction(IA_SwitchLane, ETriggerEvent::Started, this, &AMyCharacter::SwitchLane);
		//Jumping
		MyEnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &AMyCharacter::Jump);
		
		UE_LOG(LogTemp,Warning,TEXT("Setup Bindings Succesfully"));
	}
	else if(!MyEnhancedInputComponent)
	{
		UE_LOG(LogTemp,Warning,TEXT("No valid EnhancedInputComponent"));
	}
	else{UE_LOG(LogTemp,Warning,TEXT("Error binding Input actions"));}
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

    // Calculate the time needed for the character to reach the target location
    float DistanceToTarget = FVector::Dist(CurrentLocation, TargetLocation);
    float MoveTime = DistanceToTarget / LaneSwitchSpeed;

    // Use a timer to smoothly move the character to the target location
    GetWorldTimerManager().SetTimer(MovementTimerHandle, this, &AMyCharacter::OnMovementComplete, MoveTime, false);
    GetWorldTimerManager().SetTimer(MovementUpdateHandle, FTimerDelegate::CreateUObject(this, &AMyCharacter::OnMoveUpdate, TargetLocation), 0.001, true);
}

void AMyCharacter::OnMoveUpdate(FVector TargetLocation)
{
    FVector CurrentLocation = GetActorLocation();
    FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, GetWorld()->DeltaTimeSeconds, LaneSwitchSpeed);
    SetActorLocation(NewLocation);
	
    if (FVector::DistSquared(CurrentLocation, TargetLocation) < 0.01f)
    {
        GetWorldTimerManager().ClearTimer(MovementUpdateHandle);
        OnMovementComplete(); 
    }
}

void AMyCharacter::OnMovementComplete()
{
    bIsMoving = false;
}


void AMyCharacter::Jump()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Magenta, "Space pressed");
	
	if(bIsJumping||bIsMoving){return;}
	
	//Super::Jump();
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Magenta, "Jump successfully called");
	InitialLocation = GetActorLocation();
	JumpVelocity = 800.f;
	bIsJumping = true;
}

void AMyCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Magenta, "Landed");
  
	bIsJumping = false;
	JumpVelocity = 0.f;
  
	//TODO Play the landing animation
	// if (LandingAnimation != nullptr)
	// {
	// 	PlayAnimMontage(LandingAnimation);
	// }
}
