// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy_Character.generated.h"

UCLASS()
class WIL_DUNGEON_API AEnemy_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy_Character();

// Health
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
    float MaxHealth = 100.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy")
    float CurrentHealth = 100.0f;

    // Called when the enemy takes damage
	UFUNCTION(BlueprintCallable, Category = "Enemy")
    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

    // Called when the enemy is damaged (for child classes to override)
    UFUNCTION(BlueprintNativeEvent, Category = "Enemy")
    void OnDamaged(float DamageAmount, AActor* DamageCauser);

    // Called when the enemy dies
    UFUNCTION(BlueprintNativeEvent, Category = "Enemy")
    void OnDeath();


    // The item to drop on death (set in Blueprint defaults or per-instance)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drops")
    TSubclassOf<AActor> DropClass;

};
