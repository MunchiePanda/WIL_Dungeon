// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "ItemBase.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

UInventoryComponent::UInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
    Super::BeginPlay();
    
    InitializeInventory();
    InitializeEquipment();
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInventoryComponent::InitializeInventory()
{
    InventorySlots.SetNum(MaxInventorySize);
    for (int32 i = 0; i < MaxInventorySize; ++i)
    {
        InventorySlots[i] = FInventorySlot();
    }
}

void UInventoryComponent::InitializeEquipment()
{
    // Initialize equipment slots for each equipment type
    EquipmentSlots.SetNum(static_cast<int32>(EEquipmentSlot::Necklace) + 1);
    
    for (int32 i = 0; i < EquipmentSlots.Num(); ++i)
    {
        EquipmentSlots[i] = FEquipmentSlot();
        EquipmentSlots[i].SlotType = static_cast<EEquipmentSlot>(i);
    }
}

bool UInventoryComponent::AddItem(UItemBase* Item, int32 Quantity)
{
    if (!Item || Quantity <= 0 || !CanAddItem(Item, Quantity))
        return false;

    // Try to find existing stack first
    if (Item->IsStackable())
    {
        int32 StackableSlot = FindStackableSlot(Item);
        if (StackableSlot != -1)
        {
            InventorySlots[StackableSlot].Quantity += Quantity;
            UpdateSlot(StackableSlot);
            OnItemAdded.Broadcast(Item);
            BroadcastInventoryChanged(StackableSlot);
            return true;
        }
    }

    // Find empty slot
    int32 EmptySlot = FindEmptySlot();
    if (EmptySlot != -1)
    {
        InventorySlots[EmptySlot].Item = Item;
        InventorySlots[EmptySlot].Quantity = Quantity;
        InventorySlots[EmptySlot].bIsEmpty = false;
        
        UpdateSlot(EmptySlot);
        OnItemAdded.Broadcast(Item);
        BroadcastInventoryChanged(EmptySlot);
        return true;
    }

    return false;
}

bool UInventoryComponent::RemoveItem(int32 SlotIndex, int32 Quantity)
{
    if (!InventorySlots.IsValidIndex(SlotIndex) || InventorySlots[SlotIndex].bIsEmpty)
        return false;

    FInventorySlot& Slot = InventorySlots[SlotIndex];
    UItemBase* Item = Slot.Item;

    if (Quantity >= Slot.Quantity)
    {
        // Remove entire stack
        Slot.Item = nullptr;
        Slot.Quantity = 0;
        Slot.bIsEmpty = true;
        
        OnItemRemoved.Broadcast(Item);
        BroadcastInventoryChanged(SlotIndex);
    }
    else
    {
        // Remove partial stack
        Slot.Quantity -= Quantity;
        UpdateSlot(SlotIndex);
        BroadcastInventoryChanged(SlotIndex);
    }

    return true;
}

bool UInventoryComponent::UseItem(int32 SlotIndex)
{
    if (!InventorySlots.IsValidIndex(SlotIndex) || InventorySlots[SlotIndex].bIsEmpty)
        return false;

    FInventorySlot& Slot = InventorySlots[SlotIndex];
    if (!Slot.Item->IsUsable())
        return false;

    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character)
        return false;

    if (Slot.Item->UseItem(Character))
    {
        OnItemUsed.Broadcast(Slot.Item);
        
        // Remove one item from stack
        RemoveItem(SlotIndex, 1);
        return true;
    }

    return false;
}

bool UInventoryComponent::EquipItem(int32 SlotIndex)
{
    if (!InventorySlots.IsValidIndex(SlotIndex) || InventorySlots[SlotIndex].bIsEmpty)
        return false;

    FInventorySlot& Slot = InventorySlots[SlotIndex];
    if (!Slot.Item->IsEquippable())
        return false;

    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character)
        return false;

    EEquipmentSlot EquipmentSlot = Slot.Item->GetEquipmentSlot();
    
    // Unequip current item if slot is occupied
    if (IsSlotOccupied(EquipmentSlot))
    {
        UnequipItem(EquipmentSlot);
    }

    // Equip the new item
    if (EquipItemToSlot(Slot.Item, EquipmentSlot))
    {
        OnItemEquipped.Broadcast(Slot.Item);
        return true;
    }

    return false;
}

bool UInventoryComponent::UnequipItem(EEquipmentSlot SlotType)
{
    int32 SlotIndex = static_cast<int32>(SlotType);
    if (!EquipmentSlots.IsValidIndex(SlotIndex) || !EquipmentSlots[SlotIndex].bIsOccupied)
        return false;

    UItemBase* Item = EquipmentSlots[SlotIndex].EquippedItem;
    if (!Item)
        return false;

    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character)
        return false;

    if (Item->UnequipItem(Character))
    {
        EquipmentSlots[SlotIndex].EquippedItem = nullptr;
        EquipmentSlots[SlotIndex].bIsOccupied = false;
        
        OnItemUnequipped.Broadcast(Item);
        return true;
    }

    return false;
}

void UInventoryComponent::DropItem(int32 SlotIndex, FVector Location)
{
    if (!InventorySlots.IsValidIndex(SlotIndex) || InventorySlots[SlotIndex].bIsEmpty)
        return;

    FInventorySlot& Slot = InventorySlots[SlotIndex];
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    
    if (Character && Slot.Item)
    {
        Slot.Item->DropItem(Character, Location);
        RemoveItem(SlotIndex, Slot.Quantity);
    }
}

void UInventoryComponent::SwapItems(int32 SlotIndex1, int32 SlotIndex2)
{
    if (!InventorySlots.IsValidIndex(SlotIndex1) || !InventorySlots.IsValidIndex(SlotIndex2))
        return;

    FInventorySlot Temp = InventorySlots[SlotIndex1];
    InventorySlots[SlotIndex1] = InventorySlots[SlotIndex2];
    InventorySlots[SlotIndex2] = Temp;

    UpdateSlot(SlotIndex1);
    UpdateSlot(SlotIndex2);
    
    BroadcastInventoryChanged(SlotIndex1);
    BroadcastInventoryChanged(SlotIndex2);
}

void UInventoryComponent::SplitStack(int32 SlotIndex, int32 SplitAmount)
{
    if (!InventorySlots.IsValidIndex(SlotIndex) || InventorySlots[SlotIndex].bIsEmpty)
        return;

    FInventorySlot& Slot = InventorySlots[SlotIndex];
    if (SplitAmount >= Slot.Quantity || SplitAmount <= 0)
        return;

    int32 EmptySlot = FindEmptySlot();
    if (EmptySlot != -1)
    {
        InventorySlots[EmptySlot].Item = Slot.Item;
        InventorySlots[EmptySlot].Quantity = SplitAmount;
        InventorySlots[EmptySlot].bIsEmpty = false;

        Slot.Quantity -= SplitAmount;
        
        UpdateSlot(EmptySlot);
        UpdateSlot(SlotIndex);
        
        BroadcastInventoryChanged(EmptySlot);
        BroadcastInventoryChanged(SlotIndex);
    }
}

void UInventoryComponent::CombineStacks(int32 SourceSlot, int32 TargetSlot)
{
    if (!InventorySlots.IsValidIndex(SourceSlot) || !InventorySlots.IsValidIndex(TargetSlot))
        return;

    FInventorySlot& Source = InventorySlots[SourceSlot];
    FInventorySlot& Target = InventorySlots[TargetSlot];

    if (Source.bIsEmpty || Target.bIsEmpty || Source.Item != Target.Item)
        return;

    if (Target.Quantity + Source.Quantity <= Source.Item->GetMaxStackSize())
    {
        Target.Quantity += Source.Quantity;
        Source.Item = nullptr;
        Source.Quantity = 0;
        Source.bIsEmpty = true;

        UpdateSlot(TargetSlot);
        UpdateSlot(SourceSlot);
        
        BroadcastInventoryChanged(TargetSlot);
        BroadcastInventoryChanged(SourceSlot);
    }
}

bool UInventoryComponent::EquipItemToSlot(UItemBase* Item, EEquipmentSlot SlotType)
{
    if (!Item)
        return false;

    int32 SlotIndex = static_cast<int32>(SlotType);
    if (!EquipmentSlots.IsValidIndex(SlotIndex))
        return false;

    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character)
        return false;

    if (Item->EquipItem(Character))
    {
        EquipmentSlots[SlotIndex].EquippedItem = Item;
        EquipmentSlots[SlotIndex].bIsOccupied = true;
        return true;
    }

    return false;
}

UItemBase* UInventoryComponent::GetEquippedItem(EEquipmentSlot SlotType)
{
    int32 SlotIndex = static_cast<int32>(SlotType);
    if (EquipmentSlots.IsValidIndex(SlotIndex))
    {
        return EquipmentSlots[SlotIndex].EquippedItem;
    }
    return nullptr;
}

bool UInventoryComponent::IsSlotOccupied(EEquipmentSlot SlotType)
{
    int32 SlotIndex = static_cast<int32>(SlotType);
    if (EquipmentSlots.IsValidIndex(SlotIndex))
    {
        return EquipmentSlots[SlotIndex].bIsOccupied;
    }
    return false;
}

bool UInventoryComponent::IsInventoryFull() const
{
    return FindEmptySlot() == -1;
}

int32 UInventoryComponent::GetTotalWeight() const
{
    int32 TotalWeight = 0;
    for (const FInventorySlot& Slot : InventorySlots)
    {
        if (!Slot.bIsEmpty && Slot.Item)
        {
            TotalWeight += Slot.Item->GetWeight() * Slot.Quantity;
        }
    }
    return TotalWeight;
}

int32 UInventoryComponent::GetTotalValue() const
{
    int32 TotalValue = 0;
    for (const FInventorySlot& Slot : InventorySlots)
    {
        if (!Slot.bIsEmpty && Slot.Item)
        {
            TotalValue += Slot.Item->GetValue() * Slot.Quantity;
        }
    }
    return TotalValue;
}

FInventorySlot UInventoryComponent::GetSlot(int32 SlotIndex) const
{
    if (InventorySlots.IsValidIndex(SlotIndex))
    {
        return InventorySlots[SlotIndex];
    }
    return FInventorySlot();
}

int32 UInventoryComponent::FindItem(UItemBase* Item) const
{
    for (int32 i = 0; i < InventorySlots.Num(); ++i)
    {
        if (!InventorySlots[i].bIsEmpty && InventorySlots[i].Item == Item)
        {
            return i;
        }
    }
    return -1;
}

int32 UInventoryComponent::FindEmptySlot() const
{
    for (int32 i = 0; i < InventorySlots.Num(); ++i)
    {
        if (InventorySlots[i].bIsEmpty)
        {
            return i;
        }
    }
    return -1;
}

int32 UInventoryComponent::FindStackableSlot(UItemBase* Item) const
{
    if (!Item || !Item->IsStackable())
        return -1;

    for (int32 i = 0; i < InventorySlots.Num(); ++i)
    {
        if (!InventorySlots[i].bIsEmpty && 
            InventorySlots[i].Item == Item && 
            InventorySlots[i].Quantity < Item->GetMaxStackSize())
        {
            return i;
        }
    }
    return -1;
}

bool UInventoryComponent::CanAddItem(UItemBase* Item, int32 Quantity) const
{
    if (!Item)
        return false;

    // Check weight limit
    if (!CanCarryWeight(Item->GetWeight() * Quantity))
        return false;

    // Check if inventory is full and item is not stackable
    if (IsInventoryFull() && !Item->IsStackable())
        return false;

    // Check if we can stack with existing items
    if (Item->IsStackable())
    {
        int32 StackableSlot = FindStackableSlot(Item);
        if (StackableSlot != -1)
            return true;
    }

    // Check if we have empty slots
    return FindEmptySlot() != -1;
}

bool UInventoryComponent::CanCarryWeight(float AdditionalWeight) const
{
    return (GetTotalWeight() + AdditionalWeight) <= MaxWeight;
}

void UInventoryComponent::UpdateSlot(int32 SlotIndex)
{
    if (InventorySlots.IsValidIndex(SlotIndex))
    {
        FInventorySlot& Slot = InventorySlots[SlotIndex];
        Slot.bIsEmpty = (Slot.Item == nullptr || Slot.Quantity <= 0);
    }
}

void UInventoryComponent::BroadcastInventoryChanged(int32 SlotIndex)
{
    OnInventoryChanged.Broadcast(SlotIndex);
}

