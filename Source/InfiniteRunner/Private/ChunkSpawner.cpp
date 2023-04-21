#include "ChunkSpawner.h"
#include "MyCharacter.h"
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

	for(int i = 1; i < 4; i++)
	{
		FVector NewLocation = FVector(SpawnLocation.X - 2000.f * i, SpawnLocation.Y, SpawnLocation.Z);
		SpawnedChunks.Add(GetWorld()->SpawnActor<AWorldChunk>(GetRandomWorldChunkClass(), NewLocation, FRotator::ZeroRotator));
	}
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
		float Distance = FVector::Distance(Chunk->GetActorLocation(), PlayerCharacter->GetActorLocation());
		if (FVector::DotProduct(PlayerCharacter->GetActorForwardVector(), Chunk->GetActorLocation() - PlayerCharacter->GetActorLocation()) < 0 && Distance > MaxChunkBehindDistance)
		{
			// If the chunk is too far away, remove it from the game and the array
			Chunk->DestroyObstacles();
			Chunk->Destroy();
			SpawnedChunks.RemoveAt(i);
		}
	}

	// Spawn a new road chunk if necessary, but only after the initial ones are loaded
	if (SpawnedChunks.Num() > 0)
	{
		AWorldChunk* LastChunk = SpawnedChunks.Last();
		if (LastChunk->GetChunkEnd() < PlayerCharacter->GetActorLocation().X + GenerationDistance)
		{
			FVector SpawnLocation = FVector(LastChunk->GetChunkEnd(), 0.f, 0.f);
			AWorldChunk* NewChunk = GetWorld()->SpawnActor<AWorldChunk>(GetRandomWorldChunkClass(), SpawnLocation, FRotator::ZeroRotator);
			SpawnedChunks.Add(NewChunk);
			NewChunk->GenerateObstacles();
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, NewChunk->GetActorLocation().ToString());
		}
	}
}

TSubclassOf<AWorldChunk> AChunkSpawner::GetRandomWorldChunkClass() const
{
	int32 Index = FMath::RandRange(0, WorldChunks.Num() - 1);
	return WorldChunks[Index];
}
