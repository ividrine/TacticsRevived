// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DSPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "UI/Chat/ChatComponent.h"
#include "UI/Loading/LoadingMatchWidget.h"
#include "WebSocket/WebSocketMessageTypes.h"
#include "Game/DS_GameModeBase.h"


ADSPlayerController::ADSPlayerController()
{
	Username = "";
	PlayerSessionId = "";
}

void ADSPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ADSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (ULocalPlayer* LP = GetLocalPlayer())
	{
		if (auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LP))
		{
			Subsystem->AddMappingContext(PlayerMappingContext, 0);
		}
	}
}

void ADSPlayerController::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    if (IsLocalController() && ChatComponentClass && !ChatComponent)
    {
        ChatComponent = NewObject<UChatComponent>(this, ChatComponentClass);
        ChatComponent->RegisterComponent();
    }
}

void ADSPlayerController::ShowLoadingScreen(const FMatchSucceededMessageDetail& MatchDetail)
{
    if (ULoadingMatchWidget* LoadingWidget = CreateWidget<ULoadingMatchWidget>(GetWorld(), LoadingScreenClass))
    {
        TObjectPtr<UTexture2D> Player1Icon = nullptr;
        TObjectPtr<UTexture2D> Player2Icon = nullptr;


        if (MatchDetail.players.IsValidIndex(0))
        {
            Player1Icon = LoadPlayerIcon(MatchDetail.players[0].iconId);
        }

        if (MatchDetail.players.IsValidIndex(1))
        {
            Player2Icon = LoadPlayerIcon(MatchDetail.players[1].iconId);
        }

        LoadingWidget->UpdatePlayerInfo(
            MatchDetail.players.IsValidIndex(0) ? MatchDetail.players[0].playerId : TEXT("Waiting..."),
            Player1Icon ? Player1Icon : DefaultIcon,
            MatchDetail.players.IsValidIndex(1) ? MatchDetail.players[1].playerId : TEXT("Waiting..."),
            Player2Icon ? Player2Icon : DefaultIcon
        );

        LoadingWidget->UpdateLoadingProgress(0.0f);

        LoadingWidget->AddToViewport(9999);
        LoadingScreenWidget = LoadingWidget;
    }
}

void ADSPlayerController::ServerNotifyLevelLoaded_Implementation()
{
    bIsClientReady = true;
    if (ADS_GameModeBase* GameMode = Cast<ADS_GameModeBase>(GetWorld()->GetAuthGameMode()))
    {
        GameMode->MarkClientReady(this);
    }
}

void ADSPlayerController::ClientHideLoadingScreen_Implementation()
{
    if (LoadingScreenWidget.IsValid())
    {
        LoadingScreenWidget->RemoveFromParent();
        LoadingScreenWidget = nullptr;
    }
}

TObjectPtr<UTexture2D> ADSPlayerController::LoadPlayerIcon(const FString& IconId)
{
    if (!IconId.IsEmpty())
    {
        // Try loading from DataTable first
        if (IconDataTable)
        {
            FPlayerIconRow* Row = IconDataTable->FindRow<FPlayerIconRow>(FName(*IconId), TEXT(""));
            if (Row && Row->Icon)
            {
                // Assume DataTable has a UTexture2D* field named "Icon"
                return Row->Icon;
            }
        }
        // Fallback to direct asset load
        return Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *IconId));
    }
    return nullptr;
}
