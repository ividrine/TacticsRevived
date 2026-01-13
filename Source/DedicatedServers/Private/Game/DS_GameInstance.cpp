// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DS_GameInstance.h"
#include "WebSocket/WebSocketManager.h"
#include "GameFramework/GameUserSettings.h"

void UDS_GameInstance::Init()
{
	Super::Init();

	check(APIConfig);

	/*if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
	{
		Settings->SetFullscreenMode(EWindowMode::Fullscreen);
		Settings->SetScreenResolution(FIntPoint(GSystemResolution.ResX, GSystemResolution.ResY));
		Settings->ApplySettings(false);
	}*/
}

void UDS_GameInstance::Shutdown()
{
	Super::Shutdown();
}

TObjectPtr<UAPIConfig> UDS_GameInstance::GetAPIConfig() const
{
	return APIConfig;
}
