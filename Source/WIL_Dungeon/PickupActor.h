// PickupActor.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.h"
#include "PickupActor.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UPrimitiveComponent;
struct FHitResult;

UCLASS(Blueprintable)
class WIL_DUNGEON_API APickupActor : public AActor
{
	GENERATED_BODY()

public:
	APickupActor();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* Sphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Mesh;

	// Data Asset that defines this pickup
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	UItemBase* ItemData = nullptr;

	// Number of items to add to inventory on pickup
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup", meta=(ClampMin="1"))
	int32 Quantity = 1;

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};

