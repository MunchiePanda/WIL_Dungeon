// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DungeonArea.h"
#include "DungeonHallway.generated.h"

/**
 * 
 */
UCLASS()
class WIL_DUNGEON_API ADungeonHallway : public ADungeonArea
{
	GENERATED_BODY()
public:
	ADungeonHallway();
	virtual TArray<FDoorInfo> GetDoors() const override;
};
