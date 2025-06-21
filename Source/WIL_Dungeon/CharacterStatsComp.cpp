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
	if (CurrentStamina <= 0)
	{
		return true;    //return true if Drained
	}
	CurrentStamina -= DrainAmount * StaminaDrainModifier;
	FMath::Clamp(CurrentStamina, 0, MaxStamina);
	return false;
}

bool UCharacterStatsComp::RecoverStamina()
{
	if (CurrentStamina <= MaxStamina)
	{
		CurrentStamina -= StaminaRecoveryRate;
		FMath::Clamp(CurrentStamina, 0, MaxStamina);
		return true;    //return true if recovered
	}
	return false;
}
