// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DungeonArea.h"
#include "DungeonExit.generated.h"

/**
 * 
 */
UCLASS()
class WIL_DUNGEON_API ADungeonExit : public ADungeonArea
{
	GENERATED_BODY()
public:
	ADungeonExit();
	virtual TArray<FDoorInfo> GetDoors() const override { return TArray<FDoorInfo>(); } // No further doors
};
