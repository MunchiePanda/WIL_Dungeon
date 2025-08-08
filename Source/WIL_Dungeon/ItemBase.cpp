// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "CharacterStatsComp.h"

UItemBase::UItemBase()
{
    // Initialize default values
    ItemID = "DefaultItem";
}

bool UItemBase::UseItem(ACharacter* Character)
{
    if (!Character || !ItemData.bIsUsable)
        return false;

    // Call blueprint implementable event
    OnUseItem(Character);

    // Default behavior - can be overridden in Blueprint
    if (ItemData.ItemType == EItemType::Consumable)
    {
        // Apply consumable effects
        if (UCharacterStatsComp* StatsComp = Character->FindComponentByClass<UCharacterStatsComp>())
        {
            // Example: Health potion
            if (ItemData.ItemName.Contains("Health"))
            {
                StatsComp->CurrentHealth = FMath::Min(StatsComp->CurrentHealth + 25.0f, StatsComp->MaxHealth);
            }
            // Example: Stamina potion
            else if (ItemData.ItemName.Contains("Stamina"))
            {
                StatsComp->CurrentStamina = FMath::Min(StatsComp->CurrentStamina + 5.0f, StatsComp->MaxStamina);
            }
        }
    }

    return true;
}

bool UItemBase::EquipItem(ACharacter* Character)
{
    if (!Character || !ItemData.bIsEquippable)
        return false;

    // Call blueprint implementable event
    OnEquipItem(Character);

    // Default behavior - can be overridden in Blueprint
    if (UCharacterStatsComp* StatsComp = Character->FindComponentByClass<UCharacterStatsComp>())
    {
        // Apply equipment stats
        if (ItemData.ItemType == EItemType::Weapon)
        {
            StatsComp->BaseAttack += ItemData.Damage;
        }
        else if (ItemData.ItemType == EItemType::Armor)
        {
            StatsComp->DamageModifier = FMath::Max(0.1f, StatsComp->DamageModifier - (ItemData.Armor / 100.0f));
        }
    }

    return true;
}

bool UItemBase::UnequipItem(ACharacter* Character)
{
    if (!Character || !ItemData.bIsEquippable)
        return false;

    // Call blueprint implementable event
    OnUnequipItem(Character);

    // Default behavior - can be overridden in Blueprint
    if (UCharacterStatsComp* StatsComp = Character->FindComponentByClass<UCharacterStatsComp>())
    {
        // Remove equipment stats
        if (ItemData.ItemType == EItemType::Weapon)
        {
            StatsComp->BaseAttack -= ItemData.Damage;
        }
        else if (ItemData.ItemType == EItemType::Armor)
        {
            StatsComp->DamageModifier = FMath::Min(1.0f, StatsComp->DamageModifier + (ItemData.Armor / 100.0f));
        }
    }

    return true;
}

void UItemBase::DropItem(ACharacter* Character, FVector Location)
{
    if (!Character)
        return;

    // Call blueprint implementable event
    OnDropItem(Character, Location);

    // Default behavior - spawn item in world
    if (ItemData.ItemMesh)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = Character;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        // You would typically spawn a pickup actor here
        // GetWorld()->SpawnActor<AItemPickup>(Location, FRotator::ZeroRotator, SpawnParams);
    }
}

