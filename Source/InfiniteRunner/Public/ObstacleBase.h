#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObstacleBase.generated.h"

UCLASS()
class INFINITERUNNER_API AObstacleBase : public AActor
{
	GENERATED_BODY()
	
public:
	
	AObstacleBase();

protected:
	
	UPROPERTY(EditAnywhere, Category= "Collision")
	UStaticMeshComponent* BaseMesh;
};
