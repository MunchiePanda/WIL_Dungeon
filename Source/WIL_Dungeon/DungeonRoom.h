// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DungeonArea.h"
#include "DungeonRoom.generated.h"

/**
 * 
 */
UCLASS()
class WIL_DUNGEON_API ADungeonRoom : public ADungeonArea
{
	GENERATED_BODY()
public:
	ADungeonRoom();
	virtual TArray<FDoorInfo> GetDoors() const override;
};
