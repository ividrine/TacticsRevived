


#include "Player/TacticsPlayerController.h"
#include "Player/TacticsPlayerPawn.h"
#include "TacticsRevived/Macros/DebugMacros.h"
#include "Game/TacticsGameState.h"
#include "Game/TacticsGameMode.h"
#include "GameFramework/PlayerState.h"
#include <Kismet/GameplayStatics.h>


ATacticsPlayerController::ATacticsPlayerController()
{
}

void ATacticsPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        // Server-specific setup (if needed)
        return;
    }

    ServerNotifyLevelLoaded();
}

void ATacticsPlayerController::ServerRequestEndTurn_Implementation()
{
    if (ATacticsGameMode* GM = Cast<ATacticsGameMode>(UGameplayStatics::GetGameMode(this)))
    {
        GM->AdvanceTurn();
    }
}

bool ATacticsPlayerController::ServerRequestEndTurn_Validate()
{
    if (ATacticsGameState* GS = Cast<ATacticsGameState>(UGameplayStatics::GetGameState(this)))
    {
        return GS->ActivePlayer->PlayerId == GetPlayerState<APlayerState>()->PlayerId;
    }
    else
    {
        return false;
    }
}
