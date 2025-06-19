#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DungeonArea.h"
#include "DungeonGenerator.generated.h"

UENUM(BlueprintType)
enum class EAreaType : uint8
{
    Room,
    Hallway
};

USTRUCT()
struct FAreaInfo
{
    GENERATED_BODY()

    UPROPERTY()
    ADungeonArea* AreaActor;

    UPROPERTY()
    int32 Depth;

    UPROPERTY()
    EAreaType Type;
};

UCLASS()
class WIL_DUNGEON_API ADungeonGenerator : public AActor
{
    GENERATED_BODY()

public:
    ADungeonGenerator();

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void GenerateDungeon();

    UFUNCTION()
    void PlaceExit();

    UFUNCTION()
    bool CheckOverlap(const FVector& Location, const FRotator& Rotation, ADungeonArea* NewArea);

    UFUNCTION()
    ADungeonArea* SpawnArea(EAreaType Type, const FVector& Location, const FRotator& Rotation);

    UPROPERTY(EditAnywhere, Category = "Dungeon")
    TSubclassOf<ADungeonArea> CentralRoomClass;

    UPROPERTY(EditAnywhere, Category = "Dungeon")
    TArray<TSubclassOf<ADungeonArea>> RoomClasses;

    UPROPERTY(EditAnywhere, Category = "Dungeon")
    TArray<TSubclassOf<ADungeonArea>> HallwayClasses;

    UPROPERTY(EditAnywhere, Category = "Dungeon")
    TSubclassOf<ADungeonArea> ExitClass;

    UPROPERTY(EditAnywhere, Category = "Dungeon")
    int32 MaxAreas = 20;

    UPROPERTY(EditAnywhere, Category = "Dungeon")
    int32 MaxDepth = 10;

    UPROPERTY()
    TArray<FAreaInfo> GeneratedAreas;

    UPROPERTY()
    TArray<FDoorInfo> ActiveDoors;
};