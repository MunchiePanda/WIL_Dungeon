// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataAsset.h"
#include "ItemBase.generated.h"

// Item types for different categories
UENUM(BlueprintType)
enum class EItemType : uint8
{
    None        UMETA(DisplayName = "None"),
    Weapon      UMETA(DisplayName = "Weapon"),
    Armor       UMETA(DisplayName = "Armor"),
    Consumable  UMETA(DisplayName = "Consumable"),
    Material    UMETA(DisplayName = "Material"),
    Quest       UMETA(DisplayName = "Quest"),
    Currency    UMETA(DisplayName = "Currency")
};

// Item rarity levels
UENUM(BlueprintType)
enum class EItemRarity : uint8
{
    Common      UMETA(DisplayName = "Common"),
    Uncommon    UMETA(DisplayName = "Uncommon"),
    Rare        UMETA(DisplayName = "Rare"),
    Epic        UMETA(DisplayName = "Epic"),
    Legendary   UMETA(DisplayName = "Legendary")
};

// Equipment slot types
UENUM(BlueprintType)
enum class EEquipmentSlot : uint8
{
    None        UMETA(DisplayName = "None"),
    Weapon      UMETA(DisplayName = "Weapon"),
    Shield      UMETA(DisplayName = "Shield"),
    Helmet      UMETA(DisplayName = "Helmet"),
    Chest       UMETA(DisplayName = "Chest"),
    Legs        UMETA(DisplayName = "Legs"),
    Boots       UMETA(DisplayName = "Boots"),
    Gloves      UMETA(DisplayName = "Gloves"),
    Ring        UMETA(DisplayName = "Ring"),
    Necklace    UMETA(DisplayName = "Necklace")
};

// Item data structure
USTRUCT(BlueprintType)
struct FItemData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemName = "Default Item";

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemDescription = "Default description";

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* ItemIcon = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UStaticMesh* ItemMesh = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EItemType ItemType = EItemType::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EItemRarity ItemRarity = EItemRarity::Common;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxStackSize = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Weight = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Value = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsStackable = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsUsable = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsEquippable = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EEquipmentSlot EquipmentSlot = EEquipmentSlot::None;

    // Combat stats for weapons/armor
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Damage = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Armor = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Durability = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxDurability = 100.0f;
};

UCLASS(BlueprintType, Blueprintable)
class WIL_DUNGEON_API UItemBase : public UDataAsset
{
    GENERATED_BODY()

public:
    UItemBase();

    // Item data
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
    FItemData ItemData;

    // Item ID for identification
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
    FString ItemID;

    // Item functions
    UFUNCTION(BlueprintCallable, Category = "Item")
    virtual bool UseItem(class ACharacter* Character);

    UFUNCTION(BlueprintCallable, Category = "Item")
    virtual bool EquipItem(class ACharacter* Character);

    UFUNCTION(BlueprintCallable, Category = "Item")
    virtual bool UnequipItem(class ACharacter* Character);

    UFUNCTION(BlueprintCallable, Category = "Item")
    virtual void DropItem(class ACharacter* Character, FVector Location);

    // Utility functions
    UFUNCTION(BlueprintPure, Category = "Item")
    bool IsStackable() const { return ItemData.bIsStackable; }

    UFUNCTION(BlueprintPure, Category = "Item")
    bool IsUsable() const { return ItemData.bIsUsable; }

    UFUNCTION(BlueprintPure, Category = "Item")
    bool IsEquippable() const { return ItemData.bIsEquippable; }

    UFUNCTION(BlueprintPure, Category = "Item")
    EItemType GetItemType() const { return ItemData.ItemType; }

    UFUNCTION(BlueprintPure, Category = "Item")
    EItemRarity GetItemRarity() const { return ItemData.ItemRarity; }

    UFUNCTION(BlueprintPure, Category = "Item")
    EEquipmentSlot GetEquipmentSlot() const { return ItemData.EquipmentSlot; }

    UFUNCTION(BlueprintPure, Category = "Item")
    FString GetItemName() const { return ItemData.ItemName; }

    UFUNCTION(BlueprintPure, Category = "Item")
    FString GetItemDescription() const { return ItemData.ItemDescription; }

    UFUNCTION(BlueprintPure, Category = "Item")
    int32 GetMaxStackSize() const { return ItemData.MaxStackSize; }

    UFUNCTION(BlueprintPure, Category = "Item")
    float GetWeight() const { return ItemData.Weight; }

    UFUNCTION(BlueprintPure, Category = "Item")
    int32 GetValue() const { return ItemData.Value; }

    UFUNCTION(BlueprintPure, Category = "Item")
    float GetDamage() const { return ItemData.Damage; }

    UFUNCTION(BlueprintPure, Category = "Item")
    float GetArmor() const { return ItemData.Armor; }

    UFUNCTION(BlueprintPure, Category = "Item")
    float GetDurability() const { return ItemData.Durability; }

    UFUNCTION(BlueprintPure, Category = "Item")
    float GetMaxDurability() const { return ItemData.MaxDurability; }

protected:
    // Override these in derived classes for specific behavior
    UFUNCTION(BlueprintImplementableEvent, Category = "Item")
    void OnUseItem(class ACharacter* Character);

    UFUNCTION(BlueprintImplementableEvent, Category = "Item")
    void OnEquipItem(class ACharacter* Character);

    UFUNCTION(BlueprintImplementableEvent, Category = "Item")
    void OnUnequipItem(class ACharacter* Character);

    UFUNCTION(BlueprintImplementableEvent, Category = "Item")
    void OnDropItem(class ACharacter* Character, FVector Location);
};
