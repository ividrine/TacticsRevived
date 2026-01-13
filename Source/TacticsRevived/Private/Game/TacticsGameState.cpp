// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TacticsGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

void ATacticsGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATacticsGameState, TurnNumber);
	DOREPLIFETIME(ATacticsGameState, RemainingTurnTime);
	DOREPLIFETIME(ATacticsGameState, ActivePlayer);
}

void ATacticsGameState::OnRep_ActivePlayer()
{
	OnActivePlayerChanged.Broadcast(ActivePlayer);
}

void ATacticsGameState::OnRep_TurnTime()
{
}

void ATacticsGameState::OnRep_TurnNumber()
{

}
