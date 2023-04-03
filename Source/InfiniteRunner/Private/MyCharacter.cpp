#include "InfiniteRunner/Public/MyCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//SpringArm Setup
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 200.f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 15.f;

	//Camera Setup
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->FieldOfView = 90.f;

}

// Called when the game starts or when spawned
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
}
	

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
		MyEnhancedInputComponent->BindAction(IA_SwitchLane, ETriggerEvent::Completed, this, &AMyCharacter::SwitchLane);
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
	LanePositions[1] = GetActorLocation().Y;
	LanePositions[0] = LanePositions[1] - 50.f;
	LanePositions[2] = LanePositions[1] + 50.f;

	LaneIndex = 1;
}

void AMyCharacter::SwitchLane(const FInputActionInstance& Instance)
{
	float SwitchValue = Instance.GetValue().Get<float>();
	int NewLaneValue = FMath::RoundToInt(SwitchValue);
	if(SwitchValue < 0.f){NewLaneValue--;}

	LaneIndex = NewLaneValue;
}

void AMyCharacter::Jump()
{
	//Jump logic	
}

