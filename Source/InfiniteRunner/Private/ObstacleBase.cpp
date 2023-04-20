#include "ObstacleBase.h"

AObstacleBase::AObstacleBase()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = BaseMesh;
}


void AObstacleBase::BeginPlay()
{
	Super::BeginPlay();
	SetActorRotation(FRotator(0.f, 180.f, 0.f));
	
}
