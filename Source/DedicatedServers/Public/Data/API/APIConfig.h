// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "APIConfig.generated.h"

class UAPIData;

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UAPIConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	FString GetAPIEndpoint(FString Path);
	FString GetWebSocketUrl();

protected:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAPIData> Production;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAPIData> Development;
	
};
