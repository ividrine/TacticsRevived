// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HTTP/HttpRequestManager.h"
#include "Interfaces/IHttpResponse.h"
#include "Game/DS_GameInstance.h"
#include "Player/DSLocalPlayerSubsystem.h"
#include "Kismet/GameplayStatics.h" 
#include "DedicatedServers/DedicatedServers.h"

UHttpRequestManager::UHttpRequestManager()
{
	UDS_GameInstance* GI = Cast<UDS_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (IsValid(GI))
	{
		APIConfig = GI->GetAPIConfig();
	}
}

UDSLocalPlayerSubsystem* UHttpRequestManager::GetDSLocalPlayerSubsystem() const
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

bool UHttpRequestManager::IsSuccess(FHttpResponsePtr Response)
{
	return Response->GetResponseCode() < 400;
}

FString UHttpRequestManager::SerializeJsonContent(const TMap<FString, FString>& Params)
{
	TSharedPtr<FJsonObject> ContentJsonObject = MakeShareable(new FJsonObject);

	for (const auto& Param : Params)
	{
		ContentJsonObject->SetStringField(Param.Key, Param.Value);
	}

	FString Content;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(ContentJsonObject.ToSharedRef(), JsonWriter);

	return Content;
}