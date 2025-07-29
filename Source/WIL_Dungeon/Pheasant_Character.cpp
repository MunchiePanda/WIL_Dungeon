// Fill out your copyright notice in the Description page of Project Settings.


#include "Pheasant_Character.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"

void APheasant_Character::OnSeePawn(APawn* SeenPawn)
{
	FleeFromPlayer(SeenPawn);
}

void APheasant_Character::FleeFromPlayer(APawn* PlayerPawn)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController && PlayerPawn)
	{
		FVector MyLocation = GetActorLocation();
		FVector PlayerLocation = PlayerPawn->GetActorLocation();
		FVector FleeDirection = (MyLocation - PlayerLocation).GetSafeNormal();
		FVector FleeDestination = MyLocation + FleeDirection * 800.0f; // Flee 800 units away
		AIController->MoveToLocation(FleeDestination, 100.0f);
	}
}

