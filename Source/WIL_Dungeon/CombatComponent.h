#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponBase.h"
#include "CombatComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WIL_DUNGEON_API UCombatComponent : public UActorComponent
{
    GENERATED_BODY()

public:    
    UCombatComponent();

    // Equips a weapon by index (0 = Dagger, 1 = Sword, 2 = Spear)
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void EquipWeapon(int32 WeaponIndex);

    // Triggers an attack
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void Attack();

protected:
    virtual void BeginPlay() override;

    // Currently equipped weapon
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
    AWeaponBase* EquippedWeapon;

    // Array of available weapons (set up in editor or at runtime)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    TArray<TSubclassOf<AWeaponBase>> WeaponClasses;

    // Spawned weapon actors
    UPROPERTY()
    TArray<AWeaponBase*> WeaponActors;

    // Reference to the owning character's stats component
    UPROPERTY()
    class UCharacterStatsComp* StatsComp;

    // Helper to spawn and attach weapons
    void InitializeWeapons();

    // Helper to play attack montage
    void PlayAttackMontage();

    // Helper to perform hit detection
    void PerformAttackTrace();

    // Input bindings
    void SetupInputBindings();

    // Input handlers
    void OnEquipWeapon1();
    void OnEquipWeapon2();
    void OnEquipWeapon3();
    void OnAttack();

    // Cached owner character
    UPROPERTY()
    class ACharacter* OwnerCharacter;
};