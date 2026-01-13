// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/HTTP/HttpRequestManager.h"
#include "Interfaces/PortalManagement.h"
#include "PortalManager.generated.h"


UCLASS()
class DEDICATEDSERVERS_API UPortalManager : public UHttpRequestManager, public IPortalManagement
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FAPIStatusMessage SignInStatusMessageDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnAPIRequestStart LoadingDelegate;

	void SignIn(const FString& Username, const FString& Password);
	void SignOut(const FString& AccessToken);

	// IPortalManagement
	virtual void RefreshTokens(const FString& RefreshToken) override;

	UFUNCTION()
	void QuitGame();

	FString LastUsername;

private:
	void SignIn_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void RefreshTokens_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void SignOut_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	
};