// Author: Lucas Vilas-Boas
// Year: 2022
// Repo: https://github.com/lucoiso/UEAzSpeech

#pragma once

#include <CoreMinimal.h>
#include "AzSpeech/Tasks/Bases/AzSpeechAudioDataSynthesisBase.h"
#include "SSMLToAudioDataAsync.generated.h"

/**
 *
 */
UCLASS(NotPlaceable, Category = "AzSpeech")
class AZSPEECH_API USSMLToAudioDataAsync : public UAzSpeechAudioDataSynthesisBase
{
	GENERATED_BODY()

public:
	/* Task delegate that will be called when completed */
	UPROPERTY(BlueprintAssignable, Category = "AzSpeech")
	FAudioDataSynthesisDelegate SynthesisCompleted;

	/* Creates a SSML-To-AudioData task that will convert your SSML file to a audio data */
	UFUNCTION(BlueprintCallable, Category = "AzSpeech", meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "SSML To Audio Data"))
	static USSMLToAudioDataAsync* SSMLToAudioData(const UObject* WorldContextObject, const FString& SynthesisSSML);
	
protected:
	virtual void OnSynthesisUpdate(const std::shared_ptr<Microsoft::CognitiveServices::Speech::SpeechSynthesisResult>& LastResult) override;
	virtual void BroadcastFinalResult() override;
};
