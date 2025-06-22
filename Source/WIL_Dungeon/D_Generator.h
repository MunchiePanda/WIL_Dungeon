#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileMatrixComponent.h"
#include "D_Generator.generated.h"

// Declare the delegate at the top, outside the UCLASS
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGenerationComplete);

UCLASS()
class WIL_DUNGEON_API AD_Generator : public AActor
{
    GENERATED_BODY()
public:
    AD_Generator();
    UFUNCTION(BlueprintCallable)
    void GenerateDungeon();
    UFUNCTION(BlueprintCallable)
    void GenerateDungeonFromDoor(const FVector2D& ExitPoint);
    UFUNCTION(BlueprintCallable)
    void ClearDungeon();
    UFUNCTION(BlueprintCallable, Category = "Dungeon")
    void SpawnEnemyAtCenter();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon")
    int32 Rows;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon")
    int32 Cols;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon")
    int32 MinRoomSize;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon")
    int32 MaxRoomSize;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon")
    int32 RoomCount;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon")
    float TileSize;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon")
    UStaticMesh* FloorMesh;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon")
    UStaticMesh* WallMesh;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon")
    UStaticMesh* RoofMesh;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon")
    UMaterialInterface* FloorMaterial;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon")
    UMaterialInterface* WallMaterial;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon")
    UMaterialInterface* RoofMaterial;

    // Central room objects
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Central Room Objects")
    UStaticMesh* TreasureChestMesh;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Central Room Objects")
    UMaterialInterface* TreasureChestMaterial;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Central Room Objects")
    UStaticMesh* TorchMesh;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Central Room Objects")
    UMaterialInterface* TorchMaterial;

    // Dungeon objects
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon Objects")
    UStaticMesh* EnemyMesh;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon Objects")
    UMaterialInterface* EnemyMaterial;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon Objects")
    UStaticMesh* HealthPickupMesh;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon Objects")
    UMaterialInterface* HealthPickupMaterial;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon Objects")
    UStaticMesh* TechScrapMesh;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon Objects")
    UMaterialInterface* TechScrapMaterial;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon Objects")
    UStaticMesh* CarvingMesh;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon Objects")
    UMaterialInterface* CarvingMaterial;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon Objects")
    UStaticMesh* ExitPortalMesh;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon Objects")
    UMaterialInterface* ExitPortalMaterial;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon Objects")
    int32 MaxTechScrapPerRoom;

    // Enemy class to spawn after generation
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
    TSubclassOf<AActor> EnemyToSpawn;

protected:
    UPROPERTY(VisibleAnywhere)
    UTileMatrixComponent* TileMatrixComponent;
    TArray<UStaticMeshComponent*> SpawnedMeshes;
    TArray<UStaticMeshComponent*> CentralRoomObjects;
    TArray<UStaticMeshComponent*> DungeonObjects;
    UPROPERTY(VisibleAnywhere, Category = "Dungeon")
    FVector2D ActiveExitPoint;

    UPROPERTY(VisibleAnywhere, Category = "Loading")
    bool bIsGenerating;

    UPROPERTY(VisibleAnywhere, Category = "Loading")
    UStaticMeshComponent* LoadingIndicator;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnGenerationComplete OnGenerationComplete;

    void SpawnCentralRoomObjects();
    void SpawnDungeonObjects();
    void SpawnExitPortal();
    void BeginAsyncGeneration(const FVector2D& ExitPoint);
};