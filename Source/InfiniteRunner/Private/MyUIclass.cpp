#include "MyUIclass.h"
#include "MyCharacter.h"
#include "Components/TextBlock.h"

void UMyUIclass::UpdateScoreText()
{
	MyCharacter = Cast<AMyCharacter>(GetOwningPlayerPawn());
	ScoreText = Cast<UTextBlock>(GetWidgetFromName(TEXT("ScoreText")));

	if (ScoreText && MyCharacter)
	{
		int32 ScoreInt = static_cast<int32>(MyCharacter->Score);
		ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), ScoreInt)));
	}
}

void UMyUIclass::UpdateHealthText()
{
	MyCharacter = Cast<AMyCharacter>(GetOwningPlayerPawn());
	HealthText = Cast<UTextBlock>(GetWidgetFromName(TEXT("HealthText")));

	if (HealthText && MyCharacter)
	{
		HealthText->SetText(FText::FromString(FString::Printf(TEXT("Health: %d"), MyCharacter->HealthAmount)));
	}
}
