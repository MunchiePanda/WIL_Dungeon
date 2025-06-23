#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Animation/AnimMontage.h"
#include "WeaponBase.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    Dagger      UMETA(DisplayName = "Dagger"),
    Sword       UMETA(DisplayName = "Sword"),
    Spear       UMETA(DisplayName = "Spear")
};

UCLASS()
class WIL_DUNGEON_API AWeaponBase : public AActor
{
    GENERATED_BODY()
    
public:    
    AWeaponBase();

    // Weapon damage
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float Damage = 10.0f;

    // Skeletal mesh for the weapon
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    USkeletalMeshComponent* WeaponMesh;

    // Attack animation montage
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    UAnimMontage* AttackMontage;

    // Weapon type
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    EWeaponType WeaponType;

    // Name of the socket to attach to
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    FName AttachSocketName = "WeaponSocket";
};