#include "WorldChunk.h"

#include "MyCharacter.h"
#include "Components/BoxComponent.h"
#include "MyRunnerGameMode.h"


const FVector AWorldChunk::SpawnPoints[] = {
	FVector(170.f, -170.f, 70.f),
	FVector(170.f, 0.f, 70.f),
	FVector(170.f, 170.f, 70.f),
	FVector(300.f, -170.f, 70.f),
	FVector(300.f, 0.f, 70.f),
	FVector(300.f, 170.f, 70.f)
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

	//ObstacleSpawnPoints

	ObstacleTransformParent = CreateDefaultSubobject<USceneComponent>(TEXT("Obstacle Transform Parent"));
	ObstacleTransformParent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	

	for (int i = 1; i < 7; i++)
	{
		FString SpawnComponentName = FString::Printf(TEXT("ObstacleSpawnPoint_%d"), i);

		USceneComponent* SpawnComponent = CreateDefaultSubobject<USceneComponent>(*SpawnComponentName);
		SpawnComponent->SetRelativeLocation(SpawnPoints[i]);
		SpawnComponent->AttachToComponent(ObstacleTransformParent, FAttachmentTransformRules::KeepRelativeTransform);
	}
	
}

AWorldChunk::~AWorldChunk()
{
	for (AActor* SpawnedObstacle : SpawnedObstacles)
	{
		if (SpawnedObstacle != nullptr)
		{
			SpawnedObstacle->Destroy();
		}
	}
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
	return GetActorLocation().X + 1750.f; //Very hardcoded
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

			AActor* SpawnedObstacle = GetWorld()->SpawnActor<AActor>(ObstacleClasses[ObstacleIndex], ObstacleTransformParent->GetComponentTransform().TransformPosition(SpawnPoints[i]), FRotator::ZeroRotator);
			SpawnedObstacles.Add(SpawnedObstacle);
			
			if (SpawnedObstacle)
			{
				USceneComponent* SpawnComponent = ObstacleTransformParent->GetChildComponent(i);
				if (SpawnComponent)
				{
					SpawnedObstacle->AttachToComponent(SpawnComponent, FAttachmentTransformRules::KeepRelativeTransform);
				}
			}
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


