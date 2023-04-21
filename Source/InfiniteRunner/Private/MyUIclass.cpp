#include "MyUIclass.h"

#include "MyCharacter.h"
#include "Components/TextBlock.h"

void UMyUIclass::UpdateScoreText()
{

	if(MyCharacter == nullptr)
	{
		MyCharacter = Cast<AMyCharacter>(GetOwningPlayerPawn()); 
	}
	ScoreText = Cast<UTextBlock>(GetWidgetFromName(TEXT("ScoreText")));
	if (ScoreText)
	{
		ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), MyCharacter->Score)));
	}

}

void UMyUIclass::UpdateHealthText()
{
	if(MyCharacter == nullptr)
	{
		MyCharacter = Cast<AMyCharacter>(GetOwningPlayerPawn()); 
	}
	HealthText = Cast<UTextBlock>(GetWidgetFromName(TEXT("HealthText")));
	if (HealthText && MyCharacter)
	{
		HealthText->SetText(FText::FromString(FString::Printf(TEXT("Health: %d"), MyCharacter->HealthAmount)));
	}
}
