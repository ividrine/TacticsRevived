// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "WebSocket/WebSocketMessageTypes.h"
#include "WebSocketManager.generated.h"

class UDSLocalPlayerSubsystem;
class IWebSocket;
struct FOutgoingGlobalChatMessage;
class UAPIConfig;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWebSocketConnected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChatMessageReceived, const FString&, Sender, const FString&, Message);


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMatchSearching);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMatchCreated, FMatchCreatedMessageDetail, MatchDetails);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAcceptMatch, FAcceptMatchDetail, MatchDetails);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAcceptMatchCompleted, FAcceptMatchCompletedDetail, MatchDetails);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMatchSucceeded, FMatchSucceededMessageDetail, MatchDetails);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMatchTimedOut);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMatchCancelled);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMatchFailed);


/**
 * Handles WebSocket connections and communication.
 */
UCLASS(Blueprintable, BlueprintType)
class DEDICATEDSERVERS_API UWebSocketManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UDSLocalPlayerSubsystem* GetDSLocalPlayerSubsystem() const;

	void Initialize(FSubsystemCollectionBase& Collection) override;

	void Deinitialize() override;

	UFUNCTION(BlueprintCallable)
	bool IsConnected();

	UFUNCTION(BlueprintCallable, Category = "WebSocket")
	void Connect();

	UFUNCTION(BlueprintCallable, Category = "WebSocket")
	void Disconnect();

	UFUNCTION(BlueprintCallable, Category = "WebSocket")
	void SendMessage(FString& Message);
	

	// Delegates for broadcasting events

	UPROPERTY(BlueprintAssignable, Category = "WebSocket")
	FOnWebSocketConnected OnWebSocketConnected;

	UPROPERTY(BlueprintAssignable, Category = "WebSocket")
	FOnChatMessageReceived OnChatMessageReceived;

	UPROPERTY(BlueprintAssignable, Category = "WebSocket")
	FOnMatchSearching OnMatchSearching;

	UPROPERTY(BlueprintAssignable, Category = "WebSocket")
	FOnMatchCreated OnMatchCreated;

	UPROPERTY(BlueprintAssignable, Category = "WebSocket")
	FOnAcceptMatch OnAcceptMatch;

	UPROPERTY(BlueprintAssignable, Category = "WebSocket")
	FOnAcceptMatchCompleted OnAcceptMatchCompleted;

	UPROPERTY(BlueprintAssignable, Category = "WebSocket")
	FOnMatchSucceeded OnMatchSucceeded;

	UPROPERTY(BlueprintAssignable, Category = "WebSocket")
	FOnMatchTimedOut OnMatchTimedOut;

	UPROPERTY(BlueprintAssignable, Category = "WebSocket")
	FOnMatchCancelled OnMatchCancelled;

	UPROPERTY(BlueprintAssignable, Category = "WebSocket")
	FOnMatchFailed OnMatchFailed;

private:
	TSharedPtr<IWebSocket> WebSocket;

	TObjectPtr<UAPIConfig> APIConfig;

	// Internal handler for WebSocket messages
	void OnMessage(const FString& Message) const;

	void OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean) const;

	void OnConnectionError(const FString& Error) const;

	void OnConnected() const;
};
