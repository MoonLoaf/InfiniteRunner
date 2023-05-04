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
		HighScoreWidgetInstance->DisplayHighScores();
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
	TArray<float> HighScores = HighScoreWidgetInstance->LoadHighScores();

	bool bPlayer1ScoreAdded = false;
	bool bPlayer2ScoreAdded = false;

	for (int32 i = 0; i < HighScores.Num(); i++) {
		if (!bPlayer1ScoreAdded && Player1->Score > HighScores[i]) {
			HighScores.Insert(Player1->Score, i);
			bPlayer1ScoreAdded = true;
		}
		else if (!bPlayer2ScoreAdded && Player2->Score > HighScores[i]) {
			HighScores.Insert(Player2->Score, i);
			bPlayer2ScoreAdded = true;
		}
	}
	if(HighScores.Num()!= 0)
	{
		if (!bPlayer1ScoreAdded && Player1->Score > HighScores.Last()) {
			HighScores.Add(Player1->Score);
		}
		if (!bPlayer2ScoreAdded && Player2->Score > HighScores.Last()) {
			HighScores.Add(Player2->Score);
		}
	}

	HighScores.Sort([](float A, float B) {
		return A > B;
	});

	while (HighScores.Num() > 3) {
		HighScores.RemoveAt(HighScores.Num() - 1);
	}

	HighScoreWidgetInstance->SaveHighScores(HighScores);
}

void AMyRunnerGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(CheckPlayerLives() == 0)
	{
		OnGameOver();
	}
}

