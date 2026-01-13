// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/Interfaces/HUDManagement.h"
#include "PortalHUD.generated.h"


class ULoginOverlay;
class UDashboardOverlay;

UCLASS()
class DEDICATEDSERVERS_API APortalHUD : public AHUD, public IHUDManagement
{
	GENERATED_BODY()
	
public: 
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ULoginOverlay> LoginOverlayClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDashboardOverlay> DashboardOverlayClass;

	// IHUDManagement
	virtual void OnSignIn() override;
	virtual void OnSignOut() override;

protected:
	virtual void BeginPlay() override;
	
	
private:

	UPROPERTY()
	TObjectPtr<ULoginOverlay> LoginOverlay;

	UPROPERTY()
	TObjectPtr<UDashboardOverlay> DashboardOverlay;
	
};
