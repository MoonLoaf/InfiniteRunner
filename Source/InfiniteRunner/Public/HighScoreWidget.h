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

	UPROPERTY()
	TArray<float> TopScores;
	
public:

	UFUNCTION(BlueprintCallable, Category = "UI")
	void DisplayHighScores();

	void SaveHighScores(const TArray<float>& HighScores);
	
	TArray<float> LoadHighScores();
};
