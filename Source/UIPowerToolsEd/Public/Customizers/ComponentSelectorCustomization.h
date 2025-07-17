// Copyright (c) Joel Gonzales

#pragma once
#include "CoreMinimal.h"
#include <vector>
#include "Widgets/Input/SComboBox.h"
#include "IPropertyTypeCustomization.h"
#include "Customizers/BaseSelectorCustomization.h"

class UScreenComponent;

namespace Customization
{
	// contains a representation of a widget attached to a screen that can be selected in editor to be retrieved at runtime.
	// contains info needed to retrieve the widget, it's name and path (if it's in an attached UserWidget)
	struct FComponentNode
	{
		public:
			FComponentNode() = default;
			FComponentNode(const UScreenComponent* InComponent, int32 InIndex);
			bool operator==(const FComponentNode& rhs) const;

			FString DisplayName() const;
			FString DisplayNameVerbose() const;

			const UScreenComponent* SelectedComponent = nullptr;
			int32 Index = INDEX_NONE;	// index this component has in the array
			//UClass* Class = nullptr;	// the class of the widget for filtering
	};

	// the editor customization class for Widget Selectors
	class UIPOWERTOOLSED_API FComponentSelectorCustomization : public FBaseSelectorCustomization
	{
		//bool FilterEntry(const UObject* Entry) const override;
	public:
		static TSharedRef<IPropertyTypeCustomization> MakeInstance();


		//~ Begin IPropertyTypeCustomization Interface
		virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
		virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
		//~ End IPropertyTypeCustomization Interface

	protected:
		// util functions for combo box
		TSharedRef<SWidget> OnGenerateWidget(TSharedRef<FComponentNode> InItem);
		void OnWidgetSelectionChanged(TSharedPtr<FComponentNode> Node, ESelectInfo::Type Type);

		TArray<TSharedRef<FComponentNode>> GetComponentsOnScreen();

		void SaveScreenComponent(TSharedRef<FComponentNode> InWidget);
		void LoadScreenComponent();

		// slate widgets
		TSharedPtr<SComboBox<TSharedRef<FComponentNode>>> Box;
		TArray<TSharedRef<FComponentNode>> ComponentNodes;
	};
}