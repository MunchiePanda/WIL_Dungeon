#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStatsComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WIL_DUNGEON_API UCharacterStatsComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterStatsComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	//PROPERTIES
	//Health: When reaches 0, character dies
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	float CurrentHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	float DamageModifier;	//percentage of attack that player takes
							//eg. EnemyAttack * 0.90 = DamageDone -> CurrentHealth -= DamageDone

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	float MaxSystemExposure;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	float CurrentSystemExposure;

	//Stamina: Depleted as character attacks, sprints, dodges, jumping
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	float MaxStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	float CurrentStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	float StaminaRecoveryRate;		//rate at which stamina is recovered per second
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	float StaminaDrainModifier;		//% modifier for rate at which stamina is drained

	//Movement Speeds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	float WalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	float SprintSpeed;

	//Carry Capacity: Inventory size
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	int CarryCapacity;	//Max number of Inventory Slots

	//Attack: Damage caused to other's when attacking
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	float BaseAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
	float AttackModifier;	//Damage done per weapon type in % (1.10 = +10% damage)
							//Duplicate this as needed for each weapon type
	
	UFUNCTION()
	bool DrainStamina(float DrainAmount);
	UFUNCTION()
	bool RecoverStamina();

	UFUNCTION()
	bool IncreaseSystemExposure();

	UFUNCTION()
	void DrainHealth(float DrainAmount);



private:
	bool bStaminaDraining;	//if stamina is currently being drained
};
