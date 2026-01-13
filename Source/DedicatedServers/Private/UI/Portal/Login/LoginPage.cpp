// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/Login/LoginPage.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void ULoginPage::UpdateStatusMessage(const FString& Message)
{
	TextBlock_StatusMessage->SetText(FText::FromString(Message));
}

void ULoginPage::UpdateLoadingIndicator(bool visible)
{
	if (visible)
	{
		Widget_Spinner->SetVisibility(ESlateVisibility::Visible);
		TextBlock_Login->SetVisibility(ESlateVisibility::Hidden);
		Button_Login->SetIsEnabled(false);
	}
	else
	{
		Widget_Spinner->SetVisibility(ESlateVisibility::Hidden);
		TextBlock_Login->SetVisibility(ESlateVisibility::Visible);
		Button_Login->SetIsEnabled(true);
	}
}


