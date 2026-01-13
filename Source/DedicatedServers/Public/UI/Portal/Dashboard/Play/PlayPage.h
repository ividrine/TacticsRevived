// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayPage.generated.h"

class UWidgetSwitcher;
class UStartPage;
class UJoinCustomPage;
class UCreateCustomPage;
class UDashboardNavigation;
class UTabButton;

UCLASS()
class DEDICATEDSERVERS_API UPlayPage : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UStartPage> StartPage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UJoinCustomPage> JoinCustomPage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCreateCustomPage> CreateCustomPage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTabButton> Button_Start;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTabButton> Button_JoinCustom;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTabButton> Button_CreateCustom;

	void SetActiveButton(UTabButton* ButtonToActivate) const;

	

protected:
	virtual void NativeConstruct() override;

private:

	UFUNCTION()
	void ShowStartPage();

	UFUNCTION()
	void ShowJoinCustomPage();

	UFUNCTION()
	void ShowCreateCustomPage();

	

	
	
};
