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

	
	//virtual void Tick(float DeltaTime) override;

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category= "Collision")
	UStaticMeshComponent* BaseMesh;

private:
	
};
