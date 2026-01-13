#pragma once

#include "CoreMinimal.h"

UENUM()
enum class ETacticsDirection : uint8
{
    None = 0,       // No direction
    N = 1 << 0,
    S = 1 << 1,
    E = 1 << 2,
    W = 1 << 3,
    NSEW = N | S | E | W UMETA(DisplayName = "NSEW"),
};

// Allow bitwise operators on EDirectionFlags
ENUM_CLASS_FLAGS(ETacticsDirection)

UENUM(BlueprintType)
enum class EUnitMovementAnimType : uint8
{
    JogFwd	            UMETA(DisplayName = "JogFwd"),
    JogBwd	            UMETA(DisplayName = "JogBwd"),
    JogLeft	            UMETA(DisplayName = "JogLeft"),
    JogRight            UMETA(DisplayName = "JogRight"),
    Idle                UMETA(DisplayName = "Idle")
};

UENUM(BlueprintType)
enum class EUnitMovementState : uint8
{
    Idle                UMETA(DisplayName = "Idle"),
    MovingPath          UMETA(DisplayName = "MovingPath"),
    MovingWay           UMETA(DisplayName = "MovingWay"),
    MovingBack	        UMETA(DisplayName = "MovingBack"),
};

UENUM(BlueprintType)
enum class ETacticsUnit : uint8
{
    Sparrow     UMETA(DisplayName = "Sparrow"),
    Gideon	    UMETA(DisplayName = "Gideon"),
    Countess	UMETA(DisplayName = "Countess"),
    Greystone	UMETA(DisplayName = "Greystone"),
    Morigesh	UMETA(DisplayName = "Morigesh"),
    Phase       UMETA(DisplayName = "Phase"),
    Fey         UMETA(DisplayName = "The Fey"),
    Aurora      UMETA(DisplayName = "Aurora"),
};

UENUM(BlueprintType)
enum class ETacticsSelectionMode : uint8
{
    Move        UMETA(DisplayName = "Move"),
    Attack      UMETA(DisplayName = "Attack"),
    Rotate      UMETA(DisplayName = "Rotate"),
};

UENUM(BlueprintType)
enum class ETacticsGameMode : uint8
{
    PvAI UMETA(DisplayName = "Player vs AI"),
    PvP UMETA(DisplayName = "Player vs Player")
};

UENUM(BlueprintType)
enum class ETacticsPlayerNum : uint8
{
    None UMETA(DisplayName = "None"),
    PlayerOne UMETA(DisplayName = "PlayerOne"),
    PlayerTwo UMETA(DisplayName = "PlayerTwo")
};