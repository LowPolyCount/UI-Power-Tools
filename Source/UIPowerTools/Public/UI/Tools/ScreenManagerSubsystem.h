// Copyright 2026 Joel Gonzales and Contributors

#include "CoreMinimal.h"
#include "Blueprint/GameViewportSubsystem.h"
#include "ScreenManagerSubsystem.generated.h"

class UScreenManager;

/**
 * Manages the ScreenManager.  As a subsystem, it allows global access the screen manager 
 * @note: We remove all screens on the screen manager on world transition as the GameViewPortSubsystem removes all widgets
 */
UCLASS()
//class UIPOWERTOOLS_API UScreenManagerSubsystem : public UEngineSubsystem
class UIPOWERTOOLS_API UScreenManagerSubsystem : public UWorldSubsystem
//class UIPOWERTOOLS_API UScreenManagerSubsystem : public UGameViewportSubsystem
{
	GENERATED_BODY()
public:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override { return true; }

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	static UScreenManagerSubsystem* Get(const UWorld* World);
	//static UScreenManagerSubsystem* Get();

	UFUNCTION(BlueprintCallable)
	static UScreenManager* GetScreenManager(const UWorld* World);

protected:
	UPROPERTY()
	TObjectPtr<UScreenManager> ScreenManager;

	UScreenManager* GetScreenManager_Internal() const { return ScreenManager; }

	void RemoveAllScreensOnManager();
	void HandleWorldCleanup(UWorld* InWorld, bool bSessionEnded, bool bCleanupResoures);
	void HandleRemoveWorld(UWorld* InWorld);
	void HandleLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld);

};
