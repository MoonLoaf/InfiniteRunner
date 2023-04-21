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

	UFUNCTION()
	void OnCapsuleHit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere, Category="Movement")
	float LaneSwitchSpeed = 1.f;

	int HealthAmount = 3;

	float Score = 0.f;

	float ScoreModifier = 10.f;

protected:
	
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

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

	FTimerHandle IFrameTimerHandle;
	
	FTimerHandle ScoreModifierTimerHandle;

	void ResetIframe();
	
	void IncrementScoreModifier();

	void SwitchLane(const FInputActionInstance& Instance);

	virtual void Jump() override;

	virtual void Landed(const FHitResult & Hit) override; 

	UPROPERTY(EditAnywhere, Category="Jumping")
	float JumpVelocity = 800.f;
	
	bool bIsJumping;

	FVector InitialLocation;

	UPROPERTY(EditAnywhere, Category="Jumping")
	float Gravity = -200;
	
	void OnMovementComplete();
	
	void OnMoveUpdate(FVector TargetLocation);
	
	UPROPERTY(EditAnywhere, Category="Lane Positions")
	TArray<float> LanePositions;

	int LaneIndex;

	bool bIsMoving;

	bool bCanBeDamaged;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float IFrameTime = 1.5f;

	

	//HUD
	UPROPERTY(EditAnywhere, Category="HUD")
	TSubclassOf<class UMyUIclass> MyHudClass;

	UPROPERTY()
	class UMyUIclass* MyHud;
};
