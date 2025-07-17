// Copyright Epic Games, Inc. All Rights Reserved.

#include "UIPowerToolsEd.h"
#include "PropertyEditorModule.h"
#include "Customizers/WidgetSelectorCustomization.h"
#include "Customizers/ComponentSelectorCustomization.h"
#include "UI/Screens/Tools/WidgetSelector.h"
#include "UI/Screens/Tools/ComponentSelector.h"
#include "UObject/Class.h"

#define LOCTEXT_NAMESPACE "FUIPowerToolsEdModule"

void FUIPowerToolsEdModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	{
		// widget selectors
		PropertyModule.RegisterCustomPropertyTypeLayout(FWidgetSelector::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&Customization::FWidgetSelectorCustomization::MakeInstance));
		PropertyModule.RegisterCustomPropertyTypeLayout(FPanelWidgetSelector::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&Customization::FWidgetSelectorCustomization::MakeInstance));
		// component selectors
		PropertyModule.RegisterCustomPropertyTypeLayout(FComponentSelector::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&Customization::FComponentSelectorCustomization::MakeInstance));
		PropertyModule.RegisterCustomPropertyTypeLayout(FDataComponentSelector::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&Customization::FComponentSelectorCustomization::MakeInstance));
		PropertyModule.RegisterCustomPropertyTypeLayout(FViewComponentSelector::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&Customization::FComponentSelectorCustomization::MakeInstance));
		PropertyModule.RegisterCustomPropertyTypeLayout(FTransactionComponentSelector::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&Customization::FComponentSelectorCustomization::MakeInstance));
		PropertyModule.RegisterCustomPropertyTypeLayout(FEntryComponentSelector::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&Customization::FComponentSelectorCustomization::MakeInstance));
		
	}
}

void FUIPowerToolsEdModule::ShutdownModule()
{
	if (UObjectInitialized())
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		{
			// widget selectors
			PropertyModule.UnregisterCustomPropertyTypeLayout(FWidgetSelector::StaticStruct()->GetFName());
			PropertyModule.UnregisterCustomPropertyTypeLayout(FPanelWidgetSelector::StaticStruct()->GetFName());
			// component selectors
			PropertyModule.UnregisterCustomPropertyTypeLayout(FComponentSelector::StaticStruct()->GetFName());
			PropertyModule.UnregisterCustomPropertyTypeLayout(FDataComponentSelector::StaticStruct()->GetFName());
			PropertyModule.UnregisterCustomPropertyTypeLayout(FViewComponentSelector::StaticStruct()->GetFName());
			PropertyModule.UnregisterCustomPropertyTypeLayout(FTransactionComponentSelector::StaticStruct()->GetFName());
			PropertyModule.UnregisterCustomPropertyTypeLayout(FEntryComponentSelector::StaticStruct()->GetFName());
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUIPowerToolsEdModule, UIPowerToolsEd)