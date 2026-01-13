#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TacticsRevived/Enums/TacticsEnums.h"
#include "UnitMovementComponent.generated.h"

class ATile;
class AUnit;

UCLASS()
class TACTICSREVIVED_API UUnitMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UUnitMovementComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void MovePath(const TArray<ATile*>& TilePath);
	void MoveWay(ATile* PrevTile, ATile* NextTile);
	void MoveBack();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed = 220.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float LocationInterpSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RotationInterpSpeed = 0.f;

private:

	// Tick function

	void TickMovePath(float DeltaTime);
	void TickMoveWay(float DeltaTime);
	void TickMoveBack(float DeltaTime);

	// Tick helpers

	void UpdatePosition(float DeltaTime, bool shouldRotate = true);

	// Util functions

	FVector GetTileLocation(ATile* Tile);
	void MoveUnitsOnPath();
	bool HasReachedTarget();

protected:
	virtual void BeginPlay() override;

private:

	// State variables

	AUnit* OwnerUnit = nullptr;
	TArray<ATile*> Path = TArray<ATile*>();
	FVector StartLocation = FVector::ZeroVector;
	FVector TargetLocation = FVector::ZeroVector;
	int32 PathIndex = 0;
	int32 MoveWayDegreeDelta = 0;
	float zAxis = 0;

	// Mapping for moving a unit out of the way based on rotation

	TMap<int, EUnitMovementAnimType> MoveWayAnimMap = {
		{0, EUnitMovementAnimType::JogFwd},
		{-90, EUnitMovementAnimType::JogLeft},
		{90, EUnitMovementAnimType::JogRight},
		{180, EUnitMovementAnimType::JogBwd},
		{-180, EUnitMovementAnimType::JogBwd }
	};

	// Mapping for moving a unit back to location after moving out of the way

	TMap<int, EUnitMovementAnimType> MoveBackAnimMap = {
		{0, EUnitMovementAnimType::JogBwd},
		{-90, EUnitMovementAnimType::JogRight},
		{90, EUnitMovementAnimType::JogLeft},
		{180, EUnitMovementAnimType::JogFwd},
		{-180, EUnitMovementAnimType::JogFwd }
	};
};
