// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/HTTP/HttpRequestManager.h"
#include "Interfaces/IHttpRequest.h"
#include "APITestManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFleetsResponseReceived, const FleetsResponse&, Response, bool, bWasSuccessful);

UCLASS()
class DEDICATEDSERVERS_API UAPITestManager : public UHttpRequestManager
{
	GENERATED_BODY()
	
	
public:
	UFUNCTION()
	void GetFleets();

	UPROPERTY()
	FOnFleetsResponseReceived OnFleetsResponseReceived;

	void GetFleets_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	
};
