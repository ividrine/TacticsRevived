// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/Dashboard/Play/Start/StartManager.h"

#include "DedicatedServers/DedicatedServers.h"
#include "JsonObjectConverter.h"
#include "UI/HTTP/HttpRequestTypes.h"
#include "Interfaces/IHttpResponse.h"
#include "Data/API/APIConfig.h"
#include "Player/DSLocalPlayerSubsystem.h"

void UStartManager::ClearTicket()
{
	TicketId = "";
}

void UStartManager::StartMatchmaking()
{
	UDSLocalPlayerSubsystem* LocalPlayerSubsystem = GetDSLocalPlayerSubsystem();
	if (!IsValid(LocalPlayerSubsystem)) return;

	check(APIConfig);

	const FString ApiUrl = APIConfig->GetAPIEndpoint("match/start");
	FString AccessToken = LocalPlayerSubsystem->GetAuthResult().access.token;

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &UStartManager::StartMatchmaking_Response);
	Request->SetURL(ApiUrl);
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetHeader("Authorization", "Bearer " + AccessToken);
	Request->ProcessRequest();
}

void UStartManager::StartMatchmaking_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!Response || !bWasSuccessful) {

		return;
	}

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{

		if (!IsSuccess(Response)) {
			FString ErrorMessage;
			if (JsonObject->TryGetStringField("message", ErrorMessage))
			{
				OnMatchStartError.Broadcast(ErrorMessage);
			}
			else
			{
				OnMatchStartError.Broadcast(HTTPStatusMessages::SomethingWentWrong);
			}

			return;
		}

		FStartMatchResponse StartMatchResponse;
		FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &StartMatchResponse);
		StartMatchResponse.Dump();

		TicketId = StartMatchResponse.ticketId;
		OnMatchStartSuccess.Broadcast();
	}
}

void UStartManager::StopMatchmaking()
{

	if (TicketId.IsEmpty())
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("Missing ticket id - cancel request not sent"));
	}

	UDSLocalPlayerSubsystem* LocalPlayerSubsystem = GetDSLocalPlayerSubsystem();
	if (!IsValid(LocalPlayerSubsystem)) return;
	check(APIConfig);

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	const FString ApiUrl = APIConfig->GetAPIEndpoint("match/stop");
	FString AccessToken = LocalPlayerSubsystem->GetAuthResult().access.token;

	Request->OnProcessRequestComplete().BindUObject(this, &UStartManager::StopMatchmaking_Response);
	Request->SetURL(ApiUrl);
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetHeader("Authorization", "Bearer " + AccessToken);

	TMap<FString, FString> Params = {
		{ TEXT("ticketId"), TicketId },
	};
	const FString Content = SerializeJsonContent(Params);

	Request->SetContentAsString(Content);
	Request->ProcessRequest();
}

void UStartManager::StopMatchmaking_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!Response || !bWasSuccessful) {

		return;
	}

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	if (IsSuccess(Response))
	{
		OnMatchStopSuccess.Broadcast();
	}
	else if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		FString ErrorMessage;
		if (JsonObject->TryGetStringField("message", ErrorMessage))
		{
			OnMatchStopError.Broadcast(ErrorMessage);
		}
		else
		{
			OnMatchStopError.Broadcast(HTTPStatusMessages::SomethingWentWrong);
		}
	}
	else
	{
		OnMatchStopError.Broadcast(HTTPStatusMessages::SomethingWentWrong);
	}
}

void UStartManager::AcceptMatch(bool accepted)
{
	if (TicketId.IsEmpty())
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("Missing ticket id - accept request not sent"));
	}

	UDSLocalPlayerSubsystem* LocalPlayerSubsystem = GetDSLocalPlayerSubsystem();
	if (!IsValid(LocalPlayerSubsystem)) return;
	check(APIConfig);

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	const FString ApiUrl = APIConfig->GetAPIEndpoint("match/accept");
	FString AccessToken = LocalPlayerSubsystem->GetAuthResult().access.token;

	Request->OnProcessRequestComplete().BindUObject(this, &UStartManager::AcceptMatch_Response);
	Request->SetURL(ApiUrl);
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetHeader("Authorization", "Bearer " + AccessToken);

	

	FAcceptMatchRequest AcceptMatchRequest;

	AcceptMatchRequest.ticketId = TicketId;
	AcceptMatchRequest.accept = accepted;

	FString Content;
	if (FJsonObjectConverter::UStructToJsonObjectString(FAcceptMatchRequest::StaticStruct(), &AcceptMatchRequest, Content, 0, 0))
	{
		Request->SetContentAsString(Content);
		Request->ProcessRequest();
	}
	else
	{
		UE_LOG(LogDedicatedServers, Error, TEXT("Failed to serialize Struct to JSON string."));
	}
	
}



void UStartManager::AcceptMatch_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!Response || !bWasSuccessful) {

		return;
	}

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{

		if (!IsSuccess(Response)) {
			FString ErrorMessage;
			if (JsonObject->TryGetStringField("message", ErrorMessage))
			{
				OnMatchAcceptError.Broadcast(ErrorMessage);
			}
			else
			{
				OnMatchAcceptError.Broadcast(HTTPStatusMessages::SomethingWentWrong);
			}

			return;
		}

		OnMatchAcceptSuccess.Broadcast();
	}
}
