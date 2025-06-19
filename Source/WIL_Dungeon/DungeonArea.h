// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DungeonArea.generated.h"

USTRUCT(BlueprintType)
struct FDoorInfo {
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator Rotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* ParentArea;
};
UCLASS(Abstract)
class WIL_DUNGEON_API ADungeonArea : public AActor
{
	GENERATED_BODY()
	
public:
	ADungeonArea();
	virtual TArray<FDoorInfo> GetDoors() const PURE_VIRTUAL(ADungeonArea::GetDoors, return TArray<FDoorInfo>(););

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dungeon")
	TArray<FDoorInfo> Doors;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dungeon")
	UStaticMeshComponent* MeshComponent;

};
