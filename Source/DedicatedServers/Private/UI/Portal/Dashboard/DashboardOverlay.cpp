// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/Dashboard/DashboardOverlay.h"

#include "DedicatedServers/DedicatedServers.h"
#include "UI/Portal/Dashboard/QueueWidget.h"
#include "Components/WidgetSwitcher.h"
#include "UI/Portal/Dashboard/Play/PlayPage.h"
#include "UI/Portal/Dashboard/Collections/CollectionsPage.h"
#include "UI/Portal/Dashboard/Store/StorePage.h"
#include "UI/Common/TabButton.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "WebSocket/WebSocketManager.h"
#include "Player/DSPlayerController.h"
#include "UI/Portal/Dashboard/Play/PlayPage.h"
#include "UI/Portal/Dashboard/Play/Start/StartPage.h"
#include "UI/Chat/ChatComponent.h"
#include "Player/DSLocalPlayerSubsystem.h"
#include "UI/Portal/PortalManager.h"

void UDashboardOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	PortalManager = NewObject<UPortalManager>(this);
	WebSocketManager = GetGameInstance()->GetSubsystem<UWebSocketManager>();

	Button_Logout->OnClicked.AddDynamic(this, &UDashboardOverlay::Logout);

	Button_Play->Button->OnClicked.AddDynamic(this, &UDashboardOverlay::ShowPlayPage);
	Button_Collections->Button->OnClicked.AddDynamic(this, &UDashboardOverlay::ShowCollectionsPage);
	Button_Store->Button->OnClicked.AddDynamic(this, &UDashboardOverlay::ShowStorePage);

	PlayPage->StartPage->OnMatchmakingStarted.AddDynamic(this, &UDashboardOverlay::StartQueue);
	PlayPage->StartPage->OnMatchmakingStopped.AddDynamic(this, &UDashboardOverlay::StopQueue);

	ShowPlayPage();

	if (WebSocketManager->IsConnected())
	{
		ShowChat();
	}
	else
	{
		WebSocketManager->OnWebSocketConnected.AddDynamic(this, &UDashboardOverlay::ShowChat);
		WebSocketManager->Connect();
	}
}

void UDashboardOverlay::NativeDestruct()
{
	Super::NativeDestruct();
	WebSocketManager->OnWebSocketConnected.RemoveDynamic(this, &UDashboardOverlay::ShowChat);
}

void UDashboardOverlay::ShowChat()
{
	if (auto ChatComponent = GetOwningPlayer<ADSPlayerController>()->ChatComponent)
	{
		ChatComponent->CreateChatWidget(RoomTypes::GlobalRoom);
	}
}

void UDashboardOverlay::ShowPlayPage()
{
	SetActiveButton(Button_Play);
	WidgetSwitcher->SetActiveWidget(PlayPage);
}

void UDashboardOverlay::ShowCollectionsPage()
{
	SetActiveButton(Button_Collections);
	WidgetSwitcher->SetActiveWidget(CollectionsPage);
}

void UDashboardOverlay::ShowStorePage()
{
	SetActiveButton(Button_Store);
	WidgetSwitcher->SetActiveWidget(StorePage);
}

void UDashboardOverlay::SetActiveButton(UTabButton* ButtonToActivate) const
{
	if (ButtonToActivate)
	{
		// Play Button
		Button_Play->bIsActive = false;
		Button_Play->Button_TextBlock->SetColorAndOpacity(Button_Play->InactiveColor);

		// Collections Button
		Button_Collections->bIsActive = false;
		Button_Collections->Button_TextBlock->SetColorAndOpacity(Button_Collections->InactiveColor);

		// Store Button
		Button_Store->bIsActive = false;
		Button_Store->Button_TextBlock->SetColorAndOpacity(Button_Store->InactiveColor);

		ButtonToActivate->bIsActive = true;
		ButtonToActivate->Button_TextBlock->SetColorAndOpacity(ButtonToActivate->ActiveColor);
	}
}

void UDashboardOverlay::StartQueue()
{
	QueueWidget->Start();
}

void UDashboardOverlay::StopQueue()
{
	QueueWidget->Stop();
}

void UDashboardOverlay::Logout()
{
	Button_Logout->SetIsEnabled(false);

	if (auto ChatComponent = GetOwningPlayer<ADSPlayerController>()->ChatComponent)
	{
		ChatComponent->RemoveChatWidget();
	}

	UDSLocalPlayerSubsystem* LocalPlayerSubsystem = GetLocalPlayerSubsystem();
	if (IsValid(LocalPlayerSubsystem))
	{
		
		FDSAuthenticationResult AuthResult = LocalPlayerSubsystem->GetAuthResult();
		PortalManager->SignOut(AuthResult.refresh.token);
	}
}

UDSLocalPlayerSubsystem* UDashboardOverlay::GetLocalPlayerSubsystem()
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (IsValid(PlayerController) && IsValid(PlayerController->GetLocalPlayer()))
	{
		UDSLocalPlayerSubsystem* LocalPlayerSubsystem = PlayerController->GetLocalPlayer()->GetSubsystem<UDSLocalPlayerSubsystem>();
		if (IsValid(LocalPlayerSubsystem))
		{
			return LocalPlayerSubsystem;
		}
	}
	return nullptr;
}




