#include "MyUIclass.h"
#include "Components/TextBlock.h"

void UMyUIclass::UpdateScoreText(float Value)
{
	ScoreText = Cast<UTextBlock>(GetWidgetFromName(TEXT("ScoreText")));

	if (ScoreText)
	{
		int32 ScoreInt = static_cast<int32>(Value);
		ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), ScoreInt)));
	}
}

void UMyUIclass::UpdateHealthText(int Value)
{
	HealthText = Cast<UTextBlock>(GetWidgetFromName(TEXT("HealthText")));

	if (HealthText)
	{
		HealthText->SetText(FText::FromString(FString::Printf(TEXT("Health: %d"), Value)));
	}
}
