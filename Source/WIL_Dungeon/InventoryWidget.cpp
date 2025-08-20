// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "InventoryComponent.h"

void UInventoryWidget::BindToInventory(UInventoryComponent* Inventory)
{
	if (!Inventory) return;
	BoundInventory = Inventory;
	BoundInventory->OnInventoryChanged.AddDynamic(this, &UInventoryWidget::HandleInventoryChanged);
	RebuildInventoryUI();
}

void UInventoryWidget::HandleInventoryChanged(int32 SlotIndex)
{
	RebuildInventoryUI();
}

bool UInventoryWidget::UseSlot(int32 SlotIndex)
{
	return BoundInventory ? BoundInventory->UseItem(SlotIndex) : false;
}

bool UInventoryWidget::EquipSlot(int32 SlotIndex)
{
	return BoundInventory ? BoundInventory->EquipItem(SlotIndex) : false;
}

void UInventoryWidget::DropSlot(int32 SlotIndex)
{
	if (!BoundInventory) return;
	APawn* OwnerPawn = GetOwningPlayerPawn();
	if (!OwnerPawn) return;
	const FVector DropLocation = OwnerPawn->GetActorLocation() + OwnerPawn->GetActorForwardVector() * 100.f;
	BoundInventory->DropItem(SlotIndex, DropLocation);
}

