#include "EmptyObstacle.h"

#include "DamageObstacle.h"
#include "ObstacleBase.h"
#include "Components/BoxComponent.h"

AEmptyObstacle::AEmptyObstacle()
{
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	TriggerBox->SetBoxExtent(FVector(20.f, 175.f, 150.f));
	TriggerBox->SetGenerateOverlapEvents(true);
	SetRootComponent(TriggerBox);
}

bool AEmptyObstacle::CheckActorAdjacency()
{
	bObstacleAdjacent = false;
	
	TArray<AActor*> OverlappedActors;
	
	TriggerBox->GetOverlappingActors(OverlappedActors);

	for (AActor* OverlappingActor : OverlappedActors)
	{
		if (OverlappingActor == this) { continue; }

		if (OverlappingActor->IsA<AObstacleBase>() || OverlappingActor->IsA<ADamageObstacle>())
		{
			bObstacleAdjacent = true;
			return bObstacleAdjacent;
		}
	}
	return bObstacleAdjacent;
}
