// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartPage.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMatchmakingEvent);

class UButton;
class UWebSocketManager;
class UStartManager;

UCLASS()
class DEDICATEDSERVERS_API UStartPage : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	FOnMatchmakingEvent OnMatchmakingStarted;

	UPROPERTY()
	FOnMatchmakingEvent OnMatchmakingStopped;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* FindMatch_Button;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(BlueprintReadOnly)
	bool bIsMatchmaking;

private:

	UPROPERTY()
	TObjectPtr<UWebSocketManager> WebSocketManager;

	UPROPERTY()
	TObjectPtr<UStartManager> StartManager;

	UFUNCTION()
	void FindMatchButtonClicked();

	/* Http response delegate handlers */

	UFUNCTION()
	void OnMatchStartSuccess();

	UFUNCTION()
	void OnMatchStartError(FString Error);

	UFUNCTION()
	void OnMatchStopSuccess();

	UFUNCTION()
	void OnMatchStopError(FString Error);

	/* WebSocket matchmaking event handling */

	UFUNCTION()
	void OnMatchSearching();

	UFUNCTION()
	void OnMatchCreated(FMatchCreatedMessageDetail MatchDetails);

	UFUNCTION()
	void OnMatchSucceeded(FMatchSucceededMessageDetail MatchDetails);

	UFUNCTION()
	void OnMatchTimedOut();

	UFUNCTION()
	void OnMatchCancelled();

	UFUNCTION()
	void OnMatchFailed();
	
};
