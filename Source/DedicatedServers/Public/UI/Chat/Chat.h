// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Chat.generated.h"

class UWebSocketManager;

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UChat : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	FString Room;

	UFUNCTION(BlueprintCallable)
	UWebSocketManager* GetWebSocketManager();

	UFUNCTION(BlueprintCallable)
	void JoinRoom();

	UFUNCTION(BlueprintCallable)
	void ChatRoom(const FString& message);

	UFUNCTION(BlueprintCallable)
	void LeaveRoom();
	
private:

	UPROPERTY()
	TObjectPtr<UWebSocketManager> WebSocketManager;

	UFUNCTION()
	void ReceiveChatMessage(const FString& sender, const FString& message);
	
};
