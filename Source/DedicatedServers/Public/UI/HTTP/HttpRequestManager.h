// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "HttpModule.h"
#include "HttpRequestManager.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAPIRequestSucceeded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAPIStatusMessage, const FString&, StatusMessage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAPIRequestStart, bool, visible);

class UAPIConfig;
class UDSLocalPlayerSubsystem;

UCLASS(Blueprintable)
class DEDICATEDSERVERS_API UHttpRequestManager : public UObject
{
	GENERATED_BODY()

public:
	UHttpRequestManager();

	UDSLocalPlayerSubsystem* GetDSLocalPlayerSubsystem() const;

protected:

	TObjectPtr<UAPIConfig> APIConfig;

	bool IsSuccess(FHttpResponsePtr Response);

	FString SerializeJsonContent(const TMap<FString, FString>& Params);

};