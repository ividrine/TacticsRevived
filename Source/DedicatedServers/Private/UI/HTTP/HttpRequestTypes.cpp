#include "UI/HTTP/HttpRequestTypes.h"
#include "DedicatedServers/DedicatedServers.h"

namespace HTTPStatusMessages
{
	const FString ConnectionError{ TEXT("Unable to connect to server.") };
	const FString SomethingWentWrong{ TEXT("Server error. Something went wrong.") };
}

void FleetMetaData::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("MetaData:"));

	UE_LOG(LogDedicatedServers, Log, TEXT("httpStatusCode: %d"), httpStatusCode);
	UE_LOG(LogDedicatedServers, Log, TEXT("requestId: %s"), *requestId);
	UE_LOG(LogDedicatedServers, Log, TEXT("attempts: %d"), attempts);
	UE_LOG(LogDedicatedServers, Log, TEXT("totalRetryDelay: %f"), totalRetryDelay);
}

void FleetsResponse::Dump() const
{
	for (const FString& FleetId : FleetIds)
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("FleetId: %s"), *FleetId);
	}
}

void FDSGameSession::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("GameSession:"));
	UE_LOG(LogDedicatedServers, Log, TEXT("  CreationTime: %s"), *CreationTime);
	UE_LOG(LogDedicatedServers, Log, TEXT("  CreatorId: %s"), *CreatorId);
	UE_LOG(LogDedicatedServers, Log, TEXT("  CurrentPlayerSessionCount: %d"), CurrentPlayerSessionCount);
	UE_LOG(LogDedicatedServers, Log, TEXT("  DnsName: %s"), *DnsName);
	UE_LOG(LogDedicatedServers, Log, TEXT("  FleetArn: %s"), *FleetArn);
	UE_LOG(LogDedicatedServers, Log, TEXT("  FleetId: %s"), *FleetId);

	UE_LOG(LogDedicatedServers, Log, TEXT("  GameProperties:"));
	for (const auto& GameProperty : GameProperties)
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("    %s: %s"), *GameProperty.Key, *GameProperty.Value);
	}

	UE_LOG(LogDedicatedServers, Log, TEXT("  GameSessionData: %s"), *GameSessionData);
	UE_LOG(LogDedicatedServers, Log, TEXT("  GameSessionId: %s"), *GameSessionId);
	UE_LOG(LogDedicatedServers, Log, TEXT("  IpAddress: %s"), *IpAddress);
	UE_LOG(LogDedicatedServers, Log, TEXT("  Location: %s"), *Location);
	UE_LOG(LogDedicatedServers, Log, TEXT("  MatchmakerData: %s"), *MatchmakerData);
	UE_LOG(LogDedicatedServers, Log, TEXT("  MaximumPlayerSessionCount: %d"), MaximumPlayerSessionCount);
	UE_LOG(LogDedicatedServers, Log, TEXT("  Name: %s"), *Name);
	UE_LOG(LogDedicatedServers, Log, TEXT("  PlayerSessionCreationPolicy: %s"), *PlayerSessionCreationPolicy);
	UE_LOG(LogDedicatedServers, Log, TEXT("  Port: %d"), Port);
	UE_LOG(LogDedicatedServers, Log, TEXT("  Status: %s"), *Status);
	UE_LOG(LogDedicatedServers, Log, TEXT("  StatusReason: %s"), *StatusReason);
	UE_LOG(LogDedicatedServers, Log, TEXT("  TerminationTime: %s"), *TerminationTime);
}

void FDSPlayerSession::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("PlayerSession:"));
	UE_LOG(LogDedicatedServers, Log, TEXT("  CreationTime: %s"), *CreationTime);
	UE_LOG(LogDedicatedServers, Log, TEXT("  DnsName: %s"), *DnsName);
	UE_LOG(LogDedicatedServers, Log, TEXT("  FleetArn: %s"), *FleetArn);
	UE_LOG(LogDedicatedServers, Log, TEXT("  FleetId: %s"), *FleetId);
	UE_LOG(LogDedicatedServers, Log, TEXT("  GameSessionId: %s"), *GameSessionId);
	UE_LOG(LogDedicatedServers, Log, TEXT("  IpAddress: %s"), *IpAddress);
	UE_LOG(LogDedicatedServers, Log, TEXT("  PlayerData: %s"), *PlayerData);
	UE_LOG(LogDedicatedServers, Log, TEXT("  PlayerId: %s"), *PlayerId);
	UE_LOG(LogDedicatedServers, Log, TEXT("  PlayerSessionId: %s"), *PlayerSessionId);
	UE_LOG(LogDedicatedServers, Log, TEXT("  Port: %d"), Port);
	UE_LOG(LogDedicatedServers, Log, TEXT("  Status: %s"), *Status);
	UE_LOG(LogDedicatedServers, Log, TEXT("  TerminationTime: %s"), *TerminationTime);
}



void FDSNewDeviceMetaData::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("NewDeviceMetadata:"));
	UE_LOG(LogDedicatedServers, Log, TEXT("   DeviceGroupKey: %s"), *DeviceGroupKey);
	UE_LOG(LogDedicatedServers, Log, TEXT("   DeviceKey: %s"), *DeviceKey);
}

void FDSAuthenticationResult::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("AuthenticationResult:"));
	UE_LOG(LogDedicatedServers, Log, TEXT("   AccessToken: %s"), *access.token);
	UE_LOG(LogDedicatedServers, Log, TEXT("   AccessExpiresIn: %s"), *access.expires);
	UE_LOG(LogDedicatedServers, Log, TEXT("   RefreshToken: %s"), *refresh.token);
	UE_LOG(LogDedicatedServers, Log, TEXT("   RefreshExpiresIn: %s"), *refresh.expires);
}



void FDSInitiateAuthResponse::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("FDSSignUpResponse Details:"));
	tokens.Dump();
}

void FDSToken::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("  token: %s"), *token);
	UE_LOG(LogDedicatedServers, Log, TEXT("  expires: %s"), *expires);
}

void FStartMatchResponse::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("  ticketId: %s"), *ticketId);
	UE_LOG(LogDedicatedServers, Log, TEXT("  status: %s"), *status);
	UE_LOG(LogDedicatedServers, Log, TEXT("  estimatedWaitTime: %s"), *estimatedWaitTime);
}

void FAcceptMatchRequest::Dump() const
{
}
