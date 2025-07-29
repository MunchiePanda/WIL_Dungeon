// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy_Character.h"
#include "Pheasant_Character.generated.h"

/**
 * 
 */
UCLASS()
class WIL_DUNGEON_API APheasant_Character : public AEnemy_Character
{
	GENERATED_BODY()

protected:
	virtual void OnSeePawn(APawn* SeenPawn) override;
	void FleeFromPlayer(APawn* PlayerPawn);
};
