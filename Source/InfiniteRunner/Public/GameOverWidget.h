#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverWidget.generated.h"

UCLASS()
class INFINITERUNNER_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	//Call from widget blueprint?
	UFUNCTION(BlueprintCallable)
    void HandleQuitButtonClicked();
};
