#pragma once

#include "CoreMinimal.h"
#include "Game/DS_GameModeBase.h"
#include "TacticsRevived/Enums/TacticsEnums.h"
#include "TacticsGameMode.generated.h"

class ABoard;
class AUnit;
class ATacticsGameState;

UCLASS()
class TACTICSREVIVED_API ATacticsGameMode : public ADS_GameModeBase
{
    GENERATED_BODY()

public:
    ATacticsGameMode();

    // Overrides

    virtual bool ReadyToStartMatch_Implementation() override;

    virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

    virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

    virtual void InitGameState() override;

    virtual void HandleMatchHasStarted() override;

    // Getters

    ATacticsGameState* GS() const { return GetGameState<ATacticsGameState>(); }

    void BeginTurn();
    void AdvanceTurn();

public:

    UPROPERTY(EditDefaultsOnly, Category = "Match")
    float TurnDuration = 120.f;

    // Questionable

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Units")
    TMap<ETacticsUnit, TSubclassOf<AUnit>> UnitClassMap;

protected:
    void StartTurnTimer();
    void OnTurnTimerExpired();
    void SpawnPlayerUnits();
    
protected:

    int32 RequiredPlayers = 2;
    FTimerHandle TurnTimerHandle;


private:

    ABoard* Board;

};
