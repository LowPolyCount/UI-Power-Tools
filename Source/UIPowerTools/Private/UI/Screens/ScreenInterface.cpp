// Copyright (c) 2025 Joel Gonzales


#include "UI/Screens/ScreenInterface.h"
#include "Blueprint/UserWidget.h"
#include "CommonActivatableWidget.h"

UUserWidget* IScreenInterface::AsUserWidget()
{
	return Cast<UUserWidget>(this);
}

void IScreenInterface::Close()
{
	TScriptInterface<IScreenInterface> AsInterface(AsUserWidget());
	GetOnScreenClose().Broadcast(AsInterface);
}

void IScreenInterface::AddScreenToViewport(int32 ZOrder)
{
	if (UUserWidget* AsUWidget = this->AsUserWidget())
	{
		AsUWidget->AddToViewport(ZOrder);
	}
}

void IScreenInterface::SetInputEnabled(bool bInEnabled)
{
	if (bInputEnabled != bInEnabled)
	{
		// Input is enabled by assuming the underlying derived widget inherits from UCommonActivatableWidget.
		if (UCommonActivatableWidget* AsActivatable = Cast<UCommonActivatableWidget>(this))
		{
			//UE_LOG(LogTemp, Warning, TEXT("Name: %s, Activate %d"), *AsActivatable->GetFName().ToString(), static_cast<int32>(bInEnabled));
			if (bInEnabled)
			{
				AsActivatable->ActivateWidget();
			}
			else
			{
				AsActivatable->DeactivateWidget();
			}
			bInputEnabled = bInEnabled;
			//@todo: need to make sure we disable Input on NativeDestruct?
		}
	}
}