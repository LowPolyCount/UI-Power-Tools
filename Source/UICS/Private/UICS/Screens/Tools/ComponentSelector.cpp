// Copyright 2025 Joel Gonzales


#include "UICS/Screens/Tools/ComponentSelector.h"
#include "UICS/Screens/Components/ScreenComponent.h"
#include "UICS/Screens/Components/Data/DataScreenComponent.h"
#include "UICS/Screens/Components/EntryScreenComponent.h"
#include "UICS/Screens/Components/Display/DisplayScreenComponent.h"
#include "UICS/Screens/Components/Action/ActionScreenComponent.h"

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
	:Super(UDisplayScreenComponent::StaticClass())
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