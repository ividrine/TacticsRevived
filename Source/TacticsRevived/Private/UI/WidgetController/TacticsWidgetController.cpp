// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/TacticsWidgetController.h"
#include "Player/TacticsPlayerController.h"
#include "Player/TacticsPlayerState.h"

void UTacticsWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& Params)
{
	GameState = Params.GameState;
	PlayerController = Params.PlayerController;
	PlayerState = Params.PlayerState;
	AbilitySystemComponent = Params.AbilitySystemComponent;
	AttributeSet = Params.AttributeSet;
}

void UTacticsWidgetController::BroadcastInitialValues(){ }

void UTacticsWidgetController::BindCallbacksToDependencies(){ }
