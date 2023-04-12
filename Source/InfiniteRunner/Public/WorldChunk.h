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

	UPROPERTY(EditAnywhere, Category="Obstacles")
	TArray<UStaticMeshComponent*> ObstacleArray;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "GameMode")
	TWeakObjectPtr<AMyRunnerGameMode> MyGameMode;
	
	//Functions
	AWorldChunk();

	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

	void MoveChunk();

	void GenerateObstacles();

	void IncreaseGameSpeed();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;
private:	

};
