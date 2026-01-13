// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameConfig.generated.h"

/**
 * 
 */
UCLASS(config=Game, defaultconfig)
class DEDICATEDSERVERS_API UGameConfig : public UObject
{
	GENERATED_BODY()
	
	
public:
	UPROPERTY(Config)
	FString DevBaseURL;

	UPROPERTY(Config)
	FString ProductionBaseURL;
	
};
