// Author: Lucas Vilas-Boas
// Year: 2022
// Repo: https://github.com/lucoiso/UEAzSpeech

#pragma once

#include <CoreMinimal.h>
#include "AzSpeechAudioInputDeviceInfo.generated.h"

USTRUCT(BlueprintType, Category = "AzSpeech")
struct FAzSpeechAudioInputDeviceInfo
{
	GENERATED_USTRUCT_BODY()

	FAzSpeechAudioInputDeviceInfo() : DeviceName("Default Device"), DeviceID(FString()) {}
	FAzSpeechAudioInputDeviceInfo(const FString& InName, const FString& InID) : DeviceName(InName), DeviceID(InID) {}

	static constexpr auto InvalidDeviceID = "INVALID_DEVICE";
	static constexpr auto PlaceholderDeviceID = "00000000-0000-0000-0000-000000000000";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AzSpeech")
	FString DeviceName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AzSpeech", Meta = (Getter = "GetDeviceID"))
	FString DeviceID;

	const FString GetDeviceID() const
	{
		FString Output = DeviceID;
		if (!Output.StartsWith("{"))
		{
			Output.InsertAt(0, "{");
		}

		if (!Output.EndsWith("}"))
		{
			Output.Append("}");
		}

		return Output;
	}
	
	const FString GetAudioInputDeviceEndpointID() const
	{
		// {0.0.1.00000000}.{Device ID}
		return FString::Format(TEXT("{0}.{1}"), { FString("{0.0.1.00000000}"), GetDeviceID() });
	}

	const bool operator==(const FAzSpeechAudioInputDeviceInfo& Rhs) const
	{
		return DeviceID == Rhs.DeviceID;
	}
};