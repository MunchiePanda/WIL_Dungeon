#include "D_Generator.h"
#include "TileMatrixComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"

AD_Generator::AD_Generator()
{
    PrimaryActorTick.bCanEverTick = false;
    TileMatrixComponent = CreateDefaultSubobject<UTileMatrixComponent>(TEXT("TileMatrixComponent"));
    LoadingIndicator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LoadingIndicator"));
    LoadingIndicator->SetupAttachment(RootComponent);
    LoadingIndicator->SetVisibility(false); // Hidden by default
    Rows = 80;
    Cols = 80;
    MinRoomSize = 10; // Updated from 6
    MaxRoomSize = 16; // Updated from 12
    RoomCount = 40;
    TileSize = 100.0f;
    MaxTechScrapPerRoom = 3;
    ActiveExitPoint = FVector2D(-1, -1);
    bIsGenerating = false;
}

void AD_Generator::GenerateDungeon()
{
    if (!TileMatrixComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("TileMatrixComponent is null in GenerateDungeon"));
        return;
    }

    ClearDungeon();
    if (Rows < 20 || Cols < 20)
    {
        UE_LOG(LogTemp, Warning, TEXT("Grid must be at least 20x20 for hub"));
        return;
    }

    TileMatrixComponent->InitializeTileMap(Rows, Cols);
    TileMatrixComponent->SetRoomSize(MinRoomSize, MaxRoomSize);
    SpawnCentralRoomObjects();
    TileMatrixComponent->PrintDebugTileMap();
}

void AD_Generator::GenerateDungeonFromDoor(const FVector2D& ExitPoint)
{
    if (!TileMatrixComponent || bIsGenerating)
    {
        UE_LOG(LogTemp, Warning, TEXT("Generation already in progress or TileMatrixComponent is null"));
        return;
    }

    bIsGenerating = true;
    ActiveExitPoint = ExitPoint;
    LoadingIndicator->SetVisibility(true); // Show loading indicator
    BeginAsyncGeneration(ExitPoint);
}

void AD_Generator::BeginAsyncGeneration(const FVector2D& ExitPoint)
{
    // Simulate async generation with a 2-second delay
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this, ExitPoint]()
        {
            ClearDungeon();
            if (Rows < 20 || Cols < 20)
            {
                UE_LOG(LogTemp, Warning, TEXT("Grid must be at least 20x20 for hub"));
                bIsGenerating = false;
                LoadingIndicator->SetVisibility(false);
                return;
            }

            TileMatrixComponent->InitializeTileMap(Rows, Cols);
            TileMatrixComponent->SetRoomSize(MinRoomSize, MaxRoomSize);
            TileMatrixComponent->CreateRooms(RoomCount, ExitPoint);
            TileMatrixComponent->PrintDebugTileMap();

            TArray<FVector> FloorLocations = TileMatrixComponent->GetFloorLocations(TileSize);
            TArray<FTransform> WallSpawnPoints = TileMatrixComponent->GetWallSpawnPoints(TileSize);

            // Spawn floors and roofs
            for (const FVector& Location : FloorLocations)
            {
                UStaticMeshComponent* FloorComp = NewObject<UStaticMeshComponent>(this);
                FloorComp->RegisterComponent();
                FloorComp->SetStaticMesh(FloorMesh);
                if (FloorMaterial) FloorComp->SetMaterial(0, FloorMaterial);
                FloorComp->SetWorldLocation(Location);
                FloorComp->SetWorldScale3D(FVector(1.0f, 1.0f, 0.1f));
                SpawnedMeshes.Add(FloorComp);

                if (RoofMesh)
                {
                    UStaticMeshComponent* RoofComp = NewObject<UStaticMeshComponent>(this);
                    RoofComp->RegisterComponent();
                    RoofComp->SetStaticMesh(RoofMesh);
                    if (RoofMaterial) RoofComp->SetMaterial(0, RoofMaterial);
                    RoofComp->SetWorldLocation(Location + FVector(0, 0, 300.0f));
                    RoofComp->SetWorldScale3D(FVector(1.0f, 1.0f, 0.1f));
                    SpawnedMeshes.Add(RoofComp);
                }
            }

            // Spawn walls
            for (const FTransform& WallTransform : WallSpawnPoints)
            {
                UStaticMeshComponent* WallComp = NewObject<UStaticMeshComponent>(this);
                WallComp->RegisterComponent();
                WallComp->SetStaticMesh(WallMesh);
                if (WallMaterial) WallComp->SetMaterial(0, WallMaterial);
                WallComp->SetWorldTransform(WallTransform);
                WallComp->SetWorldScale3D(FVector(1.0f, 1.0f, 3.0f));
                SpawnedMeshes.Add(WallComp);
            }

            SpawnCentralRoomObjects();
            SpawnDungeonObjects();
            SpawnExitPortal();

            bIsGenerating = false;
            LoadingIndicator->SetVisibility(false); // Hide loading indicator
            OnGenerationComplete.Broadcast(); // Notify completion
        }), 2.0f, false); // 2-second delay for demo; adjust as needed
}

void AD_Generator::ClearDungeon()
{
    for (UStaticMeshComponent* Mesh : SpawnedMeshes)
    {
        if (Mesh) Mesh->DestroyComponent();
    }
    SpawnedMeshes.Empty();

    for (UStaticMeshComponent* Obj : DungeonObjects)
    {
        if (Obj) Obj->DestroyComponent();
    }
    DungeonObjects.Empty();

    for (UStaticMeshComponent* Obj : CentralRoomObjects)
    {
        if (Obj) Obj->DestroyComponent();
    }
    CentralRoomObjects.Empty();

    ActiveExitPoint = FVector2D(-1, -1);
    bIsGenerating = false;
    if (LoadingIndicator) LoadingIndicator->SetVisibility(false);
}

void AD_Generator::SpawnCentralRoomObjects()
{
    float CenterX = 40 * TileSize; // Center at tile (5, 40)
    float CenterY = 5 * TileSize;
    float Z = 10.0f;

    // Clear existing central room objects
    for (UStaticMeshComponent* Obj : CentralRoomObjects)
    {
        if (Obj) Obj->DestroyComponent();
    }
    CentralRoomObjects.Empty();

    if (TreasureChestMesh)
    {
        UStaticMeshComponent* ChestComp = NewObject<UStaticMeshComponent>(this);
        ChestComp->RegisterComponent();
        ChestComp->SetStaticMesh(TreasureChestMesh);
        if (TreasureChestMaterial) ChestComp->SetMaterial(0, TreasureChestMaterial);
        ChestComp->SetWorldLocation(FVector(CenterX, CenterY, Z));
        CentralRoomObjects.Add(ChestComp);
    }

    if (TorchMesh)
    {
        TArray<FVector> TorchOffsets = {
            FVector(-450, -450, 0),
            FVector(-450,  450, 0),
            FVector(450, -450, 0),
            FVector(450,  450, 0)
        };

        for (const FVector& Offset : TorchOffsets)
        {
            UStaticMeshComponent* TorchComp = NewObject<UStaticMeshComponent>(this);
            TorchComp->RegisterComponent();
            TorchComp->SetStaticMesh(TorchMesh);
            if (TorchMaterial) TorchComp->SetMaterial(0, TorchMaterial);
            FVector Location = FVector(CenterX, CenterY, Z) + Offset;
            TorchComp->SetWorldLocation(Location);
            CentralRoomObjects.Add(TorchComp);
        }
    }
}

void AD_Generator::SpawnDungeonObjects()
{
    FVector Center(40 * TileSize, 5 * TileSize, 10.0f); // Updated hub center
    float MaxDistance = FVector((Cols / 2) * TileSize, (Rows / 2) * TileSize, 0).Size();

    if (EnemyMesh || TechScrapMesh)
    {
        TArray<FSpawnPointData> RoomSpawnPoints = TileMatrixComponent->GetRoomSpawnPoints(TileSize, true);
        for (const FSpawnPointData& SpawnPoint : RoomSpawnPoints)
        {
            FVector Location = SpawnPoint.Location;
            float Distance = SpawnPoint.Distance;
            float DistanceRatio = Distance / MaxDistance;

            if (EnemyMesh && FMath::RandRange(0.0f, 1.0f) < DistanceRatio)
            {
                UStaticMeshComponent* EnemyComp = NewObject<UStaticMeshComponent>(this);
                EnemyComp->RegisterComponent();
                EnemyComp->SetStaticMesh(EnemyMesh);
                if (EnemyMaterial) EnemyComp->SetMaterial(0, EnemyMaterial);
                EnemyComp->SetWorldLocation(Location);
                EnemyComp->SetWorldScale3D(FVector(1.0f + DistanceRatio, 1.0f + DistanceRatio, 1.0f));
                DungeonObjects.Add(EnemyComp);
            }

            if (TechScrapMesh)
            {
                int32 NumScrap = FMath::RandRange(1, FMath::CeilToInt(MaxTechScrapPerRoom * DistanceRatio));
                for (int32 i = 0; i < NumScrap; i++)
                {
                    float OffsetX = FMath::RandRange(-300.0f, 300.0f);
                    float OffsetY = FMath::RandRange(-300.0f, 300.0f);
                    FVector ScrapLocation = Location + FVector(OffsetX, OffsetY, 0);
                    UStaticMeshComponent* ScrapComp = NewObject<UStaticMeshComponent>(this);
                    ScrapComp->RegisterComponent();
                    ScrapComp->SetStaticMesh(TechScrapMesh);
                    if (TechScrapMaterial) ScrapComp->SetMaterial(0, TechScrapMaterial);
                    ScrapComp->SetWorldLocation(ScrapLocation);
                    DungeonObjects.Add(ScrapComp);
                }
            }

            if (CarvingMesh && DistanceRatio > 0.7f && FMath::RandBool())
            {
                UStaticMeshComponent* CarvingComp = NewObject<UStaticMeshComponent>(this);
                CarvingComp->RegisterComponent();
                CarvingComp->SetStaticMesh(CarvingMesh);
                if (CarvingMaterial) CarvingComp->SetMaterial(0, CarvingMaterial);
                CarvingComp->SetWorldLocation(Location + FVector(0, 0, 50.0f));
                DungeonObjects.Add(CarvingComp);
            }
        }
    }

    if (HealthPickupMesh)
    {
        TArray<FVector> CorridorSpawnPoints = TileMatrixComponent->GetCorridorSpawnPoints(TileSize);
        for (const FVector& Location : CorridorSpawnPoints)
        {
            if (FMath::RandBool())
            {
                UStaticMeshComponent* PickupComp = NewObject<UStaticMeshComponent>(this);
                PickupComp->RegisterComponent();
                PickupComp->SetStaticMesh(HealthPickupMesh);
                if (HealthPickupMaterial) PickupComp->SetMaterial(0, HealthPickupMaterial);
                PickupComp->SetWorldLocation(Location);
                DungeonObjects.Add(PickupComp);
            }
        }
    }
}

void AD_Generator::SpawnExitPortal()
{
    if (!ExitPortalMesh) return;

    TArray<FVector> BorderTiles;
    for (int32 R = 0; R < Rows; R++)
    {
        for (int32 C = 0; C < Cols; C++)
        {
            FVector2D Tile(R, C);
            if (!TileMatrixComponent->IsTileOccupiedPublic(Tile)) continue;
            if (R == 0 || R == Rows - 1 || C == 0 || C == Cols - 1)
            {
                BorderTiles.Add(FVector(C * TileSize, R * TileSize, 10.0f));
            }
        }
    }

    if (BorderTiles.Num() > 0)
    {
        FVector PortalLocation = BorderTiles[FMath::RandRange(0, BorderTiles.Num() - 1)];
        UStaticMeshComponent* PortalComp = NewObject<UStaticMeshComponent>(this);
        PortalComp->RegisterComponent();
        PortalComp->SetStaticMesh(ExitPortalMesh);
        if (ExitPortalMaterial) PortalComp->SetMaterial(0, ExitPortalMaterial);
        PortalComp->SetWorldLocation(PortalLocation + FVector(0, 0, 50.0f));
        DungeonObjects.Add(PortalComp);
    }
}