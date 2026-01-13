// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Board.h"
#include "TacticsRevived/Macros/DebugMacros.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"
#include "EngineUtils.h"
#include "Core/Tile.h"
#include "Core/Unit/Unit.h"
#include "Core/Unit/UnitMovementComponent.h"
#include <utility>
#include "Kismet/GameplayStatics.h"
#include "Player/TacticsPlayerController.h"
#include "Player/TacticsPlayerPawn.h"
#include "GameFramework/PlayerState.h"


void ABoard::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    if (HasAuthority())
    {
        SpawnTiles();
    }
    
}

void ABoard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ABoard, Tiles);
}

void ABoard::SpawnTiles()
{
    if (!TileClass) return;

    FVector BoardLocation = GetActorLocation();

    TMap<FIntPoint, ATile*> TileMap;

    for (TActorIterator<AActor> It(GetWorld()); It; ++It)
    {
        AActor* Actor = *It;

        if (!Actor) continue;

        for (const FName& Tag : { "Tile" })
        {
            if (Actor->Tags.Contains(Tag))
            {
                Actor->Destroy();
            }
        }
    }

    for (int Row = 0; Row < Rows; ++Row)
    {
        for (int Col = 0; Col < Columns; ++Col)
        {
            FIntPoint BoardIndex = FIntPoint(Row, Col);
            if (IsTileOutOfBounds(BoardIndex)) continue;

            FVector Location = BoardLocation + FVector(Row * TileSize, Col * TileSize, 0.f);
            ATile* NewTile = GetWorld()->SpawnActor<ATile>(TileClass, Location, FRotator::ZeroRotator);
            if (!NewTile) return;

            NewTile->BoardIndex = BoardIndex;
			NewTile->Tags.Add(FName("Tile"));

            Tiles.Add(NewTile);
            TileMap.Add(BoardIndex, NewTile);
        }
    }

    for (auto& TilePair : TileMap)
    {
        FIntPoint Index = TilePair.Key;
        ATile* Tile = TilePair.Value;

        if (TileMap.Contains(FIntPoint(Index.X + 1, Index.Y)))
            Tile->N = TileMap[FIntPoint(Index.X + 1, Index.Y)];

        if (TileMap.Contains(FIntPoint(Index.X - 1, Index.Y))) 
            Tile->S = TileMap[FIntPoint(Index.X - 1, Index.Y)];

        if (TileMap.Contains(FIntPoint(Index.X, Index.Y - 1))) 
            Tile->W = TileMap[FIntPoint(Index.X, Index.Y - 1)];

        if (TileMap.Contains(FIntPoint(Index.X, Index.Y + 1))) 
            Tile->E = TileMap[FIntPoint(Index.X, Index.Y + 1)];
    }
}

ATile* ABoard::GetTile(FIntPoint Index)
{

    for (ATile* Tile : Tiles)
    {
        if (!Tile) continue;

        if (Tile->BoardIndex == Index)
        {
            return Tile;
        }
    }

    return nullptr;
}

TArray<ATile*> ABoard::GetTilesInRange(ATile* Tile, int32 Range)
{
    TArray<ATile*> ValidMoves = {};

    if (!Tile || !Range) return ValidMoves;

	FIntPoint TileLocation = Tile->BoardIndex;

	int CenterX = TileLocation.X;
	int CenterY = TileLocation.Y;

	int Radius = Range + 1;

    for (int y = CenterY - Radius; y <= CenterY + Radius; ++y) {
        // Calculate the horizontal range for the current row
        int distanceFromCenter = abs(y - CenterY);
        int maxXOffset = Radius - distanceFromCenter;

        // Iterate over columns (x-axis) within the range
        for (int x = CenterX - maxXOffset; x <= CenterX + maxXOffset; ++x) {

            // Exclude the corners (diagonal points)
            if (abs(x - CenterX) + abs(y - CenterY) != Radius) {

                ATile* CurrentTile = GetTile(FIntPoint(x, y));
				if (!CurrentTile) continue;

				bool IsInBounds = !IsTileOutOfBounds(CurrentTile->BoardIndex);

                if (IsInBounds && !CurrentTile->Unit.Get())
                {
                    ValidMoves.Add(CurrentTile);
                }
            }
        }
    }

    return ValidMoves;
}

TArray<ATile*> ABoard::GetTilesInLinearRange(ATile* StartTile, int32 Range)
{
    return {};
}

TArray<ATile*> ABoard::GetPath(AUnit* Unit, ATile* Dest)
{
    if (!Unit || !Unit->Tile.IsValid() || !Dest)
    {
        return TArray<ATile*>();
    }

    ATile* StartTile = Unit->Tile.Get();
    TArray<ATile*> Path;
    TArray<ATile*> Opened;
    TArray<ATile*> Closed;
    TMap<ATile*, ATile*> CameFrom;
    TMap<ATile*, float> GScore;
    TMap<ATile*, float> FScore;

    Opened.Add(StartTile);
    GScore.Add(StartTile, 0.0f);
    FScore.Add(StartTile, GetDistance(StartTile, Dest));

    while (Opened.Num() > 0)
    {
        Opened.Sort([&](const ATile& A, const ATile& B) {
            return FScore[&A] < FScore[&B];
            });

        ATile* Current = Opened[0];
        Opened.RemoveAt(0);

        if (Current == Dest)
        {
            while (CameFrom.Contains(Current))
            {
                Path.Insert(Current, 0);
                Current = CameFrom[Current];
            }
            return Path;
        }

        Closed.Add(Current);

        TArray<ATile*> Neighbors = GetNeighbors(Current);

        for (ATile* Neighbor : Neighbors)
        {
            if (!Neighbor || Closed.Contains(Neighbor))
            {
                continue;
            }

            AUnit* TileUnit = Neighbor->Unit.Get();

            if (TileUnit && (TileUnit->PlayerId != Unit->PlayerId))
            {
                continue;
            }

            if (Closed.Contains(Neighbor)) continue;

            float Score = GScore[Current] + 1.0f;

            if (TileUnit) Score += 0.4f;

            if (!Opened.Contains(Neighbor) || Score < GScore[Neighbor])
            {
                CameFrom.Add(Neighbor, Current);
                GScore.Add(Neighbor, Score);
                FScore.Add(Neighbor, Score + GetDistance(Neighbor, Dest));

                if (!Opened.Contains(Neighbor))
                {
                    Opened.Add(Neighbor);
                }
            }
        }
    }

    return TArray<ATile*>();
}

TArray<ATile*> ABoard::GetUnitPathRange(AUnit* Unit)
{
    if (!Unit || !Unit->Tile.IsValid())
    {
        return TArray<ATile*>();
    }

    ATile* StartTile = Unit->Tile.Get();
    int32 MaxRange = Unit->TileRange;
    TArray<ATile*> MoveTiles;
    TArray<TPair<ATile*, int32>> Search;
    TSet<ATile*> Checked;

    Search.Add(TPair<ATile*, int32>(StartTile, 0));
    Checked.Add(StartTile);

    for (int32 i = 0; i < Search.Num(); i++)
    {
        ATile* Tile = Search[i].Key;
        AUnit* TileUnit = Tile->Unit.Get();
        int32 Distance = Search[i].Value;

        if (TileUnit)
        {
            if (TileUnit != Unit && (TileUnit->PlayerId != Unit->PlayerId))
            {
                continue;
            }
        }
        else
        {
            MoveTiles.Add(Tile);
        }

        if (Distance < MaxRange)
        {
            Distance++;

            if (Tile->N.Get() && !Checked.Contains(Tile->N.Get()))
            {
                Checked.Add(Tile->N.Get());
                Search.Add(TPair<ATile*, int32>(Tile->N.Get(), Distance));
            }
            if (Tile->E.Get() && !Checked.Contains(Tile->E.Get()))
            {
                Checked.Add(Tile->E.Get());
                Search.Add(TPair<ATile*, int32>(Tile->E.Get(), Distance));
            }
            if (Tile->S.Get() && !Checked.Contains(Tile->S.Get()))
            {
                Checked.Add(Tile->S.Get());
                Search.Add(TPair<ATile*, int32>(Tile->S.Get(), Distance));
            }
            if (Tile->W.Get() && !Checked.Contains(Tile->W.Get()))
            {
                Checked.Add(Tile->W.Get());
                Search.Add(TPair<ATile*, int32>(Tile->W.Get(), Distance));
            }
        }
    }

    return MoveTiles;
}

TArray<ATile*> ABoard::GetNeighbors(ATile* Tile)
{
    return
    {
        Tile->N.Get(),
        Tile->S.Get(),
        Tile->E.Get(),
        Tile->W.Get()
    };
}

void ABoard::PlaceUnit(TSubclassOf<AUnit> UnitClass, FIntPoint TargetIndex, APlayerController* Controller, int32 PlayerIndex)
{

    if (!HasAuthority() || !UnitClass || !Controller) return;

    FIntPoint TrueIndex =
        PlayerIndex == 0
        ? TargetIndex
        : FIntPoint(10 - TargetIndex.X, 10 - TargetIndex.Y);

    ATile* Tile = GetTile(TrueIndex);
    int32 PlayerId = Controller->PlayerState->PlayerId;
    ETacticsDirection Direction = PlayerIndex == 0 ? ETacticsDirection::N : ETacticsDirection::S;

    AUnit* UnitDefaults = UnitClass->GetDefaultObject<AUnit>();
    float HH = UnitDefaults->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();

    FVector TL = Tile->GetActorLocation();
    FVector SpawnLocation = FVector(TL.X, TL.Y, TL.Z + HH + 1);
    FRotator SpawnRotation = FRotator(0.f, PlayerIndex == 0 ? 0.f : 180.f, 0.f);

    if (AUnit* SpawnedUnit = GetWorld()->SpawnActor<AUnit>(UnitClass, SpawnLocation, SpawnRotation))
    {
        SpawnedUnit->SetOwner(Controller);
        SpawnedUnit->Board = this;
        SpawnedUnit->Tile = Tile;
        SpawnedUnit->PlayerId = PlayerId;
        SpawnedUnit->Direction = Direction;
        Tile->Unit = SpawnedUnit;
    }
}

ETacticsDirection ABoard::GetDirection(const ATile* A, const ATile* B, bool bSimple)
{
    if (!A || !B)
    {
        return ETacticsDirection::None;
    }

    if (A->BoardIndex == B->BoardIndex)
    {
        return ETacticsDirection::NSEW;
    }

    const int32 xdist = A->BoardIndex.X - B->BoardIndex.X;
    const int32 ydist = A->BoardIndex.Y - B->BoardIndex.Y;

    ETacticsDirection Direction = ETacticsDirection::None;

    if (xdist > 0)
    {
        Direction |= ETacticsDirection::S;
    }
    else if (xdist < 0)
    {
        Direction |= ETacticsDirection::N;
    }

    if (ydist > 0)
    {
        Direction |= ETacticsDirection::W;
    }
    else if (ydist < 0)
    {
        Direction |= ETacticsDirection::E;
    }

    // If bSimple => keep only the stronger axis
    const int32 absX = FMath::Abs(xdist);
    const int32 absY = FMath::Abs(ydist);

    if (bSimple && absX != absY)
    {
        if (absX > absY)
        {
            // Clear out horizontal bits
            Direction = Direction & ~(ETacticsDirection::W | ETacticsDirection::E);
        }
        else
        {
            // Clear out vertical bits
            Direction = Direction & ~(ETacticsDirection::N | ETacticsDirection::S);
        }
    }

    return Direction;
}

float ABoard::GetDirectionYaw(ETacticsDirection Direction)
{
    switch (Direction)
    {
        case ETacticsDirection::N:  return 0.f;
        case ETacticsDirection::E:  return 90.f;
        case ETacticsDirection::S:  return 180.f;
        case ETacticsDirection::W:  return -90.f;
        default:                    return 0.f;
    }
}

float ABoard::GetDegreeDelta(ETacticsDirection A, ETacticsDirection B)
{
    float AngleA = GetDirectionYaw(A); 
    float AngleB = GetDirectionYaw(B);
    return FMath::FindDeltaAngleDegrees(AngleA, AngleB);
}

int8 ABoard::DirectionToIndex(ETacticsDirection Dir)
{
    switch (Dir)
    {
        case ETacticsDirection::N: return 0;
        case ETacticsDirection::E: return 1;
        case ETacticsDirection::S: return 2;
        case ETacticsDirection::W: return 3;
        default:                   return 0;  // Handle "None" or "NSEW" however you like
    }
}

// Convert index 0..3 back to direction
ETacticsDirection ABoard::IndexToDirection(int8 Index)
{
    switch (Index % 4)
    {
        case 0: return ETacticsDirection::N;
        case 1: return ETacticsDirection::E;
        case 2: return ETacticsDirection::S;
        case 3: return ETacticsDirection::W;
        default: return ETacticsDirection::N;
    }
}

// Convert a rotation in degrees to "steps"
// e.g. +90 => +1, -90 => -1, 180 => +2, 0 => +0
int8 ABoard::DegreesToSteps(int32 Degrees)
{
    switch (Degrees)
    {
        case 90:   return +1;
        case -90:  return -1;
        case 180:  return +2;
        case -180: return +2; // Because rotating -180 is the same as +180
        case 0:    return 0;
        default:    return 0;
    }
}

ETacticsDirection ABoard::GetRotation(ETacticsDirection Direction, float Degree)
{
    int8 oldIndex = DirectionToIndex(Direction);

    // Convert the degree-based rotation to steps
    int8 steps = DegreesToSteps(Degree);

    // Add steps and wrap around with mod 4
    int8 newIndex = (oldIndex + steps) % 4;
    // Handle negative wrap
    if (newIndex < 0)
    {
        newIndex += 4;
    }

    // Convert back to ETacticsDirection
    return IndexToDirection(newIndex);
}

float ABoard::GetDistance(ATile* A, ATile* B) {
    if (!A || !B) return 0;
    return FMath::Abs(A->BoardIndex.X - B->BoardIndex.X) 
        + FMath::Abs(A->BoardIndex.Y - B->BoardIndex.Y);
}

bool ABoard::IsTileOutOfBounds(FIntPoint Index)
{
    return OutOfBoundTiles.Contains(Index);
}



