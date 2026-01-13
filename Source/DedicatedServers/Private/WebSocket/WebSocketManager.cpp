// Fill out your copyright notice in the Description page of Project Settings.


#include "WebSocket/WebSocketManager.h"
#include "Player/DSLocalPlayerSubsystem.h"
#include "DedicatedServers/DedicatedServers.h"
#include "WebSocketsModule.h"
#include "IWebSocket.h"
#include "JsonObjectConverter.h"
#include "Data/API/APIConfig.h"
#include "Game/DS_GameInstance.h"
#include "Data/API/APIData.h"

UDSLocalPlayerSubsystem* UWebSocketManager::GetDSLocalPlayerSubsystem() const
{
	APlayerController* LocalPlayerController = GEngine->GetFirstLocalPlayerController(GetWorld());
	if (IsValid(LocalPlayerController))
	{
		ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(LocalPlayerController->Player);
		if (IsValid(LocalPlayer))
		{
			return LocalPlayer->GetSubsystem<UDSLocalPlayerSubsystem>();
		}
	}
	return nullptr;
}

void UWebSocketManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UDS_GameInstance* GI = Cast<UDS_GameInstance>(GetGameInstance());
	if (IsValid(GI))
	{
		APIConfig = GI->GetAPIConfig();
	}
}

void UWebSocketManager::Deinitialize()
{
	Disconnect();
	Super::Deinitialize();
}

bool UWebSocketManager::IsConnected()
{
	if (WebSocket)
	{
		return WebSocket->IsConnected();
	}
	else
	{
		return false;
	}
}

void UWebSocketManager::Connect()
{

	// Todo maybe move this somewhere else
	if (!FModuleManager::Get().IsModuleLoaded("WebSockets"))
	{
		FModuleManager::Get().LoadModule("WebSockets");
	}

	FString Url = APIConfig->GetWebSocketUrl();

	if (Url.IsEmpty())
	{
		UE_LOG(LogDedicatedServers, Warning, TEXT("WebSocket URL is empty. Cannot connect."));
		return;
	}

	UDSLocalPlayerSubsystem* LocalPlayerSubsystem = GetDSLocalPlayerSubsystem();

	if (!IsValid(LocalPlayerSubsystem)) 
	{ 
		UE_LOG(LogDedicatedServers, Warning, TEXT("Invalid DS local player subsystem."));
		return;
	}

	FString token = LocalPlayerSubsystem->GetAuthResult().access.token;

	const TMap<FString, FString>& headers = {
		{ TEXT("Authorization"), TEXT("Bearer " + token) }
	};

	WebSocket = FWebSocketsModule::Get().CreateWebSocket(Url, "", headers);
	WebSocket->OnConnected().AddUObject(this, &UWebSocketManager::OnConnected);
	WebSocket->OnMessage().AddUObject(this, &UWebSocketManager::OnMessage);
	WebSocket->OnClosed().AddUObject(this, &UWebSocketManager::OnClosed);
	WebSocket->OnConnectionError().AddUObject(this, &UWebSocketManager::OnConnectionError);
	WebSocket->Connect();
}

void UWebSocketManager::Disconnect()
{
	if (WebSocket.IsValid() && WebSocket->IsConnected())
	{
		WebSocket->Close();
	}
}

void UWebSocketManager::SendMessage(FString& Message)
{
	if (WebSocket.IsValid() && WebSocket->IsConnected())
	{
		WebSocket->Send(Message);
	}
	else
	{
		UE_LOG(LogDedicatedServers, Warning, TEXT("Cannot send chat message: WebSocket not connected"));
	}
}

void UWebSocketManager::OnConnected() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("WebSocket connected"));
	OnWebSocketConnected.Broadcast();
}

void UWebSocketManager::OnMessage(const FString& Message) const
{

	UE_LOG(LogDedicatedServers, Log, TEXT("WebSocket message received: %s"), *Message);

	TSharedPtr<FJsonObject> Data = MakeShareable(new FJsonObject);
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Message);

	if (!FJsonSerializer::Deserialize(JsonReader, Data) || !Data.IsValid())
	{
		UE_LOG(LogDedicatedServers, Warning, TEXT("Failed to parse WebSocket message as JSON"));
		return;
	}

	FString Type;
	if (!Data->TryGetStringField("type", Type) || Type.IsEmpty())
	{
		UE_LOG(LogDedicatedServers, Warning, TEXT("WebSocket message missing 'type' field"));
		return;
	}

	const TSharedPtr<FJsonObject>* Payload;
	Data->TryGetObjectField("payload", Payload);

	/* Broadcast chat message event */

	if (Type == WebSocketMessageTypes::Chat)
	{
		FIncomingChatMessage ChatMessage;

		if (FJsonObjectConverter::JsonObjectToUStruct(Payload->ToSharedRef(), &ChatMessage))
		{
			ChatMessage.Dump();
			OnChatMessageReceived.Broadcast(ChatMessage.sender, ChatMessage.message);
		}
	}

	/* Broadcast matchmaking event */

	else if (Type == WebSocketMessageTypes::MatchmakingSearching)
	{
		OnMatchSearching.Broadcast();
	}
	else if (Type == WebSocketMessageTypes::PotentialMatchCreated)
	{
		FMatchCreatedMessageDetail MatchDetail;
		if (FJsonObjectConverter::JsonObjectToUStruct(Payload->ToSharedRef(), &MatchDetail))
		{
			MatchDetail.Dump();
			OnMatchCreated.Broadcast(MatchDetail);
		}
	}
	else if (Type == WebSocketMessageTypes::AcceptMatch)
	{
		FAcceptMatchDetail MatchDetail;
		if (FJsonObjectConverter::JsonObjectToUStruct(Payload->ToSharedRef(), &MatchDetail))
		{
			MatchDetail.Dump();
			OnAcceptMatch.Broadcast(MatchDetail);
		}
	}
	else if (Type == WebSocketMessageTypes::AcceptMatchCompleted)
	{
		FAcceptMatchCompletedDetail MatchDetail;
		if (FJsonObjectConverter::JsonObjectToUStruct(Payload->ToSharedRef(), &MatchDetail))
		{
			MatchDetail.Dump();
			OnAcceptMatchCompleted.Broadcast(MatchDetail);
		}
	}
	else if (Type == WebSocketMessageTypes::MatchmakingSucceeded)
	{
		FMatchSucceededMessageDetail MatchDetail;

		if (FJsonObjectConverter::JsonObjectToUStruct(Payload->ToSharedRef(), &MatchDetail))
		{
			MatchDetail.Dump();
			OnMatchSucceeded.Broadcast(MatchDetail);
		}
	}
	else if (Type == WebSocketMessageTypes::MatchmakingTimedOut)
	{
		OnMatchTimedOut.Broadcast();
	}
	else if (Type == WebSocketMessageTypes::MatchmakingCancelled)
	{
		OnMatchCancelled.Broadcast();
	}
	else if (Type == WebSocketMessageTypes::MatchmakingFailed)
	{
		OnMatchFailed.Broadcast();
	}
	
}

void UWebSocketManager::OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean) const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("WebSocket closed. StatusCode: %d, Reason: %s, bWasClean: %s"), StatusCode, *Reason, bWasClean ? TEXT("true") : TEXT("false"));
}

void UWebSocketManager::OnConnectionError(const FString& Error) const
{
	UE_LOG(LogDedicatedServers, Error, TEXT("WebSocket connection error: %s"), *Error);
}

