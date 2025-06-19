// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonArea.h"

// Sets default values
ADungeonArea::ADungeonArea()
{
	
		PrimaryActorTick.bCanEverTick = false;
		MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
		RootComponent = MeshComponent;
	
}



