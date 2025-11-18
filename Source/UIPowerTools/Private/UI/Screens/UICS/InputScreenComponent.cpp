// Copyright (c) 2025 Joel Gonzales


#include "UI/Screens/UICS/InputScreenComponent.h"
#include "InputMappingContext.h"

#include "UI/Tools/UIPowerToolsDeveloperSettings.h"

/*UInputScreenComponent::UInputScreenComponent()
{
	if (const UUIPowerToolsDeveloperSettings* DefaultSettings = GetDefault<UUIPowerToolsDeveloperSettings>())
	{
		if(UInputMappingContext* DefaultMappingContext = Cast<UInputMappingContext>(DefaultSettings->DefaultInputMapping.TryLoad()))
		{
			SetInputMapping(DefaultMappingContext);
		}
	}
}*/

void UInputScreenComponent::NativeConstruct()
{
	Super::NativeConstruct();
}
