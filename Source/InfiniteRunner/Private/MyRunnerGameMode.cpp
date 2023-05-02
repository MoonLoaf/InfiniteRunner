#include "MyRunnerGameMode.h"

#include "CoreMinimal.h"
#include "GameOverWidget.h"
#include "HighScoreWidget.h"
#include "MyCharacter.h"
#include "Components/Button.h"
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
	
	Player1 = GetWorld()->SpawnActor<AMyCharacter>(Player1Class, SpawnLocation, FRotator::ZeroRotator);
	ULocalPlayer* LocalPlayer1 = GetGameInstance()->GetLocalPlayerByIndex(0);
	APlayerController* Player1Controller = LocalPlayer1->GetPlayerController(GetWorld());

	if (Player1 && Player1Controller)
	{
		Player1Controller->Possess(Player1);
		UE_LOG(LogTemp, Warning, TEXT("Player 1 possessed by %d"), Player1Controller->GetLocalPlayer()->GetLocalPlayerIndex());
	}

	if (bIsMultiplayer)
	{
		APlayerController* Player2Controller = UGameplayStatics::CreatePlayer(GetWorld(), 1, true);
		Player2 = GetWorld()->SpawnActor<AMyCharacter>(Player2Class, SpawnLocation, FRotator::ZeroRotator);
		Player2Controller->Possess(Player2);
		UE_LOG(LogTemp, Warning, TEXT("Player 2 possessed by %d"), Player2Controller->GetLocalPlayer()->GetLocalPlayerIndex());
	}

	if(HighScoreWidgetClass)
	{
		HighScoreWidgetInstance = CreateWidget<UHighScoreWidget>(GetWorld(), HighScoreWidgetClass);

		HighScoreWidgetInstance->AddToViewport();
		HighScoreWidgetInstance->DisplayHighScore();
	}

	GetWorldTimerManager().SetTimer(GameSpeedUpdateHandle, this, &AMyRunnerGameMode::UpdateGameSpeed, GameSpeedUpdateInterval, true);

	UWorld* World = GetWorld();
	TArray<APlayerController*> Controllers;
	
	for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PlayerController = It->Get();
		if (PlayerController)
		{
			Controllers.Add(PlayerController);
		}
	}

	for (APlayerController* Controller : Controllers)
	{
		if (Controller)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found player controller with ID %d"), Controller->GetLocalPlayer()->GetControllerId());
		}
	}
}

void AMyRunnerGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AMyRunnerGameMode::UpdateGameSpeed()
{
	GameSpeed += 10.f;
}

int AMyRunnerGameMode::CheckPlayerLives() const
{
	return Player1->HealthAmount + Player2->HealthAmount;
}

void AMyRunnerGameMode::OnGameOver()
{
	CheckNewHighScore();
	
	if(GameOverWidgetClass)
	{
		UGameOverWidget* GameOverWidgetPtr = CreateWidget<UGameOverWidget>(GetWorld(), GameOverWidgetClass);
		GameOverWidgetPtr->AddToViewport();

		UButton* RestartButton = Cast<UButton>(GameOverWidgetPtr->GetWidgetFromName("RestartButton"));
		RestartButton->OnClicked.AddDynamic(GameOverWidgetPtr, &UGameOverWidget::HandleRestartButtonClicked);

		UButton* QuitButton = Cast<UButton>(GameOverWidgetPtr->GetWidgetFromName("QuitButton"));
		QuitButton->OnClicked.AddDynamic(GameOverWidgetPtr, &UGameOverWidget::HandleQuitButtonClicked);
	}

	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	Controller->SetInputMode(FInputModeUIOnly());
	Controller->bShowMouseCursor = true;
	Controller->SetPause(true);
}

void AMyRunnerGameMode::CheckNewHighScore()
{
	if(Player1->Score < HighScoreWidgetInstance->LoadHighScore() && Player2->Score < HighScoreWidgetInstance->LoadHighScore())
	{
		return;
	}
	if(Player1->Score > Player2->Score && Player1->Score > HighScoreWidgetInstance->LoadHighScore())
	{
		HighScoreWidgetInstance->SaveHighScore(Player1->Score);
	}
	if(Player2->Score > Player1->Score && Player2->Score > HighScoreWidgetInstance->LoadHighScore())
	{
		HighScoreWidgetInstance->SaveHighScore(Player2->Score);
	}
}

void AMyRunnerGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(CheckPlayerLives() == 0)
	{
		OnGameOver();
	}
}

