// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DungeonArea.h"
#include "DungeonCave.generated.h"

/**
 * 
 */
UCLASS()
class WIL_DUNGEON_API ADungeonCave : public ADungeonArea
{
	GENERATED_BODY()
public:
    ADungeonCave();
    virtual TArray<FDoorInfo> GetDoors() const override;
};
