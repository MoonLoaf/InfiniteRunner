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

	UPROPERTY(EditAnywhere, Category="Loading")
	class UBoxComponent* ChunkGenerationBox;

	UPROPERTY(EditAnywhere, Category = "Obstacles")
	TArray<TSubclassOf<AActor>> ObstacleClasses;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "GameMode")
	TWeakObjectPtr<AMyRunnerGameMode> MyGameMode;
	
	//Functions
	AWorldChunk();

	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

	float GetChunkEnd() const;
	
	void IncreaseGameSpeed();

	void DestroyObstacles();

	void GenerateObstacles();
	
protected:
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
	virtual void PostInitializeComponents() override;

	
	
private:	


	static const FVector SpawnPoints[];

	TArray<AActor*> SpawnedObstacles;
};
