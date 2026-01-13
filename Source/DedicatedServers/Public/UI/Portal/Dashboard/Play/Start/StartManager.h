// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/HTTP/HttpRequestManager.h"
#include "StartManager.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMatchStartSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMatchStartError, FString, Error);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMatchStopSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMatchStopError, FString, Error);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMatchAcceptSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMatchAcceptError, FString, Error);


/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UStartManager : public UHttpRequestManager
{
	GENERATED_BODY()
	
public:

	/* Matchmaking delegates */

	UPROPERTY()
	FOnMatchStartSuccess OnMatchStartSuccess;

	UPROPERTY()
	FOnMatchStartError OnMatchStartError;

	UPROPERTY()
	FOnMatchStopSuccess OnMatchStopSuccess;

	UPROPERTY()
	FOnMatchStopError OnMatchStopError;

	UPROPERTY()
	FOnMatchAcceptSuccess OnMatchAcceptSuccess;

	UPROPERTY()
	FOnMatchAcceptError OnMatchAcceptError;

	void StartMatchmaking();

	void StopMatchmaking();

	void AcceptMatch(bool accepted);

	void ClearTicket();
	
private:
	FString TicketId;

	void StartMatchmaking_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void StopMatchmaking_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void AcceptMatch_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
