// Copyright (c) Joel Gonzales


#include "Customizers/BaseSelectorCustomization.h"
#include "PropertyHandle.h"
#include "UI/Screens/UICSScreen.h"


void FBaseSelectorCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	PropertyToCustomize = PropertyHandle;
}

TScriptInterface<IUICSScreenAccessor> FBaseSelectorCustomization::GetScreenAccessor() const
{
	TScriptInterface<IUICSScreenAccessor> RetVal = nullptr;
	if (PropertyToCustomize.IsValid())
	{
		TArray<UObject*> Outers;
		PropertyToCustomize->GetOuterObjects(Outers);
		// which outer the screen is depends on where the selector is located
		if (Outers.IsValidIndex(0) && IsValid(Outers[0]))
		{
			// selector is a property on the screen
			if (Outers[0]->Implements<UUICSScreenAccessor>())
			{
				RetVal = TScriptInterface<IUICSScreenAccessor>(Outers[0]);
			}
			else // the selector is in blueprints, or is a child of another widget connected to the screen
			{
				UObjectBaseUtility* Outer = Outers[0]->GetImplementingOuterObject(UUICSScreenAccessor::StaticClass());
				RetVal = TScriptInterface<IUICSScreenAccessor>(Cast<UObject>(Outer));
			}
		}
		else
		{
			//UE_LOG(Warning, LogTemp, TEXT("FBaseSelectorCustomization: No screen was found for %s"), PropertyToCustomize->GetName());
		}
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("PropertyToCustomize is null"));
	}

	return RetVal;
}

bool FBaseSelectorCustomization::FilterEntry(const UObject* Entry) const
{
	return ((IsValid(ClassFilter)) ? Entry->IsA(ClassFilter) : true);
}


void FBaseSelectorCustomization::SetClassFilter(UClass* InClass)
{
	ClassFilter = TSubclassOf<UObject>(InClass);
}

const TSubclassOf<UObject>& FBaseSelectorCustomization::GetClassFilter() const
{
	return ClassFilter;
}
