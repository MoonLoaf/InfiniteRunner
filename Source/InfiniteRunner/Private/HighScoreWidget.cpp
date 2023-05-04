#include "HighScoreWidget.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "Components/TextBlock.h"

void UHighScoreWidget::DisplayHighScores()
{
	TArray<float> HighScores = LoadHighScores();
	
	FString HighScoreTextString = "High Scores:\n";
	for (int32 i = 0; i < HighScores.Num(); i++)
	{
		HighScoreTextString += FString::Printf(TEXT("%d. %d\n"), i+1, static_cast<int32>(HighScores[i]));
	}
	
	if (HighScoreText)
	{
		HighScoreText->SetText(FText::FromString(HighScoreTextString));
	}
}

void UHighScoreWidget::SaveHighScores(const TArray<float>& HighScores)
{
	FString SaveFilePath = FPaths::ProjectSavedDir() + "Highscores.json";
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	TArray<TSharedPtr<FJsonValue>> HighScoreArray;
	for (float HighScore : HighScores)
	{
		HighScoreArray.Add(MakeShareable(new FJsonValueNumber(HighScore)));
	}

	JsonObject->SetArrayField("HighScores", HighScoreArray);

	FString OutputString;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

	FFileHelper::SaveStringToFile(OutputString, *SaveFilePath);
}

TArray<float> UHighScoreWidget::LoadHighScores()
{
	FString LoadFilePath = FPaths::ProjectSavedDir() + "Highscores.json";
	FString JsonString;

	if (!FFileHelper::LoadFileToString(JsonString, *LoadFilePath))
	{
		return TArray<float>();
	}

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);

	if (!FJsonSerializer::Deserialize(JsonReader, JsonObject) || !JsonObject.IsValid())
	{
		return TArray<float>();
	}

	TArray<float> HighScores;
	TArray<TSharedPtr<FJsonValue>> HighScoreArray = JsonObject->GetArrayField("HighScores");
	int32 NumHighScores = HighScoreArray.Num();
	if (NumHighScores == 0)
	{
		HighScores.Init(0.f, 3);
	}
	else
	{
		for (const TSharedPtr<FJsonValue>& HighScoreValue : HighScoreArray)
		{
			float HighScore = HighScoreValue->AsNumber();
			HighScores.Add(HighScore);
		}
	}

	return HighScores;
}
