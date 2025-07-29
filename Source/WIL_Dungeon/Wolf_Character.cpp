// Fill out your copyright notice in the Description page of Project Settings.


#include "Wolf_Character.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"

void AWolf_Character::OnSeePawn(APawn* SeenPawn)
{
	ChasePlayer(SeenPawn);
}

void AWolf_Character::ChasePlayer(APawn* PlayerPawn)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController && PlayerPawn)
	{
		AIController->MoveToActor(PlayerPawn, 100.0f); // 100 units acceptance radius
	}
}

