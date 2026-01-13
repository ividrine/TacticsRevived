// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/Dashboard/Play/PlayPage.h"

#include "Components/WidgetSwitcher.h"
#include "UI/Portal/Dashboard/Play/Start/StartPage.h"
#include "UI/Portal/Dashboard/Play/JoinCustom/JoinCustomPage.h"
#include "UI/Portal/Dashboard/Play/CreateCustom/CreateCustomPage.h"
#include "UI/Common/TabButton.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UPlayPage::SetActiveButton(UTabButton* ButtonToActivate) const
{
	if (ButtonToActivate)
	{
		Button_Start->bIsActive = false;
		Button_Start->Button_TextBlock->SetColorAndOpacity(Button_Start->InactiveColor);

		Button_JoinCustom->bIsActive = false;
		Button_JoinCustom->Button_TextBlock->SetColorAndOpacity(Button_JoinCustom->InactiveColor);

		Button_CreateCustom->bIsActive = false;
		Button_CreateCustom->Button_TextBlock->SetColorAndOpacity(Button_CreateCustom->InactiveColor);

		ButtonToActivate->bIsActive = true;
		ButtonToActivate->Button_TextBlock->SetColorAndOpacity(ButtonToActivate->ActiveColor);
	}
}

void UPlayPage::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Start->Button->OnClicked.AddDynamic(this, &UPlayPage::ShowStartPage);
	Button_JoinCustom->Button->OnClicked.AddDynamic(this, &UPlayPage::ShowJoinCustomPage);
	Button_CreateCustom->Button->OnClicked.AddDynamic(this, &UPlayPage::ShowCreateCustomPage);

	ShowStartPage();
}

void UPlayPage::ShowStartPage()
{
	SetActiveButton(Button_Start);
	WidgetSwitcher->SetActiveWidget(StartPage);
}

void UPlayPage::ShowJoinCustomPage()
{
	SetActiveButton(Button_JoinCustom);
	WidgetSwitcher->SetActiveWidget(JoinCustomPage);
}

void UPlayPage::ShowCreateCustomPage()
{
	SetActiveButton(Button_CreateCustom);
	WidgetSwitcher->SetActiveWidget(CreateCustomPage);
}