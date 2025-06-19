#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TileMatrixComponent.generated.h"

USTRUCT()
struct FRoomData
{
    GENERATED_BODY()
    TArray<FVector2D> OccupiedTiles;
};

USTRUCT()
struct FSpawnPointData
{
    GENERATED_BODY()
    FVector Location;
    float Distance;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WIL_DUNGEON_API UTileMatrixComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UTileMatrixComponent();

    UFUNCTION(BlueprintCallable, Category = "Dungeon")
    void InitializeTileMap(int32 Rows, int32 Cols);

    UFUNCTION(BlueprintCallable, Category = "Dungeon")
    void SetRoomSize(int32 NewMinRoomSize, int32 NewMaxRoomSize);

    UFUNCTION(BlueprintCallable, Category = "Dungeon")
    void CreateRooms(int32 RoomCount, const FVector2D& ExitPoint);

    UFUNCTION(BlueprintCallable, Category = "Dungeon")
    TArray<FVector> GetFloorLocations(float TileSize);

    UFUNCTION(BlueprintCallable, Category = "Dungeon")
    TArray<FTransform> GetWallSpawnPoints(float TileSize);

    UFUNCTION(BlueprintCallable, Category = "Dungeon")
    void PrintDebugTileMap();

    TArray<FSpawnPointData> GetRoomSpawnPoints(float TileSize, bool ExcludeCentralRoom);

    UFUNCTION(BlueprintCallable, Category = "Dungeon")
    TArray<FVector> GetRoomSpawnLocations(float TileSize, bool ExcludeCentralRoom);

    UFUNCTION(BlueprintCallable, Category = "Dungeon")
    TArray<float> GetRoomSpawnDistances(float TileSize, bool ExcludeCentralRoom);

    UFUNCTION(BlueprintCallable, Category = "Dungeon")
    TArray<FVector> GetCorridorSpawnPoints(float TileSize);

    UFUNCTION(BlueprintCallable, Category = "Dungeon")
    bool IsTileOccupiedPublic(const FVector2D& Tile);

protected:
    UPROPERTY(VisibleAnywhere, Category = "Dungeon")
    int32 RowsNum;

    UPROPERTY(VisibleAnywhere, Category = "Dungeon")
    int32 ColsNum;

    UPROPERTY(VisibleAnywhere, Category = "Dungeon")
    int32 MinRoomSize;

    UPROPERTY(VisibleAnywhere, Category = "Dungeon")
    int32 MaxRoomSize;

    UPROPERTY(VisibleAnywhere, Category = "Dungeon")
    int32 MaxRandomAttemptsPerRoom;

    UPROPERTY(VisibleAnywhere, Category = "Dungeon")
    TArray<bool> TileMap;

    UPROPERTY(VisibleAnywhere, Category = "Dungeon")
    TArray<FRoomData> GeneratedRooms;

    UPROPERTY(VisibleAnywhere, Category = "Dungeon")
    TArray<FVector2D> HubTiles;

    bool IsTileInMap(const FVector2D& Tile);
    bool IsTileOccupied(const FVector2D& Tile);
    bool IsTileInHub(const FVector2D& Tile);
    bool AreTilesValid(const TArray<FVector2D>& Tiles);
    void OccupyTile(const FVector2D& Tile);
    float ManhattanDistance(const FVector2D& A, const FVector2D& B);
    TArray<FVector2D> GetEdgeTiles(const FRoomData& Room);
    bool TryPlaceRoomFromEdge(const FVector2D& EdgeTile, int32 Size, const FRoomData& ParentRoom);
    void ConnectRooms(const FRoomData& ParentRoom, const FRoomData& NewRoom, const FVector2D& EdgeTile);
};
