#include "GameOverWidget.h"
#include "Kismet/GameplayStatics.h"

void UGameOverWidget::HandleQuitButtonClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}

