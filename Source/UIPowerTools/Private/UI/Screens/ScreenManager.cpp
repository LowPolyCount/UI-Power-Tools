// Copyright (c) 2025 Joel Gonzales


#include "UI/Screens/ScreenManager.h"
#include "UI/Screens/Widgets/HUDActor.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Input/UIActionBindingHandle.h"
#include "Misc/Optional.h"
#include "Input/CommonUIActionRouterBase.h"
#include "TimerManager.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Tools/UIPowerToolsDeveloperSettings.h"
#include "CommonActivatableWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Blueprint/GameViewportSubsystem.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"

FScreenStruct::FScreenStruct(UUserWidget* InScreen, bool bHideScreensBelow)
	:Screen(InScreen)
	, bHideScreensBelow(bHideScreensBelow)
{

}

void FScreenStruct::SetVisibility(bool bIsVisible)
{
	if (Screen)
	{
		if (UUserWidget* AsUWidget = Screen)
		{
			AsUWidget->SetVisibility((bIsVisible) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
		}
		else
		{
			// @todo: log error
		}
	}
	else
	{
		// @todo: log error
	}

}

void FScreenStruct::SetEnableInput(bool bEnableInput)
{
	if (UCommonActivatableWidget* AsActivatableWidget = Cast<UCommonActivatableWidget>(Screen))
	{
		if (bEnableInput)
		{
			AsActivatableWidget->ActivateWidget();
		}
		else
		{
			AsActivatableWidget->DeactivateWidget();
		}
	}
	else
	{
		// @todo: log error
	}
}

UScreenManager::UScreenManager()
	:Super()
	// Set here so that we don't have to expose InputScreenComponent.h in this class's header
//	, CurrentInputMode(EScreenInputMode::None)
{

}


void UScreenManager::BeginPlay()
{
	/*RootWidget = Cast<UCommonActivatableWidget>(UUserWidget::CreateWidgetInstance(*GetWorld(), UCommonActivatableWidget::StaticClass(), FName()));
	if (ZValue == INDEX_NONE)
	{
		const UUIPowerToolsDeveloperSettings* Settings = GetDefault<UUIPowerToolsDeveloperSettings>();
		ZValue = (Settings) ? Settings->ScreenManagerZOrderStart : 1;
	}

	if (RootWidget)
	{
		RootWidget->AddToViewport(ZValue);
		Cast< UCommonActivatableWidget>(RootWidget)->ActivateWidget();
		if (UOverlay* AsOverlay = RootWidget->WidgetTree->ConstructWidget<UOverlay>())
		{
			RootWidget->WidgetTree->RootWidget = Cast<UWidget>(AsOverlay);
			ParentPanel = AsOverlay;
		}

		if (ParentPanel)
		{
			TSubclassOf<UUserWidget> AsSubclass(UScreenBaseWidget::StaticClass());

			if (UCommonActivatableWidget* AsCommon = Cast<UCommonActivatableWidget>(UUserWidget::CreateWidgetInstance(*GetWorld(), UCommonActivatableWidget::StaticClass(), FName())))
			{
				if (UOverlaySlot* Slot = ParentPanel->AddChildToOverlay(AsCommon))
				{
					Slot->SetHorizontalAlignment(HAlign_Fill);
					Slot->SetVerticalAlignment(VAlign_Fill);
				}
				AsCommon->ActivateWidget();
				if (UOverlay* AsOverlay = AsCommon->WidgetTree->ConstructWidget<UOverlay>())
				{
					AsCommon->WidgetTree->RootWidget = Cast<UWidget>(AsOverlay);
					ParentPanel = AsOverlay;
				}
			}
		}
		if (UGameViewportSubsystem* Subsystem = UGameViewportSubsystem::Get(GetWorld()))
		{
			UE_LOG(LogTemp, Warning, TEXT(""));
		}

		if (!IsReady())
		{
			check(GetWorld());
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UScreenManagerSubsystem::HandleOnAddToViewport, 0.01, true);// true); //FTimerDelegate::CreateWeakLambda(this, [this, Screen, bHideScreensBelow]()
			//FORCEINLINE void SetTimer(FTimerHandle& InOutHandle, FTimerDynamicDelegate const& InDynDelegate, float InRate, bool InbLoop, float InFirstDelay = -1.f)
		}
	//}*/
}

void UScreenManager::EndPlay(const EEndPlayReason::Type Reason)
{
	for(int i=Screens.Num() - 1; i >= 0; --i)
	{
		if (Screens[i].Screen)
		{
			Screens[i].Screen->RemoveFromParent();
		}
	}
}

UScreenManager* UScreenManager::Get(const UObject* WorldContextObject)
{
	UScreenManager* RetVal = nullptr;
	if(const APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (const AHUDActor* HudActor = Cast<AHUDActor>(PC->GetHUD()))
		{
			RetVal = HudActor->GetScreenManager();
		}
		else
		{
			if (PC->GetHUD())
			{
				UE_LOG(LogTemp, Error, TEXT("HUD on the player controller is a %s, not a AHUDActor. Have you changed the HUD class in the GameMode to use HudActor?"), *PC->GetHUD()->GetName());
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("HUD on the player controller does not exist.  "));
			}
			
			
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("There was no player controller at index 0"));
	}

	return RetVal;
}

/*UScreenManagerSubsystem& UScreenManagerSubsystem::GetChecked(UWorld* WorldContextObject)
{
	UScreenManagerSubsystem* ContainerSubsystem = Get(WorldContextObject);
	check(ContainerSubsystem);
	return *ContainerSubsystem;
}*/

bool UScreenManager::IsReady() const
{
	bool RetVal = false;
	if (RootWidget)
	{
		RetVal = RootWidget->IsInViewport();
	}

	return RetVal;
}

void UScreenManager::HandleOnReady()
{
	OnReady.Broadcast();
}

void UScreenManager::HandleOnAddToViewport()
{
	//if (GEngine)
	//{
		/*if (UGameViewportSubsystem* Subsystem = UGameViewportSubsystem::Get())
		{
			if (!Subsystem->IsWidgetAdded(RootWidget))
			{
				//UGameViewportClient::
				RootWidget->AddToViewport(ZValue);

				// no error message is given if AddToViewport failed, so check it here instead
				if (Subsystem->IsWidgetAdded(RootWidget))
				{
					Cast<UCommonActivatableWidget>(RootWidget)->ActivateWidget();
					check(GetWorld());
					GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

					// We need to wait until next frame to actually "Be ready" otherwise activatable widgets will not fire correctly
					GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UScreenManagerSubsystem::HandleOnReady);
				}
			}
			else
			{
				

			}
		}*/
	//}
}

/*void UScreenManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
	if (RootWidget)
	{
		RootWidget->RemoveFromParent();
		RootWidget = nullptr;
	}
}*/


void UScreenManager::AddScreen(UUserWidget* Screen, bool bHideScreensBelow)
{	
	if (Screen)
	{
		FScreenStruct TopScreen(Screen, bHideScreensBelow);
		AddScreen_Internal(TopScreen);

		ActivateTopScreen();
		
	}
	else
	{
		// @todo: log that the screen sent in is NULL;
	}
}

void UScreenManager::RemoveScreen(UUserWidget* Screen)
{ 
	if (IsValid(Screen)) 
	{ 
		Screen->RemoveFromParent(); 
	} 
}

bool UScreenManager::IsScreenOnStack(UUserWidget* Screen) const
{
	FScreenStruct TempScreenStruct(Screen);
	return Screens.Find(TempScreenStruct) != INDEX_NONE;
}

bool UScreenManager::IsScreenOfClassOnStack(TSubclassOf<UUserWidget> Class) const
{
	bool bRetVal = false;
	for (const FScreenStruct& IterScreen : Screens)
	{
		if (UObject* AsObject = IterScreen.Screen)
		{
			if (AsObject->IsA(Class))
			{
				bRetVal = true;
				break;
			}
		}

	}

	return bRetVal;
}

UUserWidget* UScreenManager::GetScreenOnTop() const
{
	UUserWidget* RetVal = nullptr;

	const int32 TopIndex = Screens.Num() - 1;
	if (Screens.IsValidIndex(TopIndex))
	{
		RetVal = Screens[TopIndex].Screen;
	}

	return RetVal;
}

void UScreenManager::HandleOnNativeDestruct(UUserWidget* Screen)
{
	if (Screen)
	{
		// make a temp struct so that we hold on to Screen, but also can use the == operator 
		FScreenStruct SearchStruct(Screen);
		Screens.Remove(SearchStruct);

		ActivateTopScreen();
	}
}

void UScreenManager::RemoveScreenUsingPanel(UUserWidget* Screen)
{
	if (Screen)
	{
		Screen->RemoveFromParent();
	}
}

void UScreenManager::RemoveScreenUsingViewport(UUserWidget* Screen)
{
	if (Screen)
	{
		Screen->RemoveFromParent();
	}
}

void UScreenManager::AddScreen_Internal(FScreenStruct& ScreenToAdd)
{
	if (ScreenToAdd.Screen)
	{
		Screens.Add(ScreenToAdd);
		ScreenToAdd.Screen->OnNativeDestruct.AddUObject(this, &UScreenManager::HandleOnNativeDestruct);

		// for now, we're using the viewport to add. Panel still requires some R&D
		AddScreenUsingViewport(ScreenToAdd);
	}
	else
	{
		// @todo: log error
	}
	
}

void UScreenManager::AddScreenUsingPanel(FScreenStruct& ScreenToAdd)
{
	if (ParentPanel)
	{
		if (UOverlaySlot* Slot = ParentPanel->AddChildToOverlay(Cast<UWidget>(ScreenToAdd.Screen)))
		{
			Slot->SetHorizontalAlignment(HAlign_Fill);
			Slot->SetVerticalAlignment(VAlign_Fill);
		}
	}
	else
	{
		// @todo: log error
	}
}

void UScreenManager::AddScreenUsingViewport(FScreenStruct& ScreenToAdd)
{
	ScreenToAdd.Screen->AddToViewport(++ZValue);
}

void UScreenManager::ActivateTopScreen()
{
	const int32 TopIndex = Screens.Num() - 1;
	
	if (Screens.IsValidIndex(TopIndex))
	{
		if (UCommonUIActionRouterBase* ActionRouter = UCommonUIActionRouterBase::Get(*Cast<UWidget>(Screens[TopIndex].Screen)))
		{
			UCommonActivatableWidget* AsCommon = Cast<UCommonActivatableWidget>(Screens[TopIndex].Screen);
			if (!ActionRouter->IsWidgetInActiveRoot(AsCommon))
			{
				TOptional<FUIInputConfig> InputConfig = AsCommon->GetDesiredInputConfig();
				if (InputConfig.IsSet())
				{
					ActionRouter->SetActiveUIInputConfig(InputConfig.GetValue());
				}
			}

		}

		for (int32 i = TopIndex; i >= 0; --i)
		{
			if (Screens.IsValidIndex(i))
			{
				FScreenStruct& ScreenToProcess = Screens[TopIndex];
				check(ScreenToProcess.Screen);
				ScreenToProcess.SetVisibility(true);
				ScreenToProcess.SetEnableInput(true);

				if (ScreenToProcess.bHideScreensBelow)
				{
					// hide all below
					for (int32 RemainingScreens = i - 1; RemainingScreens >= 0; --RemainingScreens)
					{
						if (Screens.IsValidIndex(RemainingScreens))
						{
							FScreenStruct& ScreenToHide = Screens[RemainingScreens];
							check(ScreenToHide.Screen);
							ScreenToHide.SetVisibility(false);
							ScreenToHide.SetEnableInput(false);
						}
						else
						{
							// @todo: log that index is wrong
						}
					}
					break;
				}

			}
			else
			{
				// @todo: log that it's not a UserWidget
			}
		}
	}
	else
	{
		// @todo: log that it's not a UserWidget
	}


}

/*void UScreenManager::SetInputMode(APlayerController* PlayerController, const UInputScreenComponent* InputComponent)
{
	if (InputComponent)
	{
		const EScreenInputMode NextMode = InputComponent->GetInputMode();

		// small problems like cursor being reset can happen if we set the Input Mode to what it already is
		if (NextMode != CurrentInputMode)
		{
			switch (NextMode)
			{
			case EScreenInputMode::GameAndUI:
				UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerController, nullptr, InputComponent->GetMouseLockMode(), InputComponent->GetHideCursorDuringCapture(), InputComponent->GetFlushInput());
				break;
			case EScreenInputMode::GameOnly:
				UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController, InputComponent->GetFlushInput());
				break;
			case EScreenInputMode::UIOnly:
				UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PlayerController, nullptr, InputComponent->GetMouseLockMode(), InputComponent->GetFlushInput());
				break;
			default:
				checkNoEntry();
				break;
			}

			CurrentInputMode = NextMode;
		}
	}
}*/

APlayerController* UScreenManager::GetPlayerController()
{

	check(GetWorld());
	//@note:  For now, we get the player controller from the first player
	return UGameplayStatics::GetPlayerController(GetWorld(), 0);
}
