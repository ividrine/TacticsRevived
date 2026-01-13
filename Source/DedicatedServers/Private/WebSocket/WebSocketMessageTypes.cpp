// Fill out your copyright notice in the Description page of Project Settings.


#include "WebSocket/WebSocketMessageTypes.h"
#include "DedicatedServers/DedicatedServers.h"

namespace WebSocketMessageTypes
{
	// Incoming Types
	const FString MatchmakingSearching{ TEXT("MatchmakingSearching") };
	const FString PotentialMatchCreated{ TEXT("PotentialMatchCreated") };
	const FString AcceptMatch{ TEXT("AcceptMatch") };
	const FString AcceptMatchCompleted{ TEXT("AcceptMatchCompleted") };
	const FString MatchmakingSucceeded{ TEXT("MatchmakingSucceeded") };
	const FString MatchmakingTimedOut{ TEXT("MatchmakingTimedOut") };
	const FString MatchmakingCancelled{ TEXT("MatchmakingCancelled") };
	const FString MatchmakingFailed{ TEXT("MatchmakingFailed") };

	const FString Chat{ TEXT("chat") };
	
	// Outgoing Types
	const FString JoinRoom{ TEXT("join-room") };
	const FString ChatRoom{ TEXT("chat-room") };
	const FString LeaveRoom{ TEXT("leave-room") };
}

namespace RoomTypes
{
	const FString GlobalRoom{ TEXT("global") };

	// These will be suffixed with ids
	const FString LobbyRoom{ TEXT("lobby-") };
	const FString GameRoom{ TEXT("game-session-") };
}

namespace MatchmakingTypes
{
	const FString MatchmakingSearching{ TEXT("MatchmakingSearching") };
	const FString PotentialMatchCreated{ TEXT("PotentialMatchCreated") };
	const FString AcceptMatch{ TEXT("AcceptMatch") };
	const FString AcceptMatchCompleted{ TEXT("AcceptMatchCompleted") };
	const FString MatchmakingSucceeded{ TEXT("MatchmakingSucceeded") };
	const FString MatchmakingTimedOut{ TEXT("MatchmakingTimedOut") };
	const FString MatchmakingCancelled{ TEXT("MatchmakingCancelled") };
	const FString MatchmakingFailed{ TEXT("MatchmakingFailed") };
}

void FIncomingChatMessage::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("ChatMessage:"));
	UE_LOG(LogDedicatedServers, Log, TEXT("   sender: %s"), *sender);
	UE_LOG(LogDedicatedServers, Log, TEXT("   message: %s"), *message);
}

void FMatchCreatedMessageDetail::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("MatchFoundMessageDetail:"));
	UE_LOG(LogDedicatedServers, Log, TEXT("   acceptanceTimeout: %d"), acceptanceTimeout);
	UE_LOG(LogDedicatedServers, Log, TEXT("   acceptanceRequired: %s"), acceptanceRequired ? TEXT("true") : TEXT("false"));
}

void FGameSessionInfo::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("GameSessionInfo:"));
	UE_LOG(LogDedicatedServers, Log, TEXT("   ipAddress: %s"), *ipAddress);
	UE_LOG(LogDedicatedServers, Log, TEXT("   port: %d"), port);
}

void FPlayerInfo::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("PlayerInfo:"));
	UE_LOG(LogDedicatedServers, Log, TEXT("   playerId: %s"), *playerId);
	UE_LOG(LogDedicatedServers, Log, TEXT("   avatarUrl: %s"), *iconId);
}

void FMatchSucceededMessageDetail::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("MatchSucceededMessageDetail:"));
	UE_LOG(LogDedicatedServers, Log, TEXT("   gameSessionInfo:"));
	gameSessionInfo.Dump();
	UE_LOG(LogDedicatedServers, Log, TEXT("   players:"));
	for (const auto& Player : players)
	{
		Player.Dump();
	}
}
void FOutgoingChatMessage::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("OutgoingGlobalChatMessage:"));
	UE_LOG(LogDedicatedServers, Log, TEXT("   message: %s"), *message);
	UE_LOG(LogDedicatedServers, Log, TEXT("   room: %s"), *room);
}

void FOutgoingRoomMessage::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("   room: %s"), *room);
}

void FAcceptMatchCompletedDetail::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("   acceptance: %s"), *acceptance);
}

void FAcceptMatchDetail::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("   accepted: %s"), (accepted ? TEXT("true") : TEXT("false")));
}
