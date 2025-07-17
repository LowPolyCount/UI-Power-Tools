// Copyright (c) Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"

class UScreen;

// the base class to provide editor customization for selector properties
class UIPOWERTOOLSED_API FBaseSelectorCustomization : public IPropertyTypeCustomization
{
public:
	//~ Begin IPropertyTypeCustomization Interface
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	//~ End IPropertyTypeCustomization Interface


protected:
	void SetClassFilter(UClass* InClass);
	const TSubclassOf<UObject>& GetClassFilter() const;

	UScreen* GetScreen() const;
	
	virtual bool FilterEntry(const UObject* Entry) const;

	TSubclassOf<UObject> ClassFilter;
	TSharedPtr<IPropertyHandle> PropertyToCustomize;
	int32 SelectedIndex = INDEX_NONE;
	// slate widgets
	TSharedPtr<STextBlock> SelectedWidgetName;

};