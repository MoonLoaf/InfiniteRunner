// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldChunk.h"

// Sets default values
AWorldChunk::AWorldChunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));

	

}

// Called when the game starts or when spawned
void AWorldChunk::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWorldChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

