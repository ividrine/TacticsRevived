// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Chat/Chat.h"
#include "WebSocket/WebSocketManager.h"
#include "WebSocket/WebSocketMessageTypes.h"
#include "JsonObjectConverter.h"
#include "DedicatedServers/DedicatedServers.h"

void UChat::NativeConstruct()
{
	Super::NativeConstruct();

	WebSocketManager = GetGameInstance()->GetSubsystem<UWebSocketManager>();

	WebSocketManager->OnChatMessageReceived.AddDynamic(this, &UChat::ReceiveChatMessage);

	JoinRoom();

}

void UChat::NativeDestruct()
{
	
	// Cleanup code here
	if (WebSocketManager)
	{
		WebSocketManager->OnChatMessageReceived.RemoveDynamic(this, &UChat::ReceiveChatMessage);
		UE_LOG(LogTemp, Log, TEXT("UChat: Unbound OnChatMessageReceived delegate"));
	}

	Super::NativeDestruct();
    
}

UWebSocketManager* UChat::GetWebSocketManager()
{
	return WebSocketManager.Get();
}

void UChat::JoinRoom()
{
	if (Room.IsEmpty())
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("Chat missing room"));
		return;
	}

	FOutgoingRoomMessage payload = FOutgoingRoomMessage(Room);
	TSharedPtr<FJsonObject> OutMessage = MakeShareable(new FJsonObject);
	OutMessage->SetStringField("type", WebSocketMessageTypes::JoinRoom);
	OutMessage->SetObjectField("payload", FJsonObjectConverter::UStructToJsonObject(payload));
	// Serialize Out message to string
	FString OutString;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&OutString);
	FJsonSerializer::Serialize(OutMessage.ToSharedRef(), JsonWriter);

	// Send message
	WebSocketManager->SendMessage(OutString);
}

void UChat::ChatRoom(const FString& message)
{
	if (Room.IsEmpty())
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("Chat missing room"));
		return;
	}
	// Create message payload
	FOutgoingChatMessage payload = FOutgoingChatMessage(Room, message);

	// Build out message
	TSharedPtr<FJsonObject> OutMessage = MakeShareable(new FJsonObject);
	OutMessage->SetStringField("type", WebSocketMessageTypes::ChatRoom);
	OutMessage->SetObjectField("payload", FJsonObjectConverter::UStructToJsonObject(payload));

	// Serialize Out message to string
	FString OutString;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&OutString);
	FJsonSerializer::Serialize(OutMessage.ToSharedRef(), JsonWriter);

	// Send message
	WebSocketManager->SendMessage(OutString);

}

void UChat::LeaveRoom()
{
	FOutgoingRoomMessage payload = FOutgoingRoomMessage(Room);
	TSharedPtr<FJsonObject> OutMessage = MakeShareable(new FJsonObject);
	OutMessage->SetStringField("type", WebSocketMessageTypes::LeaveRoom);
	OutMessage->SetObjectField("payload", FJsonObjectConverter::UStructToJsonObject(payload));
	// Serialize Out message to string
	FString OutString;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&OutString);
	FJsonSerializer::Serialize(OutMessage.ToSharedRef(), JsonWriter);

	// Send message
	WebSocketManager->SendMessage(OutString);
}

void UChat::ReceiveChatMessage(const FString& sender, const FString& message)
{
	UFunction* Function = FindFunction(FName(TEXT("ReceiveMessage")));
	if (Function)
	{
		struct ReceiveParams
		{
			FString Sender;
			FString MessageText;
		};

		// Initialize the parameter struct
		ReceiveParams Params;
		Params.Sender = sender;
		Params.MessageText = message;

		ProcessEvent(Function, &Params);
	}
}
