#include "Core/Unit/UnitMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "Net/UnrealNetwork.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TacticsRevived/Constants/AnimCurves.h"
#include "Core/Board.h"
#include "Core/Unit/Unit.h"
#include "Core/Tile.h"

UUnitMovementComponent::UUnitMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UUnitMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerUnit = GetOwner<AUnit>();

	OwnerUnit->ServerRotation = OwnerUnit->GetActorRotation();
	OwnerUnit->ServerLocation = OwnerUnit->GetActorLocation();

	if (OwnerUnit->HasAuthority())
	{
		zAxis = OwnerUnit->GetActorLocation().Z;
	}
}

void UUnitMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!OwnerUnit) return;

	// Interpolate unit position client side

	if (!OwnerUnit->HasAuthority())
	{
		FVector NewLocation = FMath::VInterpTo(OwnerUnit->GetActorLocation(), OwnerUnit->ServerLocation, DeltaTime, LocationInterpSpeed);
		FRotator NewRotation = FMath::RInterpTo(OwnerUnit->GetActorRotation(), OwnerUnit->ServerRotation, DeltaTime, RotationInterpSpeed);
		OwnerUnit->SetActorLocationAndRotation(NewLocation, NewRotation);
	}

	// Server side movement

	if (OwnerUnit->HasAuthority() && !OwnerUnit->IsIdle())
	{
		OwnerUnit->ServerRotation = OwnerUnit->GetActorRotation();
		OwnerUnit->ServerLocation = OwnerUnit->GetActorLocation();

		if (OwnerUnit->IsMovingPath()) TickMovePath(DeltaTime);
		else if (OwnerUnit->IsMovingWay()) TickMoveWay(DeltaTime);
		else if (OwnerUnit->IsMovingBack()) TickMoveBack(DeltaTime);
	}
}

void UUnitMovementComponent::MovePath(const TArray<ATile*>& TilePath)
{
	if (TilePath.Num() < 1 || !OwnerUnit) return;
	Path = TilePath;
	PathIndex = 0;
	TargetLocation = GetTileLocation(Path[PathIndex]);
	StartLocation = OwnerUnit->GetActorLocation();
	OwnerUnit->MovementState = EUnitMovementState::MovingPath;
}

void UUnitMovementComponent::TickMovePath(float DeltaTime)
{
	MoveUnitsOnPath();

	UpdatePosition(DeltaTime);

	if (!HasReachedTarget()) return;

	if (Path.IsValidIndex(PathIndex - 1))
	{
		OwnerUnit->Direction = OwnerUnit->Board->GetDirection(Path[PathIndex - 1], Path[PathIndex]);
	}
	else
	{
		OwnerUnit->Direction = OwnerUnit->Board->GetDirection(OwnerUnit->Tile.Get(), Path[PathIndex]);
	}

	if (Path.IsValidIndex(PathIndex + 1))
	{
		PathIndex++;
		TargetLocation = GetTileLocation(Path[PathIndex]);
		StartLocation = OwnerUnit->GetActorLocation();
		
	}
	else
	{
		OwnerUnit->MovementState = EUnitMovementState::Idle;
		ATile* EndTile = Path[Path.Num() - 1];
		ATile* StartTile = OwnerUnit->Tile.Get();
		if (!EndTile || !StartTile) return;

		StartTile->Unit.Reset();
		EndTile->Unit = OwnerUnit;
		OwnerUnit->Tile = EndTile;
	}
	
}

void UUnitMovementComponent::MoveWay(ATile* PrevTile, ATile* NextTile)
{
	ATile* ThisTile = OwnerUnit->Tile.Get();
	ETacticsDirection DirectionToMove = ETacticsDirection::None;

	StartLocation = OwnerUnit->ServerLocation;

	// This unit cannot move in the direction of the tile the running unit is currently on or the tile it is going to

	TArray<ETacticsDirection> BadDirections = {
		OwnerUnit->Board->GetDirection(ThisTile, NextTile),
		OwnerUnit->Board->GetDirection(ThisTile, PrevTile)
	};

	// Possible directions this unit can move to in order to get out of way of running unit

	TArray<ETacticsDirection> PossibleDirections = {
		OwnerUnit->Board->GetRotation(OwnerUnit->Direction, 180),
		OwnerUnit->Board->GetRotation(OwnerUnit->Direction, 90),
		OwnerUnit->Board->GetRotation(OwnerUnit->Direction, -90)
	};

	// Find a valid direction to move to get out of the way

	for (ETacticsDirection Direction : PossibleDirections)
	{
		if (!BadDirections.Contains(Direction))
		{
			DirectionToMove = Direction;
			break;
		}
	}

	MoveWayDegreeDelta = OwnerUnit->Board->GetDegreeDelta(OwnerUnit->Direction, DirectionToMove);

	// Calculate target location

	FVector DirectionVector = (
		MoveWayDegreeDelta == 0 || FMath::Abs(MoveWayDegreeDelta) == 180
	) ? OwnerUnit->GetActorForwardVector() : OwnerUnit->GetActorRightVector();
	
	float Distance = (MoveWayDegreeDelta == -90 || FMath::Abs(MoveWayDegreeDelta) == 180) ? -90.f : 90.f;

	TargetLocation = OwnerUnit->ServerLocation + (DirectionVector * Distance);
	
	// Movement State

	OwnerUnit->MovementState = EUnitMovementState::MovingWay;
	OwnerUnit->bHasMovedBack = false;
	OwnerUnit->MovementType = MoveWayAnimMap[MoveWayDegreeDelta];
	
}

void UUnitMovementComponent::TickMoveWay(float DeltaTime)
{
	UpdatePosition(DeltaTime, false);

	if (HasReachedTarget())
	{
		OwnerUnit->MovementState = EUnitMovementState::Idle;
		OwnerUnit->bHasMovedWay = true;
	}
}

void UUnitMovementComponent::MoveBack()
{
	StartLocation = OwnerUnit->ServerLocation;
	TargetLocation = GetTileLocation(OwnerUnit->Tile.Get());
	OwnerUnit->MovementState = EUnitMovementState::MovingBack;
	OwnerUnit->MovementType = MoveBackAnimMap[MoveWayDegreeDelta];
	OwnerUnit->bHasMovedWay = false;
	OwnerUnit->bHasMovedBack = false;
}

void UUnitMovementComponent::TickMoveBack(float DeltaTime)
{
	UpdatePosition(DeltaTime, false);

	if (HasReachedTarget())
	{
		OwnerUnit->MovementType = EUnitMovementAnimType::Idle;
		OwnerUnit->MovementState = EUnitMovementState::Idle;
		OwnerUnit->bHasMovedBack = true;
	}
}

void UUnitMovementComponent::MoveUnitsOnPath()
{
	ATile* CurrentTile = Path[PathIndex];
	AUnit* CurrentUnit = CurrentTile ? CurrentTile->Unit.Get() : nullptr;
	AUnit* PrevUnit = Path.IsValidIndex(PathIndex - 1) ? Path[PathIndex - 1]->Unit.Get() : nullptr;

	if (CurrentUnit && !CurrentUnit->IsMovingWay() && !CurrentUnit->bHasMovedWay)
	{
		ATile* StartTile = OwnerUnit->Tile.Get();

		CurrentUnit->GetMovementComponent()->MoveWay(
			Path.IsValidIndex(PathIndex - 1) ? Path[PathIndex - 1] : StartTile,
			Path[PathIndex + 1]
		);
	}

	if (PrevUnit && !PrevUnit->IsMovingBack() && !PrevUnit->bHasMovedBack)
	{
		PrevUnit->GetMovementComponent()->MoveBack();
	}
}

FVector UUnitMovementComponent::GetTileLocation(ATile* Tile)
{
	FVector TileLocation = Tile->GetActorLocation();
	return FVector(TileLocation.X, TileLocation.Y, zAxis);
}

bool UUnitMovementComponent::HasReachedTarget()
{
	const FVector PathVector = TargetLocation - StartLocation;
	const FVector ToUnit = OwnerUnit->GetActorLocation() - StartLocation;
	const float TotalDistanceSq = PathVector.SizeSquared();
	const float Progress = FVector::DotProduct(ToUnit, PathVector.GetSafeNormal());
	return Progress * Progress >= TotalDistanceSq;
}

void UUnitMovementComponent::UpdatePosition(float DeltaTime, bool shouldRotate)
{
	FVector MoveDirection = (TargetLocation - OwnerUnit->ServerLocation).GetSafeNormal();
	FVector Velocity = MoveDirection * (MoveSpeed * DeltaTime);
	FVector NewLocation = OwnerUnit->ServerLocation + Velocity;
	FRotator NewRotation = MoveDirection.Rotation();
	if (shouldRotate) OwnerUnit->SetActorLocationAndRotation(NewLocation, NewRotation);
	else OwnerUnit->SetActorLocation(NewLocation);
}







