// Copyright 2026 Joel Gonzales and contributors. See LICENSE for further information

#pragma once

#include "CoreMinimal.h"
#include "UObject/Class.h"
#include "NativeGameplayTags.h"
#include "ActionResultData.generated.h"

// the result of the last action check
USTRUCT(BlueprintType)
struct FActionResultData
{
	GENERATED_BODY();
	FActionResultData();
	FActionResultData(bool bInResult, FGameplayTag InTag);

	// are we able to execute?
	UPROPERTY(BlueprintReadWrite)
	bool bActionResult = false;
	// Gameplay Tag containing more information about the result
	UPROPERTY(BlueprintReadWrite)
	FGameplayTag ActionResultTag;
};