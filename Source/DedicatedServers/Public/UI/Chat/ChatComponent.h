// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChatComponent.generated.h"

class UChat;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class DEDICATEDSERVERS_API UChatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UChatComponent();

	// Create and add chat widget to viewport
	UFUNCTION(BlueprintCallable, Category = "Chat")
	void CreateChatWidget(const FString& Room);

	UFUNCTION(BlueprintCallable, Category = "Chat")
	void RemoveChatWidget();

	UFUNCTION(BlueprintCallable, Category = "Chat")
	void ToggleChat();
	
};
