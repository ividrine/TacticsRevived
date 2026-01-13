// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RadialProgress.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API URadialProgress : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION()
	void SetPercentValue(float percentValue);
	
	
};
