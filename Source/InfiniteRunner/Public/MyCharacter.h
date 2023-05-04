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
	
	UFUNCTION()
	void OnCapsuleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category="Movement")
	float LaneSwitchSpeed = 1.f;

	int HealthAmount = 3;

	float Score = 0.f;

	float ScoreModifier = 10.f;

protected:
	
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//Input Actions
	UPROPERTY(EditAnywhere, Category="Input Actions")
	UInputAction* IA_SwitchLane;

	UPROPERTY(EditAnywhere, Category="Input Actions")
	UInputAction* IA_Jump;
	
private:

	class AChunkSpawner* ChunkSpawner;

	virtual void PostInitializeComponents() override;

	virtual void PossessedBy(AController* NewController) override;

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
