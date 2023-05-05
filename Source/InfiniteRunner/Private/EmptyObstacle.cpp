#include "EmptyObstacle.h"

#include "DamageObstacle.h"
#include "ObstacleBase.h"
#include "Components/BoxComponent.h"

AEmptyObstacle::AEmptyObstacle()
{
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	TriggerBox->SetBoxExtent(FVector(20.f, 175.f, 150.f));
	TriggerBox->SetGenerateOverlapEvents(true);
	TriggerBox->SetVisibility(true);
	SetRootComponent(TriggerBox);
}

bool AEmptyObstacle::CheckActorAdjacency()
{
	TArray<AActor*> OverlappingActors;
	TriggerBox->GetOverlappingActors(OverlappingActors);

	for (AActor* OverlappingActor : OverlappingActors)
	{
		if (OverlappingActor->IsA<AObstacleBase>() || OverlappingActor->IsA<ADamageObstacle>())
		{
			return true;
		}
	}
	
	return false;
}

