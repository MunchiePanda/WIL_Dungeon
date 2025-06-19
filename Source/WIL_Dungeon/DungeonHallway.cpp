#include "DungeonHallway.h"

ADungeonHallway::ADungeonHallway()
{
    if (MeshComponent)
    {
        static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
        if (MeshAsset.Succeeded())
        {
            MeshComponent->SetStaticMesh(MeshAsset.Object);
            MeshComponent->SetWorldScale3D(FVector(2.f, 10.f, 1.f));
        }
    }
    Doors.Add(FDoorInfo{ FVector(0.f, 500.f, 0.f), FRotator(0.f, 0.f, 0.f), nullptr });   // Forward (north)
    Doors.Add(FDoorInfo{ FVector(0.f, -500.f, 0.f), FRotator(0.f, 180.f, 0.f), nullptr }); // Backward (south)
}

TArray<FDoorInfo> ADungeonHallway::GetDoors() const
{
    TArray<FDoorInfo> WorldDoors;
    for (const FDoorInfo& Door : Doors)
    {
        FVector WorldLocation = GetActorLocation() + GetActorRotation().RotateVector(Door.Location);
        FRotator WorldRotation = GetActorRotation() + Door.Rotation;
        WorldDoors.Add(FDoorInfo{ WorldLocation, WorldRotation, const_cast<ADungeonHallway*>(this) });
    }
    return WorldDoors;
}