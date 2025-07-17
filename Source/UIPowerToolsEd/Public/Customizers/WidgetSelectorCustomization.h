// Copyright (c) Joel Gonzales

#pragma once
#include "CoreMinimal.h"
#include <vector>
#include "Widgets/Input/SComboBox.h"
#include "IPropertyTypeCustomization.h"
#include "Customizers/BaseSelectorCustomization.h"

class UUserWidget;
class UWidget;
class UWidgetTree;

namespace Customization
{
	// contains a representation of a widget attached to a screen that can be selected in editor to be retrieved at runtime.
	// contains info needed to retrieve the widget, it's name and path (if it's in an attached UserWidget)
	struct FWidgetNode
	{
		FWidgetNode();
		FWidgetNode(FName InName);
		FWidgetNode(FName InName, const TArray<FName>& InPath);
		bool operator==(const FWidgetNode& rhs) const;

		FText DisplayName() const;

		FName WidgetName;
		TArray<FName> Path;
		UClass* Class = nullptr;	// the class of the widget for filtering
		
	};

	// the editor customization class for Widget Selectors
	class UIPOWERTOOLSED_API FWidgetSelectorCustomization : public FBaseSelectorCustomization
	{
		bool FilterEntry(const UObject* Entry) const override;
	public:
		static TSharedRef<IPropertyTypeCustomization> MakeInstance();


		//~ Begin IPropertyTypeCustomization Interface
		virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
		virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
		//~ End IPropertyTypeCustomization Interface

	protected:
		// util functions for combo box
		TSharedRef<SWidget> OnGenerateWidget(TSharedRef<FWidgetNode> InItem);
		void OnWidgetSelectionChanged(TSharedPtr<FWidgetNode> Node, ESelectInfo::Type Type);

		TArray<TSharedRef<FWidgetNode>> GetWidgetsAttachedToScreen();

		void TraverseWidgetTree(const UWidgetTree* Tree, TArray<TSharedRef<FWidgetNode>>& RetVal, TArray<FName> Path);

		void SaveWidget(TSharedRef<FWidgetNode> InWidget);
		void LoadWidget();

		TArray<TSharedRef<FWidgetNode>> WidgetNodes;

		// slate widgets
		TSharedPtr<SComboBox<TSharedRef<FWidgetNode>>> Box;
	};
}