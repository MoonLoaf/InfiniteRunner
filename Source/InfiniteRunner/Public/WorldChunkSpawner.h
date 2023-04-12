#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldChunkSpawner.generated.h"

UCLASS()
class INFINITERUNNER_API AWorldChunkSpawner : public AActor
{
	GENERATED_BODY()

	class UBoxComponent* SpawnerBox;
	
public:	
	// Sets default values for this actor's properties
	AWorldChunkSpawner();

	virtual void Tick(float DeltaTime) override;
	
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, Category = "Spawner")
	TSubclassOf<class AWorldChunk> WorldChunk;

};
