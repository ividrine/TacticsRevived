// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Chat/ChatComponent.h"
#include "UI/Chat/Chat.h"


// Sets default values for this component's properties
UChatComponent::UChatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UChatComponent::CreateChatWidget(const FString& Room)
{
	UFunction* Function = FindFunction(FName(TEXT("_CreateChatWidget")));
	if (Function)
	{
		struct FMyFunctionParams
		{
			FString Room;
		};

		// Initialize the parameter struct
		FMyFunctionParams Params;
		Params.Room = Room;

		ProcessEvent(Function, &Params);
	}
}

void UChatComponent::RemoveChatWidget()
{
	UFunction* Function = FindFunction(FName(TEXT("_RemoveChatWidget")));
	if (Function)
	{
		ProcessEvent(Function, nullptr);
	}
}


void UChatComponent::ToggleChat()
{
	UFunction* Function = FindFunction(FName(TEXT("Open_Close_Chat")));
	if (Function)
	{
		ProcessEvent(Function, nullptr);
	}
}

