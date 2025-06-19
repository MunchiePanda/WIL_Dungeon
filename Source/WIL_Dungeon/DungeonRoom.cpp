#include "DungeonRoom.h"

ADungeonRoom::ADungeonRoom()
{
    if (MeshComponent)
    {
        static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
        if (MeshAsset.Succeeded())
        {
            MeshComponent->SetStaticMesh(MeshAsset.Object);
            MeshComponent->SetWorldScale3D(FVector(10.f, 10.f, 1.f));
        }
    }
    // North (Y+), East (X+), South (Y-), West (X-)
    Doors.Add(FDoorInfo{ FVector(0.f, 500.f, 0.f), FRotator(0.f, 0.f, 0.f), nullptr });   // North, face north
    Doors.Add(FDoorInfo{ FVector(500.f, 0.f, 0.f), FRotator(0.f, 90.f, 0.f), nullptr });  // East, face east
    Doors.Add(FDoorInfo{ FVector(0.f, -500.f, 0.f), FRotator(0.f, 180.f, 0.f), nullptr }); // South, face south
    Doors.Add(FDoorInfo{ FVector(-500.f, 0.f, 0.f), FRotator(0.f, 270.f, 0.f), nullptr }); // West, face west
}

TArray<FDoorInfo> ADungeonRoom::GetDoors() const
{
    TArray<FDoorInfo> WorldDoors;
    for (const FDoorInfo& Door : Doors)
    {
        FVector WorldLocation = GetActorLocation() + GetActorRotation().RotateVector(Door.Location);
        FRotator WorldRotation = GetActorRotation() + Door.Rotation;
        WorldDoors.Add(FDoorInfo{ WorldLocation, WorldRotation, const_cast<ADungeonRoom*>(this) });
    }
    return WorldDoors;
}