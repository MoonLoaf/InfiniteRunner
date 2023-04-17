#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChunkSpawner.generated.h"

UCLASS()
class INFINITERUNNER_API AChunkSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AChunkSpawner();
	
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, Category = "Chunk Generation")
	TSubclassOf<class AWorldChunk> WorldChunk;

	// Maximum distance from the player that a road chunk can be before it is removed
	UPROPERTY(EditAnywhere, Category = "Chunk Generation")
	float GenerationDistance = 6000.f;

	UPROPERTY(EditAnywhere, Category = "Chunk Generation")
	float MaxChunkBehindDistance = 1500.f;

	// Array of spawned road chunks
	TArray<AWorldChunk*> SpawnedChunks;

	class ACharacter* PlayerCharacter;

};
