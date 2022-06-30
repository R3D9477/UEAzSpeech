// Author: Lucas Vilas-Boas
// Year: 2022
// Repo: https://github.com/lucoiso/UEAzSpeech

#include "AzSpeech/AzSpeechHelper.h"
#include "AzSpeech.h"
#include "Sound/SoundWave.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFileManager.h"

#if PLATFORM_ANDROID
#include "AndroidPermissionFunctionLibrary.h"
#endif

bool UAzSpeechHelper::IsAzSpeechDataEmpty(const FAzSpeechData Data)
{
	return Data.LanguageID.IsEmpty() || Data.RegionID.IsEmpty() || Data.APIAccessKey.IsEmpty();
}

FString UAzSpeechHelper::QualifyWAVFileName(const FString& Path, const FString& Name)
{
	FString LocalPath = Path;

	if (*Path.end() != '/')
	{
		LocalPath += '/';
	}

	FString LocalName = Name;
	if (Name.Right(Name.Len() - 4) != ".wav")
	{
		LocalName += ".wav";
	}

	const FString QualifiedName = LocalPath + LocalName;
	UE_LOG(LogAzSpeech, Log, TEXT("AzSpeech - %s: Qualified WAV file name: %s"), *FString(__func__), *QualifiedName);

	return QualifiedName;
}

USoundWave* UAzSpeechHelper::ConvertFileToSoundWave(const FString& FilePath, const FString& FileName)
{
	if (!FilePath.IsEmpty() && !FileName.IsEmpty())
	{
		if (const FString& Full_FileName = QualifyWAVFileName(FilePath, FileName);
			FPlatformFileManager::Get().GetPlatformFile().FileExists(*Full_FileName))
		{
#if PLATFORM_ANDROID
			if (!UAndroidPermissionFunctionLibrary::CheckPermission(FString("android.permission.READ_EXTERNAL_STORAGE")))
			{
				UAndroidPermissionFunctionLibrary::AcquirePermissions(TArray<FString>{ FString("android.permission.READ_EXTERNAL_STORAGE") });
			}
#endif
			if (TArray<uint8> RawData;
				FFileHelper::LoadFileToArray(RawData, *QualifyWAVFileName(FilePath, FileName), FILEREAD_NoFail))
			{
				UE_LOG(LogAzSpeech, Display, TEXT("AzSpeech - %s: Result: Success"), *FString(__func__));
				return ConvertStreamToSoundWave(RawData);
			}
			// else
			UE_LOG(LogAzSpeech, Error, TEXT("AzSpeech - %s: Result: Failed to load file"), *FString(__func__));
		}
		else
		{
			UE_LOG(LogAzSpeech, Error, TEXT("AzSpeech - %s: Result: File not found"), *FString(__func__));
		}
	}
	else
	{
		UE_LOG(LogAzSpeech, Error, TEXT("AzSpeech - %s: FilePath or FileName is empty"), *FString(__func__));
	}

	return nullptr;
}

USoundWave* UAzSpeechHelper::ConvertStreamToSoundWave(const TArray<uint8> RawData)
{
	#if ENGINE_MAJOR_VERSION >= 5
    	if (!RawData.IsEmpty())
    #else
    	if (RawData.Num() != 0)
    #endif
	{
		if (USoundWave* SoundWave = NewObject<USoundWave>())
		{
			FWaveModInfo WaveInfo;
			WaveInfo.ReadWaveInfo(RawData.GetData(), RawData.Num());

			const int32 ChannelCount = *WaveInfo.pChannels;
			const int32 SizeOfSample = *WaveInfo.pBitsPerSample / 8;
			const int32 NumSamples = WaveInfo.SampleDataSize / SizeOfSample;
			const int32 NumFrames = NumSamples / ChannelCount;

			SoundWave->Duration = NumFrames / *WaveInfo.pSamplesPerSec;
			SoundWave->NumChannels = ChannelCount;
			SoundWave->TotalSamples = *WaveInfo.pSamplesPerSec * SoundWave->Duration;
			SoundWave->SetSampleRate(*WaveInfo.pSamplesPerSec);

#if ENGINE_MAJOR_VERSION >= 5
			SoundWave->SetImportedSampleRate(*WaveInfo.pSamplesPerSec);
#endif

			SoundWave->RawPCMDataSize = WaveInfo.SampleDataSize;
			SoundWave->RawPCMData = static_cast<uint8*>(FMemory::Malloc(WaveInfo.SampleDataSize));
			FMemory::Memcpy(SoundWave->RawPCMData, WaveInfo.SampleDataStart, WaveInfo.SampleDataSize);

			UE_LOG(LogAzSpeech, Display, TEXT("AzSpeech - %s: Result: Success"), *FString(__func__));
			return SoundWave;
		}
		// else
		UE_LOG(LogAzSpeech, Error, TEXT("AzSpeech - %s: Cannot create a new Sound Wave"), *FString(__func__));
	}
	else
	{
		UE_LOG(LogAzSpeech, Error, TEXT("AzSpeech - %s: RawData is empty"), *FString(__func__));
	}

	return nullptr;
}
