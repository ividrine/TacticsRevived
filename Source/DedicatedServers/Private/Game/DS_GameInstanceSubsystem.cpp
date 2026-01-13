// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DS_GameInstanceSubsystem.h"
#include "DedicatedServers/DedicatedServers.h"


UDS_GameInstanceSubsystem::UDS_GameInstanceSubsystem()
{
	bGameLiftInitialized = false;
}

void UDS_GameInstanceSubsystem::InitGameLift()
{
	if (bGameLiftInitialized) return;

	#if WITH_GAMELIFT

		ParseCommandLineArgs();

		UE_LOG(LogDedicatedServers, Log, TEXT("Initializing the GameLift Server"));

		FGameLiftServerSDKModule* GameLiftSdkModule = 
			&FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK"));

		GameLiftSdkModule->InitSDK(ServerParameters);

		auto OnGameSession = [=](Aws::GameLift::Server::Model::GameSession gameSession)
		{
			FString GameSessionId = FString(gameSession.GetGameSessionId());
			UE_LOG(LogDedicatedServers, Log, TEXT("GameSession Initializing: %s"), *GameSessionId);
			GameLiftSdkModule->ActivateGameSession();
		};

		ProcessParameters.OnStartGameSession.BindLambda(OnGameSession);

		auto OnProcessTerminate = [=]()
		{
			UE_LOG(LogDedicatedServers, Log, TEXT("Game Server process is terminating."));
			GameLiftSdkModule->ProcessEnding();
		};

		ProcessParameters.OnTerminate.BindLambda(OnProcessTerminate);

		auto OnHealthCheck = []()
		{
			UE_LOG(LogDedicatedServers, Log, TEXT("Performing Health Check"));
			return true;
		};

		ProcessParameters.OnHealthCheck.BindLambda(OnHealthCheck);

		ProcessParameters.port = Port;

		TArray<FString> LogFiles;
		LogFiles.Add(TEXT("FPSTemplate/Saved/Logs/FPSTemplate.log"));
		ProcessParameters.logParameters = LogFiles;

		UE_LOG(LogDedicatedServers, Log, TEXT("Calling Process Ready."));
		GameLiftSdkModule->ProcessReady(ProcessParameters);

	#endif
		bGameLiftInitialized = true;
}

void UDS_GameInstanceSubsystem::ParseCommandLineArgs()
{
	//AuthToken returned from the "aws gamelift get-compute-auth-token" API. Note this will expire and require a new call to the API after 15 minutes.
	if (FParse::Value(FCommandLine::Get(), TEXT("-authtoken="), ServerParameters.m_authToken))
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("AUTH_TOKEN: %s"), *ServerParameters.m_authToken)
	}

	//The Host/compute-name of the GameLift Anywhere instance.
	if (FParse::Value(FCommandLine::Get(), TEXT("-hostid="), ServerParameters.m_hostId))
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("HOST_ID: %s"), *ServerParameters.m_hostId)
	}

	//The Anywhere Fleet ID.
	if (FParse::Value(FCommandLine::Get(), TEXT("-fleetid="), ServerParameters.m_fleetId))
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("FLEET_ID: %s"), *ServerParameters.m_fleetId)
	}

	//The WebSocket URL (GameLiftServiceSdkEndpoint).
	if (FParse::Value(FCommandLine::Get(), TEXT("-websocketurl="), ServerParameters.m_webSocketUrl))
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("WEBSOCKET_URL: %s"), *ServerParameters.m_webSocketUrl)
	}

	// Server port
	FString StringPort;
	if (FParse::Value(FCommandLine::Get(), TEXT("-port="), StringPort) && StringPort.IsNumeric())
	{
		LexFromString(Port, *StringPort);
		UE_LOG(LogDedicatedServers, Log, TEXT("PORT: %s"), *StringPort)
	}

	//The PID of the running process
	ServerParameters.m_processId = FString::Printf(TEXT("%d"), GetCurrentProcessId());
	UE_LOG(LogDedicatedServers, Log, TEXT("PID: %s"), *ServerParameters.m_processId);
}

