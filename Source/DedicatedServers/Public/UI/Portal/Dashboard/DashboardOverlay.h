// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DashboardOverlay.generated.h"

class UWidgetSwitcher;
class UPlayPage;
class UCollectionsPage;
class UStorePage;
class UTabButton;
class UWebSocketManager;
class UQueueWidget;
class UMatchCreatedWidget;
class UPortalManager;
class UButton;
class UDSLocalPlayerSubsystem;

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UDashboardOverlay : public UUserWidget
{
	GENERATED_BODY()
	
	
public:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPlayPage> PlayPage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCollectionsPage> CollectionsPage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UStorePage> StorePage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTabButton> Button_Play;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTabButton> Button_Collections;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTabButton> Button_Store;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UQueueWidget> QueueWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMatchCreatedWidget> MatchCreatedWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Logout;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:

	/* Network Request / Message Managers */

	UPROPERTY()
	TObjectPtr<UWebSocketManager> WebSocketManager;

	UPROPERTY()
	TObjectPtr<UPortalManager> PortalManager;

	UFUNCTION()
	void ShowPlayPage();

	UFUNCTION()
	void ShowCollectionsPage();

	UFUNCTION()
	void ShowStorePage();

	UFUNCTION()
	void ShowChat();

	UFUNCTION()
	void SetActiveButton(UTabButton* ButtonToActivate) const;

	UFUNCTION()
	void StartQueue();

	UFUNCTION()
	void StopQueue();

	UFUNCTION()
	void Logout();

	UDSLocalPlayerSubsystem* GetLocalPlayerSubsystem();
	
};
