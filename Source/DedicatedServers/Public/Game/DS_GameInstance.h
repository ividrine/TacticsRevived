// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DS_GameInstance.generated.h"

class UWebSocketManager;
class UAPIConfig;

UCLASS(Blueprintable)
class DEDICATEDSERVERS_API UDS_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	
public:
	virtual void Init() override;
	virtual void Shutdown() override;

	TObjectPtr<UAPIConfig> GetAPIConfig() const;
	TObjectPtr<UWebSocketManager> GetWebSocketManager() const { return WebSocketManager; }

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAPIConfig> APIConfig;

private:
	UPROPERTY()
	TObjectPtr<UWebSocketManager> WebSocketManager;

	
};
