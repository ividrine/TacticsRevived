

#pragma once

#include "CoreMinimal.h"
#include "Player/DSPlayerController.h"
#include "TacticsPlayerController.generated.h"

UCLASS()
class TACTICSREVIVED_API ATacticsPlayerController : public ADSPlayerController
{
	GENERATED_BODY()

public:
    ATacticsPlayerController();
    virtual void BeginPlay() override;

   // RPC

    UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
    void ServerRequestEndTurn();
};
