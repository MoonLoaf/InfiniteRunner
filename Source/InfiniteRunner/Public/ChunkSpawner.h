#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChunkSpawner.generated.h"

class AObstacleBase;
class AWorldChunk;
class AEmptyObstacle;

UCLASS()
class INFINITERUNNER_API AChunkSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AChunkSpawner();
	
	virtual void Tick(float DeltaTime) override;
	
	void GenerateObstacles(AWorldChunk* Chunk);

	void RemoveRandomObstacle(class AEmptyObstacle* DodgedObstacle);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	
	TSubclassOf<::AWorldChunk> GetRandomWorldChunkClass() const;

	TSubclassOf<::AActor> GetRandomObstacleClass() const;
	
	float GetChunkEnd(AActor* Chunk) const;
	
	void DestroyChunkObstacles(AActor* Chunk);

	UPROPERTY(EditAnywhere, Category = "Chunk Generation")
	TArray<TSubclassOf<AActor>> ObstacleClasses;
	
	UPROPERTY(EditAnywhere, Category = "Chunk Generation")
	TArray<TSubclassOf<AWorldChunk>> WorldChunks;
	
	UPROPERTY(EditAnywhere, Category = "Chunk Generation")
	TSubclassOf<AActor> EmptyObstacle;

	// Maximum distance from the player that a road chunk can be before it is removed
	UPROPERTY(EditAnywhere, Category = "Chunk Generation")
	float GenerationDistance = 6000.f;

	UPROPERTY(EditAnywhere, Category = "Chunk Generation")
	float MaxChunkBehindDistance = 1500.f;

	// Array of spawned road chunks
	TArray<AWorldChunk*> SpawnedChunks;

	static const FVector SpawnPoints[];

	TArray<AActor*> SpawnedObstacles;
	TArray<AActor*> EmptyObstacles;
};
