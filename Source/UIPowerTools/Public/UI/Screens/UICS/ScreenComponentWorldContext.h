// Copyright 2025 Joel Gonzales

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ScreenComponentWorldContext.generated.h"

/**
 * Implements GetWorld for Screen Component and related classes so that World Context Blueprint Nodes can be used
 */
UCLASS()
class UIPOWERTOOLS_API UScreenComponentWorldContext : public UObject
{
	GENERATED_BODY()

public:
	virtual UWorld* GetWorld() const override;
};
