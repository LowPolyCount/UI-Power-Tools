// Copyright 2025 Joel Gonzales


#include "UI/Screens/UICS/Transaction/ActionScreenComponentProvider.h"
#include "UI/Screens/UICS/Transaction/ActionScreenComponent.h"

FGameplayTag UActionScreenComponentProvider::ExecuteAction_Implementation(UActionScreenComponent* Component, UObject* Entry)
 { 
	return UICS_Action_Success;
}