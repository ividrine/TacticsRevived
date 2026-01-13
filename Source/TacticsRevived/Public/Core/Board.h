// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TacticsRevived/Enums/TacticsEnums.h"
#include "Board.generated.h"

class ATile;
class AUnit;

UCLASS()
class TACTICSREVIVED_API ABoard : public AActor
{
	GENERATED_BODY()

    public:

        void SpawnTiles();

        // Tile Operations
        ATile* GetTile(FIntPoint BoardIndex);
        TArray<ATile*> GetTilesInRange(ATile* StartTile, int32 Range);
        TArray<ATile*> GetTilesInLinearRange(ATile* StartTile, int32 Range);
        TArray<ATile*> GetPath(AUnit* Unit, ATile* Dest);
        TArray<ATile*> GetUnitPathRange(AUnit* Unit);
        TArray<ATile*> GetNeighbors(ATile* Tile);
        bool IsTileOutOfBounds(FIntPoint Index);
        float GetDistance(ATile* A, ATile* B);
        void PlaceUnit(TSubclassOf<AUnit> UnitClass, FIntPoint TargetIndex, APlayerController* Controller, int32 PlayerIndex);

        ETacticsDirection GetDirection(const ATile* A, const ATile* B, bool Simple = true);
        float GetDirectionYaw(ETacticsDirection Direction);
        float GetDegreeDelta(ETacticsDirection A, ETacticsDirection B);
        ETacticsDirection GetRotation(ETacticsDirection, float Degree);

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
        float TileSize = 180.f;

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
        TSubclassOf<ATile> TileClass;

        UPROPERTY(Replicated)
        TArray<ATile*> Tiles = TArray<ATile*>();

    protected:

        virtual void OnConstruction(const FTransform& Transform) override;

    private:
        int8 DirectionToIndex(ETacticsDirection Dir);
        ETacticsDirection IndexToDirection(int8 Index);
        int8 DegreesToSteps(int32 Degrees);

    private:

        int Rows = 11;
        int Columns = 11;

        TArray<FIntPoint> OutOfBoundTiles =
        {
            {0, 0},
            {0, 1},
            {0, 9},
            {0, 10},
            {1, 0},
            {1, 10},
            {9, 0},
            {9, 10},
            {10, 0},
            {10, 1},
            {10, 9},
            {10, 10}
        };

};
