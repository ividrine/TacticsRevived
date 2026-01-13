// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TacticsHUD.generated.h"

class ATacticsGameState;
class ATacticsPlayerController;
class ATacticsPlayerState;

class UAbilitySystemComponent;
class UAttributeSet;
class UTacticsUserWidget;
class UOverlayWidgetController;

struct FWidgetControllerParams;

UCLASS()
class TACTICSREVIVED_API ATacticsHUD : public AHUD
{
	GENERATED_BODY()
	
	public:
		UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& Params);

		void InitOverlay(ATacticsGameState* GS, ATacticsPlayerController* PC, ATacticsPlayerState* PS);

	private:

		UPROPERTY()
		TObjectPtr<UTacticsUserWidget> OverlayWidget;

		UPROPERTY(EditAnywhere)
		TSubclassOf<UTacticsUserWidget> OverlayWidgetClass;

		UPROPERTY()
		TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

		UPROPERTY(EditAnywhere)
		TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
};
