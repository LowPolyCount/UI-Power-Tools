// Copyright (c) 2025 Joel Gonzales


#include "UI/Screens/Widgets/HUDBootStrap.h"
#include "UI/Screens/ScreenManager.h"
#include "UI/Tools/ScreenManagerSubsystem.h"
#include "Blueprint/UserWidget.h"

void AHUDBootStrap::BeginPlay()
{
	Super::BeginPlay();

	CreateScreensAndAddToManager();

}

void AHUDBootStrap::CreateScreensAndAddToManager()
{
	if (UScreenManagerSubsystem* System = UScreenManagerSubsystem::Get(GetWorld()))
	{
		if (UScreenManager* ScreenManager = UScreenManagerSubsystem::GetScreenManager(GetWorld()))
		{
			for (TSubclassOf<UUserWidget>& ScreenClass : ScreensToDisplayAtStart)
			{
				UUserWidget* Screen(CreateWidget(GetWorld(), ScreenClass));
				if (Screen)
				{
					ScreenManager->AddScreen(Screen);
				}
			}
		}
	}
}
