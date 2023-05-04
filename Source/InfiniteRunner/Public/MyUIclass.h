#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyUIclass.generated.h"

UCLASS()
class INFINITERUNNER_API UMyUIclass : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateScoreText(float Value);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateHealthText(int Value);

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ScoreText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* HealthText;
};
