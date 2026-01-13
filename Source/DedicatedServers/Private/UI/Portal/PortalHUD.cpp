// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/PortalHUD.h"
#include "UI/Portal/Login/LoginOverlay.h"
#include "Blueprint/UserWidget.h"
#include "UI/Portal/Dashboard/DashboardOverlay.h"

void APortalHUD::BeginPlay()
{
	Super::BeginPlay();

	// Todo check if player has saved refresh token and log them in
	APlayerController* OwningPlayerController = GetOwningPlayerController();

	LoginOverlay = CreateWidget<ULoginOverlay>(OwningPlayerController, LoginOverlayClass);

	if (IsValid(LoginOverlay))
	{
		LoginOverlay->AddToViewport();
	}

	FInputModeGameAndUI InputModeData;

	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	OwningPlayerController->SetInputMode(InputModeData);
	OwningPlayerController->SetShowMouseCursor(true);
}

void APortalHUD::OnSignIn()
{
	if (IsValid(LoginOverlay))
	{
		LoginOverlay->RemoveFromParent();
	}
	APlayerController* OwningPlayerController = GetOwningPlayerController();
	DashboardOverlay = CreateWidget<UDashboardOverlay>(OwningPlayerController, DashboardOverlayClass);
	if (IsValid(DashboardOverlay))
	{
		DashboardOverlay->AddToViewport();
	}
}

void APortalHUD::OnSignOut()
{
	if (IsValid(DashboardOverlay))
	{
		DashboardOverlay->RemoveFromParent();
	}
	APlayerController* OwningPlayerController = GetOwningPlayerController();
	LoginOverlay = CreateWidget<ULoginOverlay>(OwningPlayerController, LoginOverlayClass);
	if (IsValid(LoginOverlay))
	{
		LoginOverlay->AddToViewport();
	}
}
