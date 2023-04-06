// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldChunk.generated.h"

UCLASS()
class INFINITERUNNER_API AWorldChunk : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldChunk();

	UPROPERTY(EditAnywhere, Category="Mesh")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditAnywhere, Category="Loading")
	class UBoxComponent* DestructionBox;

	UPROPERTY(EditAnywhere, Category="Obstacles")
	TArray<UStaticMeshComponent*> ObstacleArray;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:	

};
