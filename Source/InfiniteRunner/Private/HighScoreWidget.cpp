#include "HighScoreWidget.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "Components/TextBlock.h"


void UHighScoreWidget::DisplayHighScore()
{
	if(HighScoreText)
	{
		int32 HighScoreInt = static_cast<int32>(LoadHighScore());
		HighScoreText->SetText(FText::FromString(FString::Printf(TEXT("High score: %d"), HighScoreInt)));
	}
}


void UHighScoreWidget::SaveHighScore(float HighScore)
{
	FString SaveFilePath = FPaths::ProjectSavedDir() + "Highscore.json";
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	JsonObject->SetNumberField("HighScore", HighScore);

	FString OutputString;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

	FFileHelper::SaveStringToFile(OutputString, *SaveFilePath);
}

float UHighScoreWidget::LoadHighScore()
{
	FString LoadFilePath = FPaths::ProjectSavedDir() + "Highscore.json";
	FString JsonString;

	if (!FFileHelper::LoadFileToString(JsonString, *LoadFilePath))
	{
		return 0.0f;
	}

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);

	if (!FJsonSerializer::Deserialize(JsonReader, JsonObject) || !JsonObject.IsValid())
	{
		return 0.0f;
	}

	float Highscore = 0.0f;
	if (JsonObject->TryGetNumberField("Highscore", Highscore))
	{
		return Highscore;
	}

	return 0.0f;
}