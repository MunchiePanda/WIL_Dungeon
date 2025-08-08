// DungeonCharacter.h
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "CharacterStatsComp.h"
#include "InventoryComponent.h"
//#include "AdvancedCombatComponent.h"
#include "DungeonCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class WIL_DUNGEON_API ADungeonCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ADungeonCharacter();

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USpringArmComponent* SpringArm;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UCameraComponent* Camera;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UCharacterStatsComp* Stats;     //Custom UActorComponent to hold Character's Stats

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UInventoryComponent* Inventory;     //Inventory system component

    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    //UAdvancedCombatComponent* AdvancedCombat;     //Advanced combat system

protected:
    void MoveForward(float Value);
    void MoveRight(float Value);
    void LookUp(float Value);
    void LookRight(float Value);

    void StartSprint(); 
    void StopSprint();

    // Advanced combat input functions
    //void OnLightAttack();
    //void OnHeavyAttack();
    //void OnStartBlock();
    //void OnEndBlock();
    //void OnDodge();
    //void OnStartAim();
    //void OnEndAim();
    //void OnFire();
    //void OnReload();
    //void OnSwitchFireMode();

    FTimerHandle StaminaTimer;
};