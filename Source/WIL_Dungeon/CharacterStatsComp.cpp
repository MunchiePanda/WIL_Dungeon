#include "CharacterStatsComp.h"

// Sets default values for this component's properties
UCharacterStatsComp::UCharacterStatsComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCharacterStatsComp::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCharacterStatsComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UCharacterStatsComp::DrainStamina(float DrainAmount)
{
	//RETURN bDrained
	if (CurrentStamina <= 0)
	{
		return true;    //return true if Drained
	}
	UE_LOG(LogTemp, Log, TEXT("DRAINING STAMINA! Current: %f"), CurrentStamina);
	CurrentStamina = CurrentStamina - DrainAmount;	//(Add later: DrainAmount * DrainModifier 
	FMath::Clamp(CurrentStamina, 0, MaxStamina);
	return false;
}

bool UCharacterStatsComp::RecoverStamina()
{
	//return bRecovered
	if (CurrentStamina >= MaxStamina)
	{
		return true;    //return true if recovered
	}
	UE_LOG(LogTemp, Log, TEXT("RECOVERING STAMINA! Current: %f"), CurrentStamina);
	CurrentStamina += StaminaRecoveryRate;
	FMath::Clamp(CurrentStamina, 0, MaxStamina);
	return false;
}

bool UCharacterStatsComp::IncreaseSystemExposure()
{
	if (CurrentSystemExposure < MaxSystemExposure) 
	{
		CurrentSystemExposure -= 1.0f;
		return false;
	}
	return true;	//is increased to max (begin health drain)
}

void UCharacterStatsComp::DrainHealth(float DrainAmount)
{
	if (CurrentHealth <= MaxHealth)
	{
		CurrentHealth -= DrainAmount;
		FMath::Clamp(CurrentStamina, 0, MaxStamina);
	}
}

