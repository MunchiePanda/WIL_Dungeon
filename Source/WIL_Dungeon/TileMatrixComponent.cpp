#include "TileMatrixComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UTileMatrixComponent::UTileMatrixComponent()
{
    RowsNum = 80;
    ColsNum = 80;
    MinRoomSize = 6;
    MaxRoomSize = 12;
    MaxRandomAttemptsPerRoom = 300;
}

void UTileMatrixComponent::InitializeTileMap(int32 Rows, int32 Cols)
{
    if (Rows <= 0 || Cols <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid tile map dimensions: Rows=%d, Cols=%d"), Rows, Cols);
        return;
    }

    RowsNum = Rows;
    ColsNum = Cols;
    TileMap.Empty();
    TileMap.Init(false, Rows * Cols);
    GeneratedRooms.Empty();
    HubTiles.Empty();

    // Mark hub tiles (10x10, tiles 0x35 to 9x44, centered at 5x40)
    int32 CenterRow = 5;
    int32 CenterCol = 40;
    int32 HalfSize = 5;
    for (int32 R = CenterRow - HalfSize; R <= CenterRow + HalfSize - 1; R++)
    {
        for (int32 C = CenterCol - HalfSize; C <= CenterCol + HalfSize - 1; C++)
        {
            FVector2D Tile(R, C);
            if (IsTileInMap(Tile))
            {
                TileMap[R * ColsNum + C] = true;
                HubTiles.Add(Tile);
            }
        }
    }
}

void UTileMatrixComponent::SetRoomSize(int32 NewMinRoomSize, int32 NewMaxRoomSize)
{
    MinRoomSize = NewMinRoomSize;
    MaxRoomSize = NewMaxRoomSize;
}

bool UTileMatrixComponent::IsTileInMap(const FVector2D& Tile)
{
    return Tile.X >= 0 && Tile.X < RowsNum && Tile.Y >= 0 && Tile.Y < ColsNum;
}

bool UTileMatrixComponent::IsTileOccupied(const FVector2D& Tile)
{
    if (!IsTileInMap(Tile)) return false;
    return TileMap[Tile.X * ColsNum + Tile.Y];
}

bool UTileMatrixComponent::IsTileInHub(const FVector2D& Tile)
{
    return HubTiles.Contains(Tile);
}

bool UTileMatrixComponent::IsTileOccupiedPublic(const FVector2D& Tile)
{
    return IsTileOccupied(Tile);
}

bool UTileMatrixComponent::AreTilesValid(const TArray<FVector2D>& Tiles)
{
    for (const FVector2D& Tile : Tiles)
    {
        if (!IsTileInMap(Tile) || IsTileOccupied(Tile))
            return false;
    }
    return true;
}

void UTileMatrixComponent::OccupyTile(const FVector2D& Tile)
{
    if (IsTileInMap(Tile) && !IsTileInHub(Tile))
    {
        TileMap[Tile.X * ColsNum + Tile.Y] = true;
    }
}

float UTileMatrixComponent::ManhattanDistance(const FVector2D& A, const FVector2D& B)
{
    return FMath::Abs(A.X - B.X) + FMath::Abs(A.Y - B.Y);
}

TArray<FVector2D> UTileMatrixComponent::GetEdgeTiles(const FRoomData& Room)
{
    TArray<FVector2D> EdgeTiles;
    for (const FVector2D& Tile : Room.OccupiedTiles)
    {
        TArray<FVector2D> Neighbors = {
            FVector2D(Tile.X - 1, Tile.Y),
            FVector2D(Tile.X + 1, Tile.Y),
            FVector2D(Tile.X, Tile.Y - 1),
            FVector2D(Tile.X, Tile.Y + 1)
        };
        for (const FVector2D& Neighbor : Neighbors)
        {
            if (IsTileInMap(Neighbor) && !Room.OccupiedTiles.Contains(Neighbor) && !IsTileInHub(Neighbor))
            {
                EdgeTiles.AddUnique(Tile);
            }
        }
    }
    return EdgeTiles;
}

bool UTileMatrixComponent::TryPlaceRoomFromEdge(const FVector2D& EdgeTile, int32 Size, const FRoomData& ParentRoom)
{
    TArray<FVector2D> Directions = {
        FVector2D(-Size, 0), // Up
        FVector2D(Size, 0),  // Down
        FVector2D(0, -Size), // Left
        FVector2D(0, Size)   // Right
    };
    Directions.Sort([](const FVector2D&, const FVector2D&) { return FMath::RandBool(); });
    for (const FVector2D& Dir : Directions)
    {
        TArray<FVector2D> Tiles;
        int32 StartRow = EdgeTile.X + Dir.X;
        int32 StartCol = EdgeTile.Y + Dir.Y;
        for (int32 R = StartRow; R < StartRow + Size; R++)
        {
            for (int32 C = StartCol; C < StartCol + Size; C++)
            {
                Tiles.Add(FVector2D(R, C));
            }
        }
        if (AreTilesValid(Tiles))
        {
            for (const FVector2D& Tile : Tiles)
            {
                OccupyTile(Tile);
            }
            FRoomData NewRoom;
            NewRoom.OccupiedTiles = Tiles;
            GeneratedRooms.Add(NewRoom);
            ConnectRooms(ParentRoom, NewRoom, EdgeTile);
            return true;
        }
    }
    return false;
}

void UTileMatrixComponent::ConnectRooms(const FRoomData& ParentRoom, const FRoomData& NewRoom, const FVector2D& EdgeTile)
{
    FVector2D NewRoomTile = NewRoom.OccupiedTiles[0];
    float MinDist = ManhattanDistance(EdgeTile, NewRoomTile);
    for (const FVector2D& Tile : NewRoom.OccupiedTiles) {
        float Dist = ManhattanDistance(EdgeTile, Tile);
        if (Dist < MinDist) {
            MinDist = Dist;
            NewRoomTile = Tile;
        }
    }
    FVector2D Current = EdgeTile;
    while (Current.X != NewRoomTile.X || Current.Y != NewRoomTile.Y) {
        if (Current.X != NewRoomTile.X) {
            Current.X += Current.X < NewRoomTile.X ? 1 : -1;
        }
        if (Current.Y != NewRoomTile.Y) {
            Current.Y += Current.Y < NewRoomTile.Y ? 1 : -1;
        }
        // Occupy a 3x3 area for wider hallway
        for (int32 dR = -1; dR <= 1; dR++) {
            for (int32 dC = -1; dC <= 1; dC++) {
                FVector2D Neighbor(Current.X + dR, Current.Y + dC);
                if (IsTileInMap(Neighbor) && !IsTileInHub(Neighbor)) {
                    OccupyTile(Neighbor);
                }
            }
        }
    }
}

void UTileMatrixComponent::CreateRooms(int32 RoomCount, const FVector2D& ExitPoint)
{
    // Create a virtual hub room for connection logic
    FRoomData HubRoom;
    HubRoom.OccupiedTiles = HubTiles;
    GeneratedRooms.Add(HubRoom);

    // Generate guaranteed hallway from the specified exit point (upward only)
    int32 Size = FMath::RandRange(MinRoomSize, MaxRoomSize);
    TArray<FVector2D> Tiles;
    int32 StartRow = ExitPoint.X + Size; // Force upward
    int32 StartCol = ExitPoint.Y;
    if (StartRow + Size > RowsNum) {
        Size = FMath::Max(MinRoomSize, RowsNum - ExitPoint.X - 1); // Adjust size to fit within bounds
        StartRow = ExitPoint.X + Size;
    }
    for (int32 R = ExitPoint.X + 1; R < StartRow; R++) { // Start just above ExitPoint
        for (int32 C = StartCol - (Size / 2); C < StartCol + (Size / 2); C++) {
            if (C >= 0 && C < ColsNum) {
                Tiles.Add(FVector2D(R, C));
            }
        }
    }
    if (AreTilesValid(Tiles)) {
        for (const FVector2D& Tile : Tiles) {
            OccupyTile(Tile);
        }
        FRoomData NewRoom;
        NewRoom.OccupiedTiles = Tiles;
        GeneratedRooms.Add(NewRoom);
        ConnectRooms(HubRoom, NewRoom, ExitPoint);
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("Failed to place initial hallway at ExitPoint: %s"), *ExitPoint.ToString());
        // Fallback with minimum size
        Size = MinRoomSize;
        Tiles.Empty();
        StartRow = ExitPoint.X + Size;
        for (int32 R = ExitPoint.X + 1; R < StartRow; R++) {
            for (int32 C = StartCol - (Size / 2); C < StartCol + (Size / 2); C++) {
                if (C >= 0 && C < ColsNum) {
                    Tiles.Add(FVector2D(R, C));
                }
            }
        }
        if (AreTilesValid(Tiles)) {
            for (const FVector2D& Tile : Tiles) {
                OccupyTile(Tile);
            }
            FRoomData NewRoom;
            NewRoom.OccupiedTiles = Tiles;
            GeneratedRooms.Add(NewRoom);
            ConnectRooms(HubRoom, NewRoom, ExitPoint);
        }
        else {
            UE_LOG(LogTemp, Error, TEXT("Failed to place fallback hallway at ExitPoint: %s"), *ExitPoint.ToString());
        }
    }

    // Generate remaining rooms, restricting to above ExitPoint
    for (int32 i = GeneratedRooms.Num() - 1; i < RoomCount; i++) {
        int32 Attempts = 0;
        while (Attempts < MaxRandomAttemptsPerRoom) {
            int32 RoomIndex = FMath::RandRange(0, GeneratedRooms.Num() - 1);
            FRoomData ParentRoom = GeneratedRooms[RoomIndex];
            TArray<FVector2D> EdgeTiles = GetEdgeTiles(ParentRoom);
            if (EdgeTiles.Num() == 0) {
                Attempts++;
                continue;
            }
            FVector2D EdgeTile = EdgeTiles[FMath::RandRange(0, EdgeTiles.Num() - 1)];
            int32 RoomSize = FMath::RandRange(MinRoomSize, MaxRoomSize);
            if (EdgeTile.X <= ExitPoint.X) continue; // Only allow rooms above ExitPoint
            if (TryPlaceRoomFromEdge(EdgeTile, RoomSize, ParentRoom)) {
                break;
            }
            Attempts++;
        }
    }
}

TArray<FVector> UTileMatrixComponent::GetFloorLocations(float TileSize)
{
    TArray<FVector> FloorLocations;
    for (int32 R = 0; R < RowsNum; R++)
    {
        for (int32 C = 0; C < ColsNum; C++)
        {
            if (TileMap[R * ColsNum + C] && !IsTileInHub(FVector2D(R, C)))
            {
                FloorLocations.Add(FVector(C * TileSize, R * TileSize, 0));
            }
        }
    }
    return FloorLocations;
}

TArray<FTransform> UTileMatrixComponent::GetWallSpawnPoints(float TileSize)
{
    TArray<FTransform> WallSpawnPoints;
    for (int32 R = 0; R < RowsNum; R++)
    {
        for (int32 C = 0; C < ColsNum; C++)
        {
            if (!TileMap[R * ColsNum + C] || IsTileInHub(FVector2D(R, C))) continue;
            TArray<FVector2D> Neighbors = {
                FVector2D(R - 1, C), // North
                FVector2D(R + 1, C), // South
                FVector2D(R, C - 1), // West
                FVector2D(R, C + 1)  // East
            };
            for (int32 i = 0; i < Neighbors.Num(); i++)
            {
                const FVector2D& Neighbor = Neighbors[i];
                if (!IsTileInMap(Neighbor) || IsTileOccupied(Neighbor)) continue;
                float X = C * TileSize;
                float Y = R * TileSize;
                float Z = 10.0f;
                FRotator Rotation;
                if (i == 0) {
                    Y -= TileSize * 0.5f;
                    Rotation = FRotator(0, 90, 0);
                }
                else if (i == 1) {
                    Y += TileSize * 0.5f;
                    Rotation = FRotator(0, 90, 0);
                }
                else if (i == 2) {
                    X -= TileSize * 0.5f;
                    Rotation = FRotator(0, 0, 0);
                }
                else {
                    X += TileSize * 0.5f;
                    Rotation = FRotator(0, 0, 0);
                }
                FVector Location(X, Y, Z);
                WallSpawnPoints.Add(FTransform(Rotation, Location));
            }
        }
    }
    return WallSpawnPoints;
}

TArray<FSpawnPointData> UTileMatrixComponent::GetRoomSpawnPoints(float TileSize, bool ExcludeCentralRoom)
{
    TArray<FSpawnPointData> SpawnPoints;
    FVector2D Center(5, 40); // Updated hub center
    for (int32 RoomIndex = ExcludeCentralRoom ? 1 : 0; RoomIndex < GeneratedRooms.Num(); RoomIndex++)
    {
        const FRoomData& Room = GeneratedRooms[RoomIndex];
        float AvgX = 0, AvgY = 0;
        for (const FVector2D& Tile : Room.OccupiedTiles)
        {
            AvgX += Tile.Y;
            AvgY += Tile.X;
        }
        AvgX /= Room.OccupiedTiles.Num();
        AvgY /= Room.OccupiedTiles.Num();
        float WorldX = AvgX * TileSize;
        float WorldY = AvgY * TileSize;
        float Z = 10.0f;
        FVector Location(WorldX, WorldY, Z + 5.0f);
            float Distance = FVector2D::Distance(FVector2D(AvgY, AvgX), Center);
        FSpawnPointData SpawnPoint;
        SpawnPoint.Location = Location;
        SpawnPoint.Distance = Distance;
        SpawnPoints.Add(SpawnPoint);
    }
    return SpawnPoints;
}

TArray<FVector> UTileMatrixComponent::GetRoomSpawnLocations(float TileSize, bool ExcludeCentralRoom)
{
    TArray<FSpawnPointData> SpawnPoints = GetRoomSpawnPoints(TileSize, ExcludeCentralRoom);
    TArray<FVector> Locations;
    for (const FSpawnPointData& Point : SpawnPoints)
    {
        Locations.Add(Point.Location);
    }
    return Locations;
}

TArray<float> UTileMatrixComponent::GetRoomSpawnDistances(float TileSize, bool ExcludeCentralRoom)
{
    TArray<FSpawnPointData> SpawnPoints = GetRoomSpawnPoints(TileSize, ExcludeCentralRoom);
    TArray<float> Distances;
    for (const FSpawnPointData& Point : SpawnPoints)
    {
        Distances.Add(Point.Distance);
    }
    return Distances;
}

TArray<FVector> UTileMatrixComponent::GetCorridorSpawnPoints(float TileSize)
{
    TArray<FVector> SpawnPoints;
    for (int32 R = 0; R < RowsNum; R++)
    {
        for (int32 C = 0; C < ColsNum; C++)
        {
            if (!TileMap[R * ColsNum + C] || IsTileInHub(FVector2D(R, C))) continue;
            FVector2D Tile(R, C);
            bool IsInRoom = false;
            for (const FRoomData& Room : GeneratedRooms)
            {
                if (Room.OccupiedTiles.Contains(Tile))
                {
                    IsInRoom = true;
                    break;
                }
            }
            if (!IsInRoom)
            {
                float WorldX = C * TileSize;
                float WorldY = R * TileSize;
                float Z = 10.0f;
                SpawnPoints.Add(FVector(WorldX, WorldY, Z));
            }
        }
    }
    return SpawnPoints;
}

void UTileMatrixComponent::PrintDebugTileMap()
{
    FString Output;
    for (int32 R = 0; R < RowsNum; R++)
    {
        for (int32 C = 0; C < ColsNum; C++)
        {
            FVector2D Tile(R, C);
            if (IsTileInHub(Tile))
                Output += TEXT("H");
            else
                Output += TileMap[R * ColsNum + C] ? TEXT("█") : TEXT(".");
        }
        Output += TEXT("\n");
    }
    UE_LOG(LogTemp, Log, TEXT("%s"), *Output);
}