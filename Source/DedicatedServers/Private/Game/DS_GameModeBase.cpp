// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DS_GameModeBase.h"

#include "Game/DS_GameInstanceSubsystem.h"
#include "DedicatedServers/DedicatedServers.h"
#include "Player/DSPlayerController.h"
#include "Kismet/GameplayStatics.h"

/* Lifecycle Function Overrides */

void ADS_GameModeBase::BeginPlay()
{
	Super::BeginPlay();

	InitGameLift();
}

void ADS_GameModeBase::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	const FString PlayerSessionId = UGameplayStatics::ParseOption(Options, TEXT("PlayerSessionId"));
	const FString Username = UGameplayStatics::ParseOption(Options, TEXT("Username"));

	TryAcceptPlayerSession(PlayerSessionId, Username, ErrorMessage);
}

FString ADS_GameModeBase::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal)
{
	FString InitializedString = Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);

	const FString PlayerSessionId = UGameplayStatics::ParseOption(Options, TEXT("PlayerSessionId"));
	const FString Username = UGameplayStatics::ParseOption(Options, TEXT("Username"));

	if (ADSPlayerController* DSPlayerController = Cast<ADSPlayerController>(NewPlayerController); IsValid(DSPlayerController))
	{
		DSPlayerController->PlayerSessionId = PlayerSessionId;
		DSPlayerController->Username = Username;
	}

	return InitializedString;
}

void ADS_GameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ADSPlayerController* PC = Cast<ADSPlayerController>(NewPlayer);
	ConnectedPlayers.Add(PC);

	CheckIfAllPlayersJoined();

	
}

void ADS_GameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	RemovePlayerSession(Exiting);
	ADSPlayerController* PC = Cast<ADSPlayerController>(Exiting);
	ConnectedPlayers.Remove(PC);
}

/* Gamelift session management */

void ADS_GameModeBase::InitGameLift()
{
	if (UGameInstance* GameInstance = GetGameInstance(); IsValid(GameInstance))
	{
		if (DSGameInstanceSubsystem = GameInstance->GetSubsystem<UDS_GameInstanceSubsystem>(); IsValid(DSGameInstanceSubsystem))
		{
			DSGameInstanceSubsystem->InitGameLift();
		}
	}
}

void ADS_GameModeBase::RemovePlayerSession(AController* Exiting)
{
	ADSPlayerController* DSPlayerController = Cast<ADSPlayerController>(Exiting);
	if (!IsValid(DSPlayerController)) return;

	#if WITH_GAMELIFT
		const FString& PlayerSessionId = DSPlayerController->PlayerSessionId;
		if (!PlayerSessionId.IsEmpty())
		{
			Aws::GameLift::Server::RemovePlayerSession(TCHAR_TO_ANSI(*PlayerSessionId));
		}
	#endif
}

bool ADS_GameModeBase::GetPlayerSessions(TArray<Aws::GameLift::Server::Model::PlayerSession>& OutSessions, int32& OutCount)
{
	#if WITH_GAMELIFT

		auto gameSessionOutcome = Aws::GameLift::Server::GetGameSessionId();

		if (!gameSessionOutcome.IsSuccess())
		{
			UE_LOG(LogTemp, Error, TEXT("GetGameSessionId failed: %s"), ANSI_TO_TCHAR(gameSessionOutcome.GetError().GetErrorMessage()));
			return false;
		}

		Aws::GameLift::Server::Model::DescribePlayerSessionsRequest req;
		req.SetGameSessionId(gameSessionOutcome.GetResult());

		auto playerSessionsOutcome = Aws::GameLift::Server::DescribePlayerSessions(req);

		if (!playerSessionsOutcome.IsSuccess())
		{
			UE_LOG(LogTemp, Error, TEXT("DescribePlayerSessions failed: %s"), ANSI_TO_TCHAR(playerSessionsOutcome.GetError().GetErrorMessage()));
			return false;
		}

		auto Result = playerSessionsOutcome.GetResult();
		OutCount = 0;
		const Aws::GameLift::Server::Model::PlayerSession* RawSessions = Result.GetPlayerSessions(OutCount);
		if (RawSessions && OutCount > 0)
		{
			OutSessions.Reserve(OutCount);
			for (int32 i = 0; i < OutCount; ++i)
			{
				OutSessions.Add(RawSessions[i]);
			}
		}
		return true;
	#else
		return false;
	#endif
}

void ADS_GameModeBase::TryAcceptPlayerSession(const FString& PlayerSessionId, const FString& Username, FString& OutErrorMessage)
{

#if WITH_GAMELIFT

	if (PlayerSessionId.IsEmpty() || Username.IsEmpty())
	{
		OutErrorMessage = TEXT("PlayerSessionId and/or Username invalid.");
		return;
	}

	Aws::GameLift::Server::Model::DescribePlayerSessionsRequest Req;
	Req.SetPlayerSessionId(TCHAR_TO_ANSI(*PlayerSessionId));

	const auto& Outcome = Aws::GameLift::Server::DescribePlayerSessions(Req);
	if (!Outcome.IsSuccess())
	{
		OutErrorMessage = TEXT("DescribePlayerSessions failed.");
		return;
	}

	const auto& Result = Outcome.GetResult();
	int32 Count = 0;
	const Aws::GameLift::Server::Model::PlayerSession* PlayerSessions = Result.GetPlayerSessions(Count);
	if (PlayerSessions == nullptr || Count == 0)
	{
		OutErrorMessage = TEXT("GetPlayerSessions failed.");
		return;
	}

	for (int32 i = 0; i < Count; i++)
	{
		const Aws::GameLift::Server::Model::PlayerSession& PlayerSession = PlayerSessions[i];
		if (!Username.Equals(PlayerSession.GetPlayerId())) continue;
		if (PlayerSession.GetStatus() != Aws::GameLift::Server::Model::PlayerSessionStatus::RESERVED)
		{
			OutErrorMessage = FString::Printf(TEXT("Session for %s not RESERVED; Fail PreLogin."), *Username);
			return;
		}

		const auto& AcceptPlayerSessionOutcome = Aws::GameLift::Server::AcceptPlayerSession(TCHAR_TO_ANSI(*PlayerSessionId));
		OutErrorMessage = AcceptPlayerSessionOutcome.IsSuccess() ? "" : FString::Printf(TEXT("Failed to accept player session for %s"), *Username);
	}

#endif
}

void ADS_GameModeBase::CheckIfAllPlayersJoined()
{
#if WITH_GAMELIFT

	TArray<Aws::GameLift::Server::Model::PlayerSession> Sessions;
	int32 Count;
	if (!GetPlayerSessions(Sessions, Count))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to verify player sessions."));
		return;
	}

	bool HasReserved = false;

	for (const auto& Session : Sessions)
	{
		if (Session.GetStatus() == Aws::GameLift::Server::Model::PlayerSessionStatus::RESERVED)
		{
			HasReserved = true;
		}
	}

	if (HasReserved)
	{
		UE_LOG(LogTemp, Log, TEXT("Waiting on more players to join..."));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("All %d players joined. Starting main game."), Sessions.Num());

	bIsReadyToStart = true;
#else
	bIsReadyToStart = true;
#endif
}

void ADS_GameModeBase::MarkClientReady(ADSPlayerController* PlayerController)
{
	PlayerReadyCount++;
}



