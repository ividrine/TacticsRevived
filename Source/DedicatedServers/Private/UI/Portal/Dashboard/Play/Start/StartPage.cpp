// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/Dashboard/Play/Start/StartPage.h"
#include "UI/Portal/Dashboard/Play/Start/StartManager.h"
#include "WebSocket/WebSocketMessageTypes.h"
#include "WebSocket/WebSocketManager.h"
#include "DedicatedServers/DedicatedServers.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DSPlayerController.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"


void UStartPage::NativeConstruct()
{
	Super::NativeConstruct();

	StartManager = NewObject<UStartManager>(this);
	WebSocketManager = GetGameInstance()->GetSubsystem<UWebSocketManager>();

	// Bind http resonse delegate handlers
	StartManager->OnMatchStartSuccess.AddDynamic(this, &UStartPage::OnMatchStartSuccess);
	StartManager->OnMatchStartError.AddDynamic(this, &UStartPage::OnMatchStartError);
	StartManager->OnMatchStopSuccess.AddDynamic(this, &UStartPage::OnMatchStopSuccess);
	StartManager->OnMatchStopError.AddDynamic(this, &UStartPage::OnMatchStopError);

	// Bind web socket matchmaking event delegate handlers
	WebSocketManager->OnMatchSearching.AddDynamic(this, &UStartPage::OnMatchSearching);
	WebSocketManager->OnMatchCreated.AddDynamic(this, &UStartPage::OnMatchCreated);
	WebSocketManager->OnMatchSucceeded.AddDynamic(this, &UStartPage::OnMatchSucceeded);
	WebSocketManager->OnMatchTimedOut.AddDynamic(this, &UStartPage::OnMatchTimedOut);
	WebSocketManager->OnMatchCancelled.AddDynamic(this, &UStartPage::OnMatchCancelled);
	WebSocketManager->OnMatchFailed.AddDynamic(this, &UStartPage::OnMatchFailed);

	// UI event binding
	FindMatch_Button->OnClicked.AddDynamic(this, &UStartPage::FindMatchButtonClicked);
}

void UStartPage::NativeDestruct()
{
	Super::NativeDestruct();
}

void UStartPage::FindMatchButtonClicked()
{
	if (!bIsMatchmaking)
	{
		// Start Matchmaking
		FindMatch_Button->SetIsEnabled(false);
		StartManager->StartMatchmaking();

		// Broadcast early for benefit of UX. 
		// If StartMatchmaking() fails we broadcast stop event
		OnMatchmakingStarted.Broadcast();
	}
	else
	{
		// Stop matchmaking
		FindMatch_Button->SetIsEnabled(false);
		StartManager->StopMatchmaking();

		// We can broadcast early, since whether or not 
		// StopMatchmaking() succeeds, we will stop matchmaking
		OnMatchmakingStopped.Broadcast();
	}
}

void UStartPage::OnMatchStartSuccess()
{
	bIsMatchmaking = true;
	FindMatch_Button->SetIsEnabled(true);
}

void UStartPage::OnMatchStartError(FString Error)
{
	UE_LOG(LogDedicatedServers, Log, TEXT("Match start API error"), *Error);

	bIsMatchmaking = false;
	FindMatch_Button->SetIsEnabled(true);
	OnMatchmakingStopped.Broadcast();
	
}

void UStartPage::OnMatchStopSuccess()
{
	bIsMatchmaking = false;
	FindMatch_Button->SetIsEnabled(true);
}

void UStartPage::OnMatchStopError(FString Error)
{
	bIsMatchmaking = false;
	FindMatch_Button->SetIsEnabled(true);
}

/* Websocket matchmaking event handlers */

void UStartPage::OnMatchSearching()
{
	// Probably dont need to do anything here since API manager handles this
	UE_LOG(LogDedicatedServers, Log, TEXT("OnMatchSearching Event Emitted"));
	// Todo figure out if we need to do anything here
}

void UStartPage::OnMatchCreated(FMatchCreatedMessageDetail MatchDetails)
{
	UE_LOG(LogDedicatedServers, Log, TEXT("OnMatchCreated Event Emitted"));
	// Maybe update text?
}

void UStartPage::OnMatchSucceeded(FMatchSucceededMessageDetail MatchDetails)
{
	UE_LOG(LogDedicatedServers, Log, TEXT("OnMatchSucceeded Event Emitted"));

	if (ADSPlayerController* PC = GetOwningPlayer<ADSPlayerController>())
	{
		PC->ShowLoadingScreen(MatchDetails);

		const FString ip = MatchDetails.gameSessionInfo.ipAddress;
		const int port = MatchDetails.gameSessionInfo.port;
		const FString playerSessionId = MatchDetails.playerSessionId;
		const FString username = MatchDetails.playerId;

		const FString Options = "PlayerSessionId=" + playerSessionId + "?Username=" + username;
		const FString Url = ip + TEXT(":") + FString::FromInt(port) + TEXT("/Game/Maps/Default");
		const FName Address(*Url);

		UGameplayStatics::OpenLevel(this, Address, true, Options);
	}
}

void UStartPage::OnMatchTimedOut()
{
	UE_LOG(LogDedicatedServers, Log, TEXT("OnMatchTimedOut Event Emitted"));
	FindMatch_Button->SetIsEnabled(true);
	bIsMatchmaking = false;
	OnMatchmakingStopped.Broadcast();
}

void UStartPage::OnMatchCancelled()
{
	// Probably dont need to do anything here since API manager handles this
	UE_LOG(LogDedicatedServers, Log, TEXT("OnMatchCancelled Event Emitted"));
	/*FindMatch_Button->SetIsEnabled(true);
	bIsMatchmaking = false;
	OnMatchmakingStopped.Broadcast();*/
}

void UStartPage::OnMatchFailed()
{
	UE_LOG(LogDedicatedServers, Log, TEXT("OnMatchFailed Event Emitted"));
	FindMatch_Button->SetIsEnabled(true);
	bIsMatchmaking = false;
	OnMatchmakingStopped.Broadcast();
}
