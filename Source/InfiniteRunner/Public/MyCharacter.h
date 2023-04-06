#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "MyCharacter.generated.h"

UCLASS()
class INFINITERUNNER_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();
	
	//Components	
	UPROPERTY(EditAnywhere, Category="Camera")
	class UInputMappingContext* InputMapping;

	UPROPERTY()
	class UEnhancedInputComponent* MyEnhancedInputComponent;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputComponent* MyInputComponent;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void SetupLanes();

	UPROPERTY(EditAnywhere, Category="Movement")
	float LaneSwitchSpeed = 1.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Components
	UPROPERTY(EditAnywhere, Category="Camera")
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category="Camera")
	class USpringArmComponent* SpringArm;

	//Input Actions
	UPROPERTY(EditAnywhere, Category="Input Actions")
	UInputAction* IA_SwitchLane;

	UPROPERTY(EditAnywhere, Category="Input Actions")
	UInputAction* IA_Jump;
	
private:

	FTimerHandle MovementTimerHandle;
	
	FTimerHandle MovementUpdateHandle;

	void SwitchLane(const FInputActionInstance& Instance);

	void Jump();

	void OnMovementComplete();
	
	void OnMoveUpdate(FVector TargetLocation);
	
	UPROPERTY(EditAnywhere, Category="Lane Positions")
	TArray<float> LanePositions;

	int LaneIndex;

	bool bIsMoving;
};
