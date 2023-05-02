#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HighScoreWidget.generated.h"

UCLASS()
class INFINITERUNNER_API UHighScoreWidget : public UUserWidget
{
	GENERATED_BODY()

	float CurrentHighScore;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* HighScoreText;

public:

	UFUNCTION(BlueprintCallable, Category = "UI")
	void DisplayHighScore();

	void SaveHighScore(float HighScore);
	
	float LoadHighScore();
};
