// Fill out your copyright notice in the Description page of Project Settings.


#include "Boar_Character.h"
#include "AIController.h"

void ABoar_Character::OnDamaged(float DamageAmount, AActor* DamageCauser)
{
	ChargePlayer(DamageCauser);
}

void ABoar_Character::ChargePlayer(AActor* DamageCauser)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	APawn* PlayerPawn = Cast<APawn>(DamageCauser);
	if (AIController && PlayerPawn)
	{
		AIController->MoveToActor(PlayerPawn, 50.0f); // 50 units acceptance radius for a charge
	}
}

