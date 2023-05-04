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

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere, Category="Player 2")
	bool bIsMultiplayer;

	UPROPERTY(EditAnywhere, Category="Game Speed")
	float GameSpeed = 400.f;

	UPROPERTY(EditAnywhere, Category="Game Speed")
	float GameSpeedUpdateInterval = 3.f;
	
	UPROPERTY(EditAnywhere, Category = "Players")
	TSubclassOf<class AMyCharacter> Player1Class;
	
	UPROPERTY(EditAnywhere, Category = "Players")
	TSubclassOf<class AMyCharacter> Player2Class;

	UPROPERTY(EditAnywhere, Category = "High Score")
	TSubclassOf<class UHighScoreWidget> HighScoreWidgetClass;

	UPROPERTY(EditAnywhere, Category="Game Over")
	TSubclassOf<class UGameOverWidget> GameOverWidgetClass;

private:

	//Character Instances
	AMyCharacter* Player1;
	AMyCharacter* Player2;

	FTimerHandle GameSpeedUpdateHandle;

	void UpdateGameSpeed();
	
	FVector SpawnLocation;

	UHighScoreWidget* HighScoreWidgetInstance;

	int CheckPlayerLives() const;

	void OnGameOver();

	void CheckNewHighScore();
};
