// Copyright (c) 2025 Joel Gonzales


#include "UI/Screens/Widgets/HUDActor.h"
#include "UI/Screens/ScreenManager.h"
#include "Blueprint/UserWidget.h"

AHUDActor::AHUDActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ScreenManager = ObjectInitializer.CreateDefaultSubobject<UScreenManager>(this, MakeUniqueObjectName(this, UScreenManager::StaticClass()));
}

void AHUDActor::BeginPlay()
{
	Super::BeginPlay();

	if (ScreenManager)
	{
		ScreenManager->BeginPlay();
	}

	CreateScreensAndAddToManager();

}

void AHUDActor::EndPlay(const EEndPlayReason::Type Reason)
{
	if (ScreenManager)
	{
		ScreenManager->EndPlay(Reason);
	}
	Super::EndPlay(Reason);
}

void AHUDActor::CreateScreensAndAddToManager()
{
	if (ScreenManager)
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
