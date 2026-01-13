#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "TacticsRevived/Enums/TacticsEnums.h"
#include "TacticsGameState.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnActivePlayerChanged, APlayerState*)

UCLASS()
class TACTICSREVIVED_API ATacticsGameState : public AGameState
{
    GENERATED_BODY()

public:

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    FOnActivePlayerChanged OnActivePlayerChanged;

    UPROPERTY(ReplicatedUsing = OnRep_ActivePlayer, BlueprintReadOnly, Category = "Turn")
    APlayerState* ActivePlayer = nullptr;

    /** Increasing counter each time we advance the turn */
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Turn")
    int32 TurnNumber = 0;

    /** Seconds left before auto-end */
    UPROPERTY(ReplicatedUsing = OnRep_TurnTime, BlueprintReadOnly, Category = "Turn")
    float RemainingTurnTime = 0.f;

    UFUNCTION()
    void OnRep_ActivePlayer();

    UFUNCTION()
    void OnRep_TurnTime();

    UFUNCTION()
    void OnRep_TurnNumber();


};
