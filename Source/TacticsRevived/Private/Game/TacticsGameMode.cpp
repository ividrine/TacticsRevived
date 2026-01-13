// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/TacticsGameMode.h"
#include "EngineUtils.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "Game/TacticsGameState.h"
#include "Player/TacticsPlayerController.h"
#include "Player/TacticsPlayerState.h"
#include "TacticsRevived/Enums/TacticsEnums.h"
#include "Player/TacticsPlayerPawn.h"
#include "TacticsRevived/Macros/DebugMacros.h"
#include "Camera/CameraComponent.h"
#include "Core/Unit/Unit.h"
#include "Core/Board.h"
#include "Player/TacticsPlayerStart.h"

ATacticsGameMode::ATacticsGameMode()
{
}

bool ATacticsGameMode::ReadyToStartMatch_Implementation()
{
    if (!Super::ReadyToStartMatch_Implementation())
    {
        return false;
    }

    return bIsReadyToStart && PlayerReadyCount >= RequiredPlayers;
}

void ATacticsGameMode::InitGameState()
{
	Super::InitGameState();

	Board = Cast<ABoard>(UGameplayStatics::GetActorOfClass(GetWorld(), ABoard::StaticClass()));
}

void ATacticsGameMode::HandleMatchHasStarted()
{
    Super::HandleMatchHasStarted();
    
    SpawnPlayerUnits();

    for (APlayerController* Player : ConnectedPlayers)
    {
        ADSPlayerController* MyPC = Cast<ADSPlayerController>(Player);
        if (MyPC)
        {
            MyPC->ClientHideLoadingScreen();
        }
    }

    BeginTurn();
}

void ATacticsGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
    Super::HandleStartingNewPlayer_Implementation(NewPlayer);

    if (NumPlayers > RequiredPlayers)
    {
        NewPlayer->StartSpectatingOnly();
    }
}

AActor* ATacticsGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
    AActor* SelectedActor = nullptr;

    TArray<AActor*> PlayerStarts;

    int32 PlayerIndex = GS()->PlayerArray.IndexOfByKey(Player->PlayerState);

    FName DesiredTag = (PlayerIndex == 0) ? FName("1") : FName("2");

    UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);

    for (auto Start : PlayerStarts)
    {
        if (APlayerStart* PlayerStart = Cast<APlayerStart>(Start))
        {
            if (PlayerStart->PlayerStartTag == DesiredTag)
            {
                SelectedActor = PlayerStart;
                break;
            }
        }
    }

    return SelectedActor;
}

void ATacticsGameMode::SpawnPlayerUnits()
{
    for (int32 i = 0; i < GS()->PlayerArray.Num(); i++)
    {
        if (ATacticsPlayerState* PS = Cast<ATacticsPlayerState>(GS()->PlayerArray[i]))
        {
            for (const TTuple<FIntPoint, ETacticsUnit>Pair : PS->UnitLayout)
            {
                TSubclassOf<AUnit>* UnitClass = UnitClassMap.Find(Pair.Value);

                if (!UnitClass) continue;

                Board->PlaceUnit(UnitClass->Get(), Pair.Key, PS->GetPlayerController(), i);
            }
        }
    }
}


void ATacticsGameMode::StartTurnTimer()
{
    // clear any previous timer (player ended early, etc.)
    GetWorldTimerManager().ClearTimer(TurnTimerHandle);

    // set a new countdown
    GetWorldTimerManager().SetTimer(
        TurnTimerHandle,
        this,
        &ATacticsGameMode::AdvanceTurn,
        TurnDuration,
        false);
}

void ATacticsGameMode::OnTurnTimerExpired()
{
}

void ATacticsGameMode::BeginTurn()
{
    GS()->ActivePlayer = GS()->PlayerArray[0];
    StartTurnTimer();
}

void ATacticsGameMode::AdvanceTurn()
{
    int32 ActiveIndex = GS()->PlayerArray.IndexOfByKey(GS()->ActivePlayer);
    bool isValidIndex = GS()->PlayerArray.IsValidIndex(ActiveIndex + 1);

    if (isValidIndex)
        GS()->ActivePlayer = GS()->PlayerArray[ActiveIndex + 1];
    else
        GS()->ActivePlayer = GS()->PlayerArray[0];

    StartTurnTimer();
}




