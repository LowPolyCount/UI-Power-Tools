// Copyright 2025 Joel Gonzales


#include "UI/Screens/Tools/ComponentSelector.h"
#include "UI/Screens/UICS/ScreenComponent.h"
#include "UI/Screens/UICS/DataScreenComponent.h"
#include "UI/Screens/UICS/EntryScreenComponent.h"
#include "UI/Screens/UICS/ViewScreenComponent.h"
#include "UI/Screens/UICS/Transaction/ActionScreenComponent.h"

FComponentSelector::FComponentSelector()
#ifdef WITH_EDITORONLY_DATA
	:Super(UScreenComponent::StaticClass())
#endif
{

}

#ifdef WITH_EDITORONLY_DATA
FComponentSelector::FComponentSelector(const UClass* Filter)
	:Super(Filter)
{
}
#endif

void FComponentSelector::SetComponent(const UScreenComponent* InComponent)
{
	if (::IsValid(InComponent))
	{
		ComponentGuid = InComponent->GetGuid();
	}
}

FDataComponentSelector::FDataComponentSelector()
#ifdef WITH_EDITORONLY_DATA
	:Super(UDataScreenComponent::StaticClass())
#endif
{
}

FViewComponentSelector::FViewComponentSelector()
#ifdef WITH_EDITORONLY_DATA
	:Super(UViewScreenComponent::StaticClass())
#endif
{
}

FTransactionComponentSelector::FTransactionComponentSelector()
#ifdef WITH_EDITORONLY_DATA
	:Super(UActionScreenComponent::StaticClass())
#endif
{
}

FEntryComponentSelector::FEntryComponentSelector()
#ifdef WITH_EDITORONLY_DATA
	:Super(UEntryScreenComponent::StaticClass())
#endif
{
}