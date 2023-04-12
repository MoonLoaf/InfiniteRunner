#include "WorldChunk.h"

#include "MyCharacter.h"
#include "Components/BoxComponent.h"
#include "MyRunnerGameMode.h"


AWorldChunk::AWorldChunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));

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
}

void AWorldChunk::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	if(OtherActor->IsA(AMyCharacter::StaticClass()))
	{
		IncreaseGameSpeed();
	}
}

void AWorldChunk::MoveChunk()
{
}

void AWorldChunk::GenerateObstacles()
{
}

void AWorldChunk::IncreaseGameSpeed()
{
	if(MyGameMode.IsValid())
	{
		//TODO Rewrite this
		MyGameMode->GameSpeed = (MyGameMode->GameSpeed + 5.f);
	}	
}


