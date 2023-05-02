#include "GameOverWidget.h"
#include "Kismet/GameplayStatics.h"

void UGameOverWidget::HandleRestartButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), "LVL_Game");
}

void UGameOverWidget::HandleQuitButtonClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}

