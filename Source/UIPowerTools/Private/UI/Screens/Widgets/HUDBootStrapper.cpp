// Copyright (c) 2025 Joel Gonzales


#include "UI/Screens/Widgets/HUDBootStrapper.h"
#include "UI/Screens/ScreenManager.h"
#include "UI/Tools/ScreenManagerSubsystem.h"
#include "Blueprint/UserWidget.h"

void AHUDBootStrapper::BeginPlay()
{
	Super::BeginPlay();

	CreateScreensAndAddToManager();

}

void AHUDBootStrapper::CreateScreensAndAddToManager()
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
