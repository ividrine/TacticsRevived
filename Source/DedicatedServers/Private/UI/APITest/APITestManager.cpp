// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/APITest/APITestManager.h"
#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "UI/HTTP/HttpRequestTypes.h"
#include "Data/API/APIConfig.h"
#include "Interfaces/IHttpResponse.h"

void UAPITestManager::GetFleets()
{

	check(APIConfig);

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UAPITestManager::GetFleets_Response);

	const FString APIUrl = APIConfig->GetAPIEndpoint("fleets");

	Request->SetURL(APIUrl);
	Request->SetVerb("GET");
	Request->SetHeader("Content-Type", "application/json");
	Request->ProcessRequest();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Fleets request made");

}

void UAPITestManager::GetFleets_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{

	if (!bWasSuccessful)
	{
		OnFleetsResponseReceived.Broadcast(FleetsResponse(), false);
		return;
	}

	TSharedPtr<FJsonObject> Data;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	if (FJsonSerializer::Deserialize(JsonReader, Data))
	{
		FleetsResponse Fleets;
		FJsonObjectConverter::JsonObjectToUStruct(Data.ToSharedRef(), &Fleets);
		OnFleetsResponseReceived.Broadcast(Fleets, true);
	}
}
