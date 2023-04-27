#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MyRunnerGameMode.generated.h"

UCLASS()
class INFINITERUNNER_API AMyRunnerGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	
	AMyRunnerGameMode();

	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Player 2")
	bool bIsMultiplayer;

	UPROPERTY(EditAnywhere, Category="Game Speed")
	float GameSpeed = 5;

	UPROPERTY(EditAnywhere, Category = "Players")
	TSubclassOf<class AMyCharacter> Player1Class;
	
	UPROPERTY(EditAnywhere, Category = "Players")
	TSubclassOf<class AMyCharacter> Player2Class;

private:
	
	FVector SpawnLocation;
};
