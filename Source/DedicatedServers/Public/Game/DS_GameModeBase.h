// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameLiftServerSDK.h"
#include "DS_GameModeBase.generated.h"

class UDS_GameInstanceSubsystem;
class ADSPlayerController;

UCLASS()
class DEDICATEDSERVERS_API ADS_GameModeBase : public AGameMode
{
	GENERATED_BODY()

public:
	// Class Overrides
	virtual void Logout(AController* Exiting) override;
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	// Called from client via RPC when player is ready
	void MarkClientReady(ADSPlayerController* PlayerController);
	
protected:
	// Class Overrides
	virtual void BeginPlay() override;
	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal = L"") override;

	// Gamelift and Session Management
	void InitGameLift();
	void RemovePlayerSession(AController* Exiting);
	bool GetPlayerSessions(TArray<Aws::GameLift::Server::Model::PlayerSession>& OutSessions, int32& OutCount);
	void TryAcceptPlayerSession(const FString& PlayerSessionId, const FString& Username, FString& OutErrorMessage);
	void CheckIfAllPlayersJoined();

	TArray<ADSPlayerController*> ConnectedPlayers;
	int PlayerReadyCount;
	bool bIsReadyToStart = false;

private:
	UPROPERTY()
	TObjectPtr<UDS_GameInstanceSubsystem> DSGameInstanceSubsystem;	
};
