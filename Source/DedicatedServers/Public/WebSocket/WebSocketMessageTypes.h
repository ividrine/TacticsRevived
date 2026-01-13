#pragma once

#include "WebSocketMessageTypes.generated.h"


namespace WebSocketMessageTypes
{
	// Incoming Types
	extern DEDICATEDSERVERS_API const FString MatchmakingSearching;
	extern DEDICATEDSERVERS_API const FString PotentialMatchCreated;
	extern DEDICATEDSERVERS_API const FString AcceptMatch;
	extern DEDICATEDSERVERS_API const FString AcceptMatchCompleted;
	extern DEDICATEDSERVERS_API const FString MatchmakingSucceeded;
	extern DEDICATEDSERVERS_API const FString MatchmakingTimedOut;
	extern DEDICATEDSERVERS_API const FString MatchmakingCancelled;
	extern DEDICATEDSERVERS_API const FString MatchmakingFailed;

	extern DEDICATEDSERVERS_API const FString Chat;

	// Outgoing Types
	extern DEDICATEDSERVERS_API const FString JoinRoom;
	extern DEDICATEDSERVERS_API const FString ChatRoom;
	extern DEDICATEDSERVERS_API const FString LeaveRoom;
}

namespace RoomTypes
{
	extern DEDICATEDSERVERS_API const FString GlobalRoom;
	extern DEDICATEDSERVERS_API const FString LobbyRoom;
	extern DEDICATEDSERVERS_API const FString GameRoom;
}

namespace MatchmakingTypes
{
	extern DEDICATEDSERVERS_API const FString MatchmakingSearching;
	extern DEDICATEDSERVERS_API const FString PotentialMatchCreated;
	extern DEDICATEDSERVERS_API const FString AcceptMatch;
	extern DEDICATEDSERVERS_API const FString AcceptMatchCompleted;
	extern DEDICATEDSERVERS_API const FString MatchmakingSucceeded;
	extern DEDICATEDSERVERS_API const FString MatchmakingTimedOut;
	extern DEDICATEDSERVERS_API const FString MatchmakingCancelled;
	extern DEDICATEDSERVERS_API const FString MatchmakingFailed;
}

// Nested structs

USTRUCT(BlueprintType)
struct FGameSessionInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FString ipAddress{};

	UPROPERTY()
	int port{};

	void Dump() const;
};

USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FString playerId{};

	UPROPERTY()
	FString iconId{};

	void Dump() const;
};

// Incoming messages from server

USTRUCT(BlueprintType)
struct FIncomingChatMessage
{
	GENERATED_BODY()

	UPROPERTY()
	FString message{};

	UPROPERTY()
	FString sender{};

	void Dump() const;
};

USTRUCT(BlueprintType)
struct FMatchCreatedMessageDetail
{
	GENERATED_BODY()

	UPROPERTY()
	int acceptanceTimeout{};
	UPROPERTY()
	bool acceptanceRequired{};

	void Dump() const;
};

USTRUCT(BlueprintType)
struct FMatchSucceededMessageDetail
{
	GENERATED_BODY()

	UPROPERTY()
	FString playerId{};

	UPROPERTY()
	FString playerSessionId{};

	UPROPERTY()
	FGameSessionInfo gameSessionInfo{};

	UPROPERTY()
	TArray<FPlayerInfo> players{};

	void Dump() const;
};

USTRUCT(BlueprintType)
struct FAcceptMatchCompletedDetail
{
	GENERATED_BODY()

	UPROPERTY()
	FString acceptance{};

	void Dump() const;
};

USTRUCT(BlueprintType)
struct FAcceptMatchDetail
{
	GENERATED_BODY()

	UPROPERTY()
	bool accepted{};

	void Dump() const;
};

// Outgoing Messages

USTRUCT(BlueprintType)
struct FOutgoingChatMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString room{};

	UPROPERTY(BlueprintReadOnly)
	FString message{};

	void Dump() const;
};

USTRUCT(BlueprintType)
struct FOutgoingRoomMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString room{};

	void Dump() const;
};