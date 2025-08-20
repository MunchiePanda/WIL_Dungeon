// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UInventoryComponent;

/**
 * 
 */
UCLASS()
class WIL_DUNGEON_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	UInventoryComponent* BoundInventory = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void BindToInventory(UInventoryComponent* Inventory);

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void RebuildInventoryUI();

protected:
	UFUNCTION()
	void HandleInventoryChanged(int32 SlotIndex);

public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool UseSlot(int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool EquipSlot(int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void DropSlot(int32 SlotIndex);
};
