// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemBase.h"
#include "InventoryComponent.generated.h"

// Inventory slot structure
USTRUCT(BlueprintType)
struct FInventorySlot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UItemBase* Item = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Quantity = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsEmpty = true;

    FInventorySlot()
    {
        Item = nullptr;
        Quantity = 0;
        bIsEmpty = true;
    }
};

// Equipment slot structure
USTRUCT(BlueprintType)
struct FEquipmentSlot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UItemBase* EquippedItem = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EEquipmentSlot SlotType = EEquipmentSlot::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsOccupied = false;

    FEquipmentSlot()
    {
        EquippedItem = nullptr;
        SlotType = EEquipmentSlot::None;
        bIsOccupied = false;
    }
};

// Inventory events
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemAdded, UItemBase*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemRemoved, UItemBase*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemUsed, UItemBase*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemEquipped, UItemBase*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemUnequipped, UItemBase*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryChanged, int32, SlotIndex);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WIL_DUNGEON_API UInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:    
    UInventoryComponent();

protected:
    virtual void BeginPlay() override;

public:    
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // Inventory management
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool AddItem(UItemBase* Item, int32 Quantity = 1);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool RemoveItem(int32 SlotIndex, int32 Quantity = 1);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool UseItem(int32 SlotIndex);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool EquipItem(int32 SlotIndex);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool UnequipItem(EEquipmentSlot SlotType);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void DropItem(int32 SlotIndex, FVector Location);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void SwapItems(int32 SlotIndex1, int32 SlotIndex2);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void SplitStack(int32 SlotIndex, int32 SplitAmount);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void CombineStacks(int32 SourceSlot, int32 TargetSlot);

    // Equipment management
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool EquipItemToSlot(UItemBase* Item, EEquipmentSlot SlotType);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    UItemBase* GetEquippedItem(EEquipmentSlot SlotType);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool IsSlotOccupied(EEquipmentSlot SlotType);

    // Utility functions
    UFUNCTION(BlueprintPure, Category = "Inventory")
    int32 GetInventorySize() const { return InventorySlots.Num(); }

    UFUNCTION(BlueprintPure, Category = "Inventory")
    bool IsInventoryFull() const;

    UFUNCTION(BlueprintPure, Category = "Inventory")
    int32 GetTotalWeight() const;

    UFUNCTION(BlueprintPure, Category = "Inventory")
    int32 GetTotalValue() const;

    UFUNCTION(BlueprintPure, Category = "Inventory")
    FInventorySlot GetSlot(int32 SlotIndex) const;

    UFUNCTION(BlueprintPure, Category = "Inventory")
    int32 FindItem(UItemBase* Item) const;

    UFUNCTION(BlueprintPure, Category = "Inventory")
    int32 FindEmptySlot() const;

    UFUNCTION(BlueprintPure, Category = "Inventory")
    int32 FindStackableSlot(UItemBase* Item) const;

    // Inventory properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    TArray<FInventorySlot> InventorySlots;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    TArray<FEquipmentSlot> EquipmentSlots;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    int32 MaxInventorySize = 20;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    float MaxWeight = 100.0f;

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Inventory")
    FOnItemAdded OnItemAdded;

    UPROPERTY(BlueprintAssignable, Category = "Inventory")
    FOnItemRemoved OnItemRemoved;

    UPROPERTY(BlueprintAssignable, Category = "Inventory")
    FOnItemUsed OnItemUsed;

    UPROPERTY(BlueprintAssignable, Category = "Inventory")
    FOnItemEquipped OnItemEquipped;

    UPROPERTY(BlueprintAssignable, Category = "Inventory")
    FOnItemUnequipped OnItemUnequipped;

    UPROPERTY(BlueprintAssignable, Category = "Inventory")
    FOnInventoryChanged OnInventoryChanged;

protected:
    // Helper functions
    void InitializeInventory();
    void InitializeEquipment();
    bool CanAddItem(UItemBase* Item, int32 Quantity) const;
    bool CanCarryWeight(float AdditionalWeight) const;
    void UpdateSlot(int32 SlotIndex);
    void BroadcastInventoryChanged(int32 SlotIndex);
};
