#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EmptyObstacle.generated.h"

UCLASS()
class INFINITERUNNER_API AEmptyObstacle : public AActor
{
	GENERATED_BODY()
	
public:
	
	AEmptyObstacle();

	bool CheckActorAdjacency();

	UPROPERTY(EditAnywhere, Category="Collision")
	class UBoxComponent* TriggerBox;
	
};
