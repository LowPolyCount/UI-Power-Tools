// Copyright © 2026 Joel Gonzales and Contributors (https://codeberg.org/LowPolyCount/UI-Power-Tools)


#include "UI/Tools/ScreenManagerSubsystem.h"
#include "UI/Screens/ScreenManager.h"

UScreenManagerSubsystem* UScreenManagerSubsystem::Get(const UWorld* World)
{
	return World->GetSubsystem<UScreenManagerSubsystem>();
	//return Get();
}

/*UScreenManagerSubsystem* UScreenManagerSubsystem::Get()
{
	return GEngine->GetEngineSubsystem<UScreenManagerSubsystem>();
}*/

UScreenManager* UScreenManagerSubsystem::GetScreenManager(const UWorld* World)
{ 
	return Get(World)->GetScreenManager_Internal();
}

void UScreenManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	ScreenManager = NewObject<UScreenManager>(this);

	// in these cases, the ViewPortSubsystem is going to remove all of it's widgets, so we will do the same 
	//FWorldDelegates::LevelRemovedFromWorld.AddUObject(this, &ThisClass::HandleLevelRemovedFromWorld);
	FWorldDelegates::OnWorldCleanup.AddUObject(this, &ThisClass::HandleWorldCleanup);
	FWorldDelegates::OnWorldBeginTearDown.AddUObject(this, &ThisClass::HandleRemoveWorld);
	FWorldDelegates::OnPreWorldFinishDestroy.AddUObject(this, &ThisClass::HandleRemoveWorld);
}

void UScreenManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();

	RemoveAllScreensOnManager();

	//FWorldDelegates::LevelRemovedFromWorld.RemoveAll(this);
	FWorldDelegates::OnPreWorldFinishDestroy.RemoveAll(this);
	FWorldDelegates::OnWorldBeginTearDown.RemoveAll(this);
	FWorldDelegates::OnWorldCleanup.RemoveAll(this);
}

void UScreenManagerSubsystem::RemoveAllScreensOnManager()
{
	if (ScreenManager)
	{
		ScreenManager->RemoveAllScreens();
	}
}

void UScreenManagerSubsystem::HandleWorldCleanup(UWorld* InWorld, bool bSessionEnded, bool bCleanupResoures)
{
	RemoveAllScreensOnManager();
}

void UScreenManagerSubsystem::HandleRemoveWorld(UWorld* InWorld)
{
	RemoveAllScreensOnManager();
}

void UScreenManagerSubsystem::HandleLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	RemoveAllScreensOnManager();
}
