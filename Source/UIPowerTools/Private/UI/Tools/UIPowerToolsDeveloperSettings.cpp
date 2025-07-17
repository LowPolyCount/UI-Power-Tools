// Copyright (c) 2025 Joel Gonzales


#include "UI/Tools/UIPowerToolsDeveloperSettings.h"

FName UUIPowerToolsDeveloperSettings::GetCategoryName() const
{
	return TEXT("Plugins");
}

#if WITH_EDITOR
FText UUIPowerToolsDeveloperSettings::GetSectionText() const
{
	return NSLOCTEXT("UIPowerToolsAdvanced", "Systems", "UI Power Tools");
}

FText UUIPowerToolsDeveloperSettings::GetSectionDescription() const
{
	return NSLOCTEXT("UIPowerToolsAdvanced", "SystemsDescription", "All Options provided by UI Power Tools");
}
#endif