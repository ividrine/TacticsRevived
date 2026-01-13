// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/TacticsWidgetController.h"
#include "OverlayWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSignature, float, NewMaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnitSelectedSignature, AUnit*, NewUnit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectionModeChangedSignature, ETacticsSelectionMode, NewSelectionMode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerTurnChangedSignature, bool, IsPlayerTurn);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class TACTICSREVIVED_API UOverlayWidgetController : public UTacticsWidgetController
{
	GENERATED_BODY()
	
public:

	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable)
	FOnUnitSelectedSignature OnUnitChanged;

	UPROPERTY(BlueprintAssignable)
	FOnSelectionModeChangedSignature OnSelectionModeChanged;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerTurnChangedSignature OnPlayerTurnChanged;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnMaxHealthChangedSignature OnMaxHealthChanged;
	
	
};
