#include "WorldChunk.h"
#include "MyRunnerGameMode.h"

AWorldChunk::AWorldChunk()
{
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	RootComponent = BaseMesh;
	BaseMesh->SetGenerateOverlapEvents(true);
}

void AWorldChunk::BeginPlay()
{
	Super::BeginPlay();
}

void AWorldChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MyGameMode.IsValid())
	{
		const float GameSpeed = MyGameMode->GameSpeed;
		FVector NewPosition = GetActorLocation() + FVector(-GameSpeed * DeltaTime, 0.0f, 0.0f);
		SetActorLocation(NewPosition);
	}
}

void AWorldChunk::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MyGameMode = Cast<AMyRunnerGameMode>(GetWorld()->GetAuthGameMode());
}