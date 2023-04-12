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

	UPROPERTY(EditAnywhere, Category="Game Speed")
	float GameSpeed = 5;

};
