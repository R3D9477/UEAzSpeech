// Author: Lucas Vilas-Boas
// Year: 2022
// Repo: https://github.com/lucoiso/UEAzSpeech

#pragma once

#include <CoreMinimal.h>
#include <Runtime/Launch/Resources/Version.h>
#include "LogAzSpeech.h"

struct FAzSpeechRecognitionMap;

namespace AzSpeech::Internal
{
	template<typename Ty>
	constexpr const bool HasEmptyParam(const Ty& Arg1)
	{
		if constexpr (std::is_base_of<FString, Ty>())
		{
			return Arg1.IsEmpty();
		}
		else if constexpr (std::is_base_of<FName, Ty>())
		{
			return Arg1.IsNone();
		}
		else if constexpr (std::is_base_of<std::string, Ty>())
		{
			return Arg1.empty();
		}
		else
		{
#if ENGINE_MAJOR_VERSION >= 5
			return Arg1.IsEmpty();
#else
			return Arg1.Num() == 0;
#endif
		}
	}

	template<typename Ty, typename ...Args>
	constexpr const bool HasEmptyParam(const Ty& Arg1, Args&& ...args)
	{
		return HasEmptyParam(Arg1) || HasEmptyParam(std::forward<Args>(args)...);
	}

	template<typename ReturnTy, typename IteratorTy>
	constexpr const ReturnTy GetDataFromMapGroup(const FName& InGroup, const TArray<IteratorTy> InContainer)
	{
		if (HasEmptyParam(InGroup))
		{
			UE_LOG(LogAzSpeech_Internal, Error, TEXT("%s: Invalid group name"), *FString(__func__));
			return ReturnTy();
		}

		for (const IteratorTy& IteratorData : InContainer)
		{
			if (IteratorData.GroupName.IsEqual(InGroup))
			{
				if (HasEmptyParam(IteratorData.Data))
				{
					UE_LOG(LogAzSpeech_Internal, Error, TEXT("%s: Map group %s has empty data"), *FString(__func__), *IteratorData.GroupName.ToString());
					return ReturnTy();
				}

				if constexpr (std::is_base_of<FAzSpeechRecognitionMap, ReturnTy>())
				{
					return IteratorData;
				}
				else
				{
					return IteratorData.Data;
				}
			}
		}

		UE_LOG(LogAzSpeech_Internal, Error, TEXT("%s: Group with name %s not found"), *FString(__func__), *InGroup.ToString());
		return ReturnTy();
	}
}
