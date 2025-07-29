// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy_Character.h"
#include "Boar_Character.generated.h"

/**
 * 
 */
UCLASS()
class WIL_DUNGEON_API ABoar_Character : public AEnemy_Character
{
	GENERATED_BODY()

protected:
	virtual void OnDamaged(float DamageAmount, AActor* DamageCauser) override;
	void ChargePlayer(AActor* DamageCauser);
};
