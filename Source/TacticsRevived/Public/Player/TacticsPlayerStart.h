// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "TacticsRevived/Enums/TacticsEnums.h"
#include "TacticsPlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class TACTICSREVIVED_API ATacticsPlayerStart : public APlayerStart
{
	GENERATED_BODY()
	
	
public:
	UPROPERTY(EditAnywhere)
	int32 PlayerNum;
	
};
