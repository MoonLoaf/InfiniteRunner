#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldChunk.generated.h"

class AMyRunnerGameMode;

UCLASS()
class INFINITERUNNER_API AWorldChunk : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(EditAnywhere, Category="Mesh")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditAnywhere, Category = "Obstacles")
	TArray<TSubclassOf<AActor>> ObstacleClasses;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "GameMode")
	TWeakObjectPtr<AMyRunnerGameMode> MyGameMode;
	
	//Functions
	AWorldChunk();

protected:
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
	virtual void PostInitializeComponents() override;

};
