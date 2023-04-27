#include "MyRunnerGameMode.h"
#include "MyUIclass.h"
#include "CoreMinimal.h"
#include "MyCharacter.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

AMyRunnerGameMode::AMyRunnerGameMode()
{
	SpawnLocation = FVector(0.f, 0.f, 140.f);
}

void AMyRunnerGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	AMyCharacter* Player1 = GetWorld()->SpawnActor<AMyCharacter>(Player1Class, SpawnLocation, FRotator::ZeroRotator);
	APlayerController* Player1Controller = GetWorld()->GetFirstPlayerController();

	if (Player1 && Player1Controller)
	{
		Player1Controller->Possess(Player1);
	}

	if (bIsMultiplayer)
	{
		APlayerController* Player2Controller = UGameplayStatics::CreatePlayer(GetWorld(), 1, true);
		AMyCharacter* Player2 = GetWorld()->SpawnActor<AMyCharacter>(Player2Class, SpawnLocation, FRotator::ZeroRotator);

		if (Player2 && Player2Controller)
		{
			Player2Controller->Possess(Player2);
		}
	}

}

void AMyRunnerGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
}

