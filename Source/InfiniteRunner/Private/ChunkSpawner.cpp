#include "ChunkSpawner.h"
#include "EmptyObstacle.h"
#include "WorldChunk.h"

const FVector AChunkSpawner::SpawnPoints[] = {
	FVector(480.f, -170.f, 30.f),
	FVector(480.f, 0.f, 30.f),
	FVector(480.f, 170.f, 30.f),
	FVector(-350.f, -170.f, 30.f),
	FVector(-350.f, 0.f, 30.f),
	FVector(-350.f, 170.f, 30.f)
	};

AChunkSpawner::AChunkSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
	bAllowTickBeforeBeginPlay = false;
}

void AChunkSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	FVector SpawnLocation = GetActorLocation();
	AWorldChunk* FirstChunk = GetWorld()->SpawnActor<AWorldChunk>(GetRandomWorldChunkClass(), SpawnLocation, FRotator::ZeroRotator);
	SpawnedChunks.Add(FirstChunk);
}

void AChunkSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//TODO Only needs to be checked for one chunk at a time, Array.First()?
	// Iterate through the spawned road chunks
	for (int i = SpawnedChunks.Num() - 1; i >= 0; i--)
	{
		AWorldChunk* Chunk = SpawnedChunks[i];

		// Check the distance between the chunk and the player
		float Distance = FVector::Distance(Chunk->GetActorLocation(), GetActorLocation());
		if (FVector::DotProduct(GetActorForwardVector(), Chunk->GetActorLocation() - GetActorLocation()) < 0 && Distance > MaxChunkBehindDistance)
		{
			// If the chunk is too far away, remove it from the game and the array
			DestroyChunkObstacles(Chunk);
			Chunk->Destroy();
			SpawnedChunks.RemoveAt(i);
		}
	}

	// Spawn a new road chunk if necessary
	if (SpawnedChunks.Num() > 0)
	{
		AWorldChunk* LastChunk = SpawnedChunks.Last();
		if (GetChunkEnd(LastChunk) < GetActorLocation().X + GenerationDistance)
		{
			FVector SpawnLocation = FVector(GetChunkEnd(LastChunk), 0.f, 0.f);
			AWorldChunk* NewChunk = GetWorld()->SpawnActor<AWorldChunk>(GetRandomWorldChunkClass(), SpawnLocation, FRotator::ZeroRotator);
			SpawnedChunks.Add(NewChunk);

			if(SpawnedChunks.Num() > 2)
			{
				GenerateObstacles(NewChunk);
			}
		}
	}
}

void AChunkSpawner::GenerateObstacles(AWorldChunk* Chunk)
{
	for(int i = 0; i < UE_ARRAY_COUNT(SpawnPoints); i++)
	{
		bool ShouldSpawn = FMath::RandBool();
		
		if (ShouldSpawn && ObstacleClasses.Num() != 0)
		{
			AActor* SpawnedObstacle = GetWorld()->SpawnActor<AActor>(GetRandomObstacleClass(), SpawnPoints[i], FRotator(0.f, 180.f, 0.f));
			SpawnedObstacles.Add(SpawnedObstacle);
			SpawnedObstacle->AttachToActor(Chunk, FAttachmentTransformRules::KeepRelativeTransform);
		}
		else if(!ShouldSpawn)
		{
			AActor* SpawnedEmptyObstacle = GetWorld()->SpawnActor<AActor>(EmptyObstacle, SpawnPoints[i], FRotator::ZeroRotator);
			EmptyObstacles.Add(SpawnedEmptyObstacle);
			SpawnedEmptyObstacle->AttachToActor(Chunk, FAttachmentTransformRules::KeepRelativeTransform);
		}
	}
}

void AChunkSpawner::RemoveRandomObstacle(AEmptyObstacle* DodgedObstacle)
{
	int Index = SpawnedObstacles.Find(DodgedObstacle) + 4;
	Index = FMath::Clamp(Index, 0, SpawnedObstacles.Num() - 1);

	SpawnedObstacles[Index]->SetActorHiddenInGame(true);
	SpawnedObstacles[Index]->SetActorEnableCollision(false);
}

TSubclassOf<AWorldChunk> AChunkSpawner::GetRandomWorldChunkClass() const
{
	int32 Index = FMath::RandRange(0, WorldChunks.Num() - 1);
	return WorldChunks[Index];
}

TSubclassOf<AActor> AChunkSpawner::GetRandomObstacleClass() const
{
	int32 Index = FMath::RandRange(0, ObstacleClasses.Num() - 1);
	return ObstacleClasses[Index];
}

float AChunkSpawner::GetChunkEnd(AActor* Chunk) const
{
	return Chunk->GetActorLocation().X + 2000.f; //Very hardcoded, representing chunk size though
}

void AChunkSpawner::DestroyChunkObstacles(AActor* Chunk)
{
	TArray<AActor*> ChildActors;
	Chunk->GetAttachedActors(ChildActors);

	while (ChildActors.Num() > 0)
	{
		AActor* SpawnedObstacle = ChildActors[0];
		if (SpawnedObstacle != nullptr)
		{
			if (SpawnedObstacles.Contains(SpawnedObstacle))
			{
				SpawnedObstacles.Remove(SpawnedObstacle);
			}
			else if (EmptyObstacles.Contains(SpawnedObstacle))
			{
				EmptyObstacles.Remove(SpawnedObstacle);
			}
			ChildActors.Remove(SpawnedObstacle);
			SpawnedObstacle->Destroy();
		}
	}
}

