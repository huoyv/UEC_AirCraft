// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UEC_AirCraftGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class UEC_AIRCRAFT_API AUEC_AirCraftGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	// 全局变量
	UPROPERTY(VisibleAnywhere)
	uint32 points;
	
};
