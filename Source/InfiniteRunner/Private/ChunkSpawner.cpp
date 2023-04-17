#include "ChunkSpawner.h"
#include "MyCharacter.h"
#include "WorldChunk.h"
#include "Kismet/GameplayStatics.h"

AChunkSpawner::AChunkSpawner()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AChunkSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	FVector SpawnLocation = GetActorLocation();
	AWorldChunk* FirstChunk = GetWorld()->SpawnActor<AWorldChunk>(WorldChunk, SpawnLocation, FRotator::ZeroRotator);
	SpawnedChunks.Add(FirstChunk);

	for(int i = 1; i < 4; i++)
	{
		FVector NewLocation = FVector(SpawnLocation.X - 1750.f * i, SpawnLocation.Y, SpawnLocation.Z);
		SpawnedChunks.Add(GetWorld()->SpawnActor<AWorldChunk>(WorldChunk, NewLocation, FRotator::ZeroRotator));
	}
}

void AChunkSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Iterate through the spawned road chunks
	for (int32 i = SpawnedChunks.Num() - 1; i >= 0; i--)
	{
		AWorldChunk* Chunk = SpawnedChunks[i];

		// Check the distance between the chunk and the player
		float Distance = FVector::Distance(Chunk->GetActorLocation(), PlayerCharacter->GetActorLocation());
		if (FVector::DotProduct(PlayerCharacter->GetActorForwardVector(), Chunk->GetActorLocation() - PlayerCharacter->GetActorLocation()) < 0 && Distance > MaxChunkBehindDistance)
		{
			// If the chunk is too far away, remove it from the game and the array
			Chunk->Destroy();
			SpawnedChunks.RemoveAt(i);
		}
	}

	// Spawn a new road chunk if necessary
	if (SpawnedChunks.Num() > 0)
	{
		AWorldChunk* LastChunk = SpawnedChunks.Last();
		if (LastChunk->GetChunkEnd() < PlayerCharacter->GetActorLocation().X + GenerationDistance)
		{
			FVector SpawnLocation = FVector(LastChunk->GetChunkEnd(), 0.f, 0.f);
			AWorldChunk* NewChunk = GetWorld()->SpawnActor<AWorldChunk>(WorldChunk, SpawnLocation, FRotator::ZeroRotator);
			SpawnedChunks.Add(NewChunk);
		}
	}
}