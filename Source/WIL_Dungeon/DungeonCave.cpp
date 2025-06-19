// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonCave.h"
#include "Components/StaticMeshComponent.h"

ADungeonCave::ADungeonCave() {
    if (MeshComponent)
    {
        static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
        if (MeshAsset.Succeeded())
        {
            MeshComponent->SetStaticMesh(MeshAsset.Object);
            MeshComponent->SetWorldScale3D(FVector(10.f, 10.f, 1.f));
        }
    }
    Doors.Add(FDoorInfo{ FVector(500.f, 0.f, 0.f), FRotator(0.f, 90.f, 0.f), nullptr }); // East, face east
}

TArray<FDoorInfo> ADungeonCave::GetDoors() const
{
    TArray<FDoorInfo> WorldDoors;
    for (const FDoorInfo& Door : Doors)
    {
        FVector WorldLocation = GetActorLocation() + GetActorRotation().RotateVector(Door.Location);
        FRotator WorldRotation = GetActorRotation() + Door.Rotation;
        WorldDoors.Add(FDoorInfo{ WorldLocation, WorldRotation, const_cast<ADungeonCave*>(this) });
    }
    return WorldDoors;
}
