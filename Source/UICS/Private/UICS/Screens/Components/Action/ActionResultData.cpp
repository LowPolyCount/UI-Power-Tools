// Copyright 2026 Joel Gonzales and contributors. See LICENSE for further information


#include "UICS/Screens/Components/Action/ActionResultData.h"

UICS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UICS_ACTION_Default);			// Default Value

FActionResultData::FActionResultData()
	:bActionResult(false)
	, ActionResultTag(UICS_ACTION_Default)
{
}

FActionResultData::FActionResultData(bool bInResult, FGameplayTag InTag)
	:bActionResult(bInResult)
	, ActionResultTag(InTag)
{ 
}
