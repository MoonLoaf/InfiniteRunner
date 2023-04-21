#include "ObstacleBase.h"

AObstacleBase::AObstacleBase()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = BaseMesh;
}
