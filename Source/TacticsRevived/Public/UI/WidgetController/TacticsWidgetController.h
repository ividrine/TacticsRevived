// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TacticsWidgetController.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class ATacticsGameState;
class ATacticsGameState;
class ATacticsPlayerController;
class ATacticsPlayerState;

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{

	GENERATED_BODY()

	FWidgetControllerParams() {}
	FWidgetControllerParams(
		ATacticsGameState* GS,
		ATacticsPlayerController* PC, 
		ATacticsPlayerState* PS,
		UAbilitySystemComponent* ASC, 
		UAttributeSet* AS
	)
		: GameState(GS), PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC), AttributeSet(AS) {
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<ATacticsGameState> GameState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<ATacticsPlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<ATacticsPlayerState> PlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
	
};

UCLASS(Blueprintable)
class TACTICSREVIVED_API UTacticsWidgetController : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& Params);

	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues();

	virtual void BindCallbacksToDependencies();
	
protected:

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<ATacticsGameState> GameState;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<ATacticsPlayerController> PlayerController;
	
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<ATacticsPlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;
	
};
