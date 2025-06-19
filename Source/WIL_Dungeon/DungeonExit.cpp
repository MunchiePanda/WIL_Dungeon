// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonExit.h"
#include "Components/StaticMeshComponent.h"

ADungeonExit::ADungeonExit() {
    if (MeshComponent) {
        static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
        if (MeshAsset.Succeeded()) {
            MeshComponent->SetStaticMesh(MeshAsset.Object);
            MeshComponent->SetWorldScale3D(FVector(2.f, 2.f, 5.f)); // Tall cylinder as exit marker
        }
    }
}
