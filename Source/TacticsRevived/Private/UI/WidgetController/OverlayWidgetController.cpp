// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"
#include "Core/Unit/UnitAttributeSet.h"
#include "TacticsRevived/Enums/TacticsEnums.h"
#include "Player/TacticsPlayerState.h"
#include "GameFramework/PlayerState.h"
#include "Game/TacticsGameState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	OnSelectionModeChanged.Broadcast(ETacticsSelectionMode::Move);

	/*const UUnitAttributeSet* AttrSet = CastChecked<UUnitAttributeSet>(AttributeSet);
	OnHealthChanged.Broadcast(AttrSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AttrSet->GetMaxHealth());*/
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	GameState->OnActivePlayerChanged.AddLambda([this](APlayerState* NewPlayerState)
	{
		OnPlayerTurnChanged.Broadcast(NewPlayerState->GetPlayerId() == PlayerState->GetPlayerId());
	});
}
