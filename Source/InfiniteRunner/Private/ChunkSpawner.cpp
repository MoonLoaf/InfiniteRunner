#include "ChunkSpawner.h"
#include "WorldChunk.h"
#include "Kismet/GameplayStatics.h"

AChunkSpawner::AChunkSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
	bAllowTickBeforeBeginPlay = false;
}

void AChunkSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

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
			Chunk->DestroyObstacles();
			Chunk->Destroy();
			SpawnedChunks.RemoveAt(i);
		}
	}

	// Spawn a new road chunk if necessary
	if (SpawnedChunks.Num() > 0)
	{
		AWorldChunk* LastChunk = SpawnedChunks.Last();
		if (LastChunk->GetChunkEnd() < GetActorLocation().X + GenerationDistance)
		{
			FVector SpawnLocation = FVector(LastChunk->GetChunkEnd(), 0.f, 0.f);
			AWorldChunk* NewChunk = GetWorld()->SpawnActor<AWorldChunk>(GetRandomWorldChunkClass(), SpawnLocation, FRotator::ZeroRotator);
			SpawnedChunks.Add(NewChunk);

			if(SpawnedChunks.Num() > 2)
			{
				NewChunk->GenerateObstacles();
			}
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, NewChunk->GetActorLocation().ToString());
		}
	}
}

TSubclassOf<AWorldChunk> AChunkSpawner::GetRandomWorldChunkClass() const
{
	int32 Index = FMath::RandRange(0, WorldChunks.Num() - 1);
	return WorldChunks[Index];
}
