// Copyright (c) 2025 Joel Gonzales


#include "UI/Screens/Widgets/HUDActor.h"
#include "UI/Screens/ScreenManager.h"
#include "UI/Tools/ScreenManagerSubsystem.h"
#include "Blueprint/UserWidget.h"

void AHUDActor::BeginPlay()
{
	Super::BeginPlay();

	CreateScreensAndAddToManager();

}

void AHUDActor::CreateScreensAndAddToManager()
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
