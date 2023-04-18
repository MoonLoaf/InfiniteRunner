#include "WorldChunk.h"

#include "MyCharacter.h"
#include "Components/BoxComponent.h"
#include "MyRunnerGameMode.h"


const FVector AWorldChunk::SpawnPoints[] = {
	FVector(350.f, -170.f, 60.f),
	FVector(350.f, 0.f, 60.f),
	FVector(350.f, 170.f, 60.f),
	FVector(-350.f, -170.f, 60.f),
	FVector(-350.f, 0.f, 60.f),
	FVector(-350.f, 170.f, 60.f)
};

AWorldChunk::AWorldChunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	RootComponent = BaseMesh;
	BaseMesh->SetGenerateOverlapEvents(true);

	ChunkGenerationBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box"));
	ChunkGenerationBox->SetBoxExtent(FVector(512.f,32.f,256.f));
	ChunkGenerationBox->SetCollisionProfileName("Trigger");
	ChunkGenerationBox->SetBoxExtent(FVector(10.f,520.f,100.f));
	ChunkGenerationBox->SetRelativeLocation(FVector(100.f ,-950.f, 0.f));
	ChunkGenerationBox->SetupAttachment(RootComponent);
	
	ChunkGenerationBox->OnComponentBeginOverlap.AddDynamic(this, &AWorldChunk::OnOverlapBegin);
	ChunkGenerationBox->OnComponentEndOverlap.AddDynamic(this, &AWorldChunk::OnOverlapEnd);
}

void AWorldChunk::BeginPlay()
{
	Super::BeginPlay();

	GenerateObstacles();
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

void AWorldChunk::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Overlap begin, chunk");
}

void AWorldChunk::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	if(OtherActor->IsA(AMyCharacter::StaticClass()))
	{
		IncreaseGameSpeed();
	}
}

float AWorldChunk::GetChunkEnd() const
{
	return GetActorLocation().X + 2000.f; //Very hardcoded, representing chunk size though
}

void AWorldChunk::GenerateObstacles()
{
	//Iterate through all spawnpoints
	for(int i = 0; i < UE_ARRAY_COUNT(SpawnPoints); i++)
	{
		bool ShouldSpawn = FMath::RandBool();
		if (ShouldSpawn)
		{
			int ObstacleIndex = FMath::RandRange(0, ObstacleClasses.Num() - 1);

			AActor* SpawnedObstacle = GetWorld()->SpawnActor<AActor>(ObstacleClasses[ObstacleIndex], SpawnPoints[i], FRotator::ZeroRotator);
			SpawnedObstacles.Add(SpawnedObstacle);
			SpawnedObstacle->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		}
	}
}

void AWorldChunk::IncreaseGameSpeed()
{
	if(MyGameMode.IsValid())
	{
		//TODO Rewrite this
		MyGameMode->GameSpeed = (MyGameMode->GameSpeed + 5.f);
	}	
}

void AWorldChunk::DestroyObstacles()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, "Chunk destroyed" + SpawnedObstacles.Num());
	while (SpawnedObstacles.Num() > 0)
	{
		AActor* SpawnedObstacle = SpawnedObstacles[0];
		if (SpawnedObstacle != nullptr)
		{
			SpawnedObstacle->Destroy();
		}
		SpawnedObstacles.RemoveAt(0);
	}
}


