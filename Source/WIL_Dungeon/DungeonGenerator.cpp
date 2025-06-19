// DungeonGenerator.cpp
#include "DungeonGenerator.h"
#include "GameFramework/Actor.h"
#include "DungeonArea.h"
#include "DungeonRoom.h"
#include "DungeonHallway.h"
#include "DungeonCave.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMesh.h"

// Sets default values
ADungeonGenerator::ADungeonGenerator()
{
    // Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADungeonGenerator::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Log, TEXT("DungeonGenerator BeginPlay started"));

    if (CentralRoomClass)
    {
        UE_LOG(LogTemp, Log, TEXT("Spawning central room"));
        ADungeonArea* CentralRoom = GetWorld()->SpawnActor<ADungeonArea>(CentralRoomClass, FVector::ZeroVector, FRotator::ZeroRotator);
        if (CentralRoom)
        {
            FAreaInfo CentralArea{ CentralRoom, 0, EAreaType::Room };
            GeneratedAreas.Add(CentralArea);
            TArray<FDoorInfo> CentralDoors = CentralRoom->GetDoors();
            UE_LOG(LogTemp, Log, TEXT("Central room spawned with %d doors"), CentralDoors.Num());
            for (const FDoorInfo& Door : CentralDoors)
            {
                UE_LOG(LogTemp, Log, TEXT("Central door at %s, rotation %s"), *Door.Location.ToString(), *Door.Rotation.ToString());
            }
            ActiveDoors.Append(CentralDoors);
            GenerateDungeon();
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to spawn central room"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("CentralRoomClass is not set"));
    }
}

void ADungeonGenerator::GenerateDungeon()
{
    while (!ActiveDoors.IsEmpty() && GeneratedAreas.Num() < MaxAreas)
    {
        // Sort ActiveDoors by depth (deeper first)
        ActiveDoors.Sort([](const FDoorInfo& A, const FDoorInfo& B) {
            int32 DepthA = 0, DepthB = 0;
            ADungeonArea* ParentA = Cast<ADungeonArea>(A.ParentArea);
            ADungeonArea* ParentB = Cast<ADungeonArea>(B.ParentArea);
            if (ParentA)
            {
                ADungeonGenerator* Generator = Cast<ADungeonGenerator>(ParentA->GetOwner());
                if (Generator)
                {
                    for (const FAreaInfo& Area : Generator->GeneratedAreas)
                    {
                        if (Area.AreaActor == ParentA) DepthA = Area.Depth;
                    }
                }
            }
            if (ParentB)
            {
                ADungeonGenerator* Generator = Cast<ADungeonGenerator>(ParentB->GetOwner());
                if (Generator)
                {
                    for (const FAreaInfo& Area : Generator->GeneratedAreas)
                    {
                        if (Area.AreaActor == ParentB) DepthB = Area.Depth;
                    }
                }
            }
            return DepthA > DepthB; // Prioritize deeper doors
            });

        FDoorInfo CurrentDoor = ActiveDoors[0];
        ActiveDoors.RemoveAt(0);

        int32 CurrentDepth = 0;
        ADungeonArea* ParentArea = Cast<ADungeonArea>(CurrentDoor.ParentArea);
        if (ParentArea)
        {
            for (const FAreaInfo& Area : GeneratedAreas)
            {
                if (Area.AreaActor == ParentArea)
                {
                    CurrentDepth = Area.Depth;
                    break;
                }
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Door has no valid ParentArea!"));
            continue;
        }

        if (CurrentDepth + 1 <= MaxDepth)
        {
            float Rand = FMath::RandRange(0.f, 1.f);
            EAreaType ChosenType = (Rand < 0.5f) ? EAreaType::Room : EAreaType::Hallway;

            // Log the parent door
            UE_LOG(LogTemp, Log, TEXT("Parent door at %s, rotation %s"), *CurrentDoor.Location.ToString(), *CurrentDoor.Rotation.ToString());

            // Calculate offset based on door positions
            float ParentDoorOffset = 500.f; // Default for room
            if (ParentArea->IsA<ADungeonHallway>())
            {
                FVector Direction = CurrentDoor.Rotation.Vector();
                ParentDoorOffset = (FMath::Abs(Direction.Y) > 0.5f) ? 500.f : 100.f;
            }

            // Spawn the new area temporarily to determine its type
            FVector TempLocation = CurrentDoor.Location + CurrentDoor.Rotation.Vector() * 1000.f;
            FRotator SpawnRotation = CurrentDoor.Rotation;
            ADungeonArea* NewArea = SpawnArea(ChosenType, TempLocation, SpawnRotation);
            if (!NewArea) continue;

            // Adjust the new area's rotation to align its door with the parent's door
            FRotator NewRotation = CurrentDoor.Rotation + FRotator(0.f, 180.f, 0.f); // Opposite direction
            NewArea->SetActorRotation(NewRotation);

            float NewDoorOffset = 500.f; // Default for room
            if (ChosenType == EAreaType::Hallway)
            {
                FVector Direction = CurrentDoor.Rotation.Vector();
                NewDoorOffset = (FMath::Abs(Direction.Y) > 0.5f) ? 500.f : 100.f;
            }

            // Calculate the offset
            float Offset = ParentDoorOffset + NewDoorOffset;
            UE_LOG(LogTemp, Log, TEXT("Offset: ParentDoorOffset=%f, NewDoorOffset=%f, Total=%f"), ParentDoorOffset, NewDoorOffset, Offset);

            // Final spawn location
            FVector Direction = CurrentDoor.Rotation.Vector();
            FVector SpawnLocation = CurrentDoor.Location + Direction * Offset;

            // Check if the spawn location is already occupied
            bool bOccupied = false;
            for (const FAreaInfo& ExistingArea : GeneratedAreas)
            {
                if (!ExistingArea.AreaActor) continue; // Skip invalid entries
                FVector ExistingLocation = ExistingArea.AreaActor->GetActorLocation();
                if (FVector::DistSquared(SpawnLocation, ExistingLocation) < 500.f * 500.f) // 5m threshold
                {
                    bOccupied = true;
                    UE_LOG(LogTemp, Warning, TEXT("Potential spawn location %s too close to existing area at %s"), *SpawnLocation.ToString(), *ExistingLocation.ToString());
                    break;
                }
            }

            if (bOccupied)
            {
                NewArea->Destroy();
                continue;
            }

            // Move the new area to the correct location
            NewArea->SetActorLocationAndRotation(SpawnLocation, NewRotation);

            if (!CheckOverlap(SpawnLocation, NewRotation, NewArea))
            {
                FAreaInfo NewAreaInfo{ NewArea, CurrentDepth + 1, ChosenType };
                GeneratedAreas.Add(NewAreaInfo);
                TArray<FDoorInfo> NewDoors = NewArea->GetDoors();
                for (FDoorInfo& Door : NewDoors)
                {
                    Door.ParentArea = NewArea;

                    // Check if this door leads to an occupied space
                    FVector DoorDirection = Door.Rotation.Vector();
                    float DoorOffset = (ChosenType == EAreaType::Hallway) ? 600.f : 1000.f;
                    FVector PotentialSpawn = Door.Location + DoorDirection * DoorOffset;
                    bool bDoorOccupied = false;
                    for (const FAreaInfo& ExistingArea : GeneratedAreas)
                    {
                        if (!ExistingArea.AreaActor) continue;
                        FVector ExistingLocation = ExistingArea.AreaActor->GetActorLocation();
                        if (FVector::DistSquared(PotentialSpawn, ExistingLocation) < 500.f * 500.f)
                        {
                            bDoorOccupied = true;
                            break;
                        }
                    }
                    if (!bDoorOccupied)
                    {
                        ActiveDoors.Add(Door);
                    }
                }
                UE_LOG(LogTemp, Log, TEXT("Added area at depth %d, location %s, rotation %s"), CurrentDepth + 1, *SpawnLocation.ToString(), *NewRotation.ToString());
            }
            else
            {
                NewArea->Destroy();
            }
        }
    }

    // Clean up GeneratedAreas by removing invalid entries
    GeneratedAreas.RemoveAll([](const FAreaInfo& Area) {
        if (!Area.AreaActor)
        {
            UE_LOG(LogTemp, Warning, TEXT("Removing invalid area with null AreaActor"));
            return true;
        }
        return false;
        });

    PlaceExit();
}

void ADungeonGenerator::PlaceExit()
{
    UE_LOG(LogTemp, Log, TEXT("PlaceExit started with %d generated areas"), GeneratedAreas.Num());

    // Log the state of GeneratedAreas for debugging
    for (const FAreaInfo& Area : GeneratedAreas)
    {
        UE_LOG(LogTemp, Log, TEXT("Area at depth %d, type %d, actor %s"), Area.Depth, (uint8)Area.Type, Area.AreaActor ? *Area.AreaActor->GetName() : TEXT("nullptr"));
    }

    TArray<FAreaInfo> OuterAreas;
    for (const FAreaInfo& Area : GeneratedAreas)
    {
        if (!Area.AreaActor)
        {
            UE_LOG(LogTemp, Warning, TEXT("Skipping invalid area with null AreaActor at depth %d"), Area.Depth);
            continue;
        }
        if (Area.Depth == MaxDepth)
        {
            OuterAreas.Add(Area);
        }
    }

    if (OuterAreas.IsEmpty() && !GeneratedAreas.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("No areas at MaxDepth, using deepest area"));
        int32 MaxDepthFound = 0;
        for (const FAreaInfo& Area : GeneratedAreas)
        {
            if (!Area.AreaActor) continue;
            if (Area.Depth > MaxDepthFound) MaxDepthFound = Area.Depth;
        }
        for (const FAreaInfo& Area : GeneratedAreas)
        {
            if (!Area.AreaActor) continue;
            if (Area.Depth == MaxDepthFound)
            {
                OuterAreas.Add(Area);
            }
        }
    }

    if (!OuterAreas.IsEmpty())
    {
        FAreaInfo ExitArea = OuterAreas[FMath::RandRange(0, OuterAreas.Num() - 1)];
        if (!ExitArea.AreaActor)
        {
            UE_LOG(LogTemp, Error, TEXT("Selected exit area has null AreaActor at depth %d"), ExitArea.Depth);
            return;
        }

        TArray<FDoorInfo> ExitDoors = ExitArea.AreaActor->GetDoors();
        if (!ExitDoors.IsEmpty())
        {
            FDoorInfo ExitDoor = ExitDoors[FMath::RandRange(0, ExitDoors.Num() - 1)];
            UE_LOG(LogTemp, Log, TEXT("Spawning exit at %s"), *ExitDoor.Location.ToString());
            if (ExitClass)
            {
                GetWorld()->SpawnActor<ADungeonArea>(ExitClass, ExitDoor.Location, ExitDoor.Rotation);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("ExitClass is not set, cannot spawn exit"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No doors available for exit area at %s"), *ExitArea.AreaActor->GetActorLocation().ToString());
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No valid areas to place exit"));
    }
}

bool ADungeonGenerator::CheckOverlap(const FVector& Location, const FRotator& Rotation, ADungeonArea* NewArea)
{
    if (!NewArea) return false;

    // Check for near-exact location matches
    for (const FAreaInfo& ExistingArea : GeneratedAreas)
    {
        FVector ExistingLocation = ExistingArea.AreaActor->GetActorLocation();
        if (FVector::DistSquared(Location, ExistingLocation) < 500.f * 500.f) // 5m threshold
        {
            UE_LOG(LogTemp, Warning, TEXT("Location %s too close to existing area at %s"), *Location.ToString(), *ExistingLocation.ToString());
            return true;
        }
    }

    // Use bounds for collision
    FVector Origin, BoxExtent;
    NewArea->GetActorBounds(false, Origin, BoxExtent);
    UE_LOG(LogTemp, Log, TEXT("Area bounds at %s: %s"), *Location.ToString(), *BoxExtent.ToString());
    BoxExtent *= 0.8f; // Tighter bounds to catch overlaps
    TArray<FHitResult> HitResults;
    FCollisionShape Box = FCollisionShape::MakeBox(BoxExtent);
    bool bHit = GetWorld()->SweepMultiByChannel(HitResults, Location, Location, Rotation.Quaternion(), ECC_Visibility, Box);

    for (const FHitResult& Hit : HitResults)
    {
        if (Hit.GetActor() && Hit.GetActor() != NewArea && GeneratedAreas.ContainsByPredicate([&](const FAreaInfo& Area) { return Area.AreaActor == Hit.GetActor(); }))
        {
            UE_LOG(LogTemp, Warning, TEXT("Overlap with %s detected at %s"), *Hit.GetActor()->GetName(), *Location.ToString());
            return true;
        }
    }

    UE_LOG(LogTemp, Log, TEXT("No overlap at %s"), *Location.ToString());
    return false;
}

ADungeonArea* ADungeonGenerator::SpawnArea(EAreaType Type, const FVector& Location, const FRotator& Rotation)
{
    TSubclassOf<ADungeonArea> ChosenClass;
    switch (Type)
    {
    case EAreaType::Room:
        if (!RoomClasses.IsEmpty())
        {
            ChosenClass = RoomClasses[FMath::RandRange(0, RoomClasses.Num() - 1)];
            UE_LOG(LogTemp, Log, TEXT("Selected Room class"));
        }
        break;
    case EAreaType::Hallway:
        if (!HallwayClasses.IsEmpty())
        {
            ChosenClass = HallwayClasses[FMath::RandRange(0, HallwayClasses.Num() - 1)];
            UE_LOG(LogTemp, Log, TEXT("Selected Hallway class"));
        }
        break;
   
    }
    if (ChosenClass)
    {
        UE_LOG(LogTemp, Log, TEXT("Spawning area at %s"), *Location.ToString());
        return GetWorld()->SpawnActor<ADungeonArea>(ChosenClass, Location, Rotation);
    }
    UE_LOG(LogTemp, Warning, TEXT("No valid class selected for type %d"), (uint8)Type);
    return nullptr;
}