#include "WorldChunkSpawner.h"

#include "WorldChunk.h"
#include "Components/BoxComponent.h"

// Sets default values
AWorldChunkSpawner::AWorldChunkSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box"));
	RootComponent = SpawnerBox;
	SpawnerBox->SetBoxExtent(FVector(1000.f, 1000.f, 20.f));

	SpawnerBox->SetGenerateOverlapEvents(true);

	SpawnerBox->OnComponentBeginOverlap.AddDynamic(this, &AWorldChunkSpawner::OnOverlapBegin);
	SpawnerBox->OnComponentEndOverlap.AddDynamic(this, &AWorldChunkSpawner::OnOverlapEnd);
}

void AWorldChunkSpawner::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Overlap begin");
}

void AWorldChunkSpawner::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Overlap ended, should spawn chunk");
	
	
	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		World->SpawnActor<AWorldChunk>(WorldChunk, GetActorLocation(), GetActorRotation(), SpawnParams);
	}
}

void AWorldChunkSpawner::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		World->SpawnActor<AWorldChunk>(WorldChunk, GetActorLocation(), GetActorRotation(), SpawnParams);
	}	
}

// Called every frame
void AWorldChunkSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

