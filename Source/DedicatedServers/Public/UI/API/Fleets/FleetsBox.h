// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FleetsBox.generated.h"

class UScrollBox;
class UButton;

UCLASS()
class DEDICATEDSERVERS_API UFleetsBox : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ScrollBox_Fleets;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_GetFleets;
	
	
};
