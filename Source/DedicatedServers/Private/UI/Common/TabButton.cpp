// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Common/TabButton.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UTabButton::NativeConstruct()
{
	Super::NativeConstruct();

	Button->OnHovered.AddDynamic(this, &UTabButton::OnHovered);
	Button->OnUnhovered.AddDynamic(this, &UTabButton::OnUnhovered);
	Button_TextBlock->SetFont(TextFont);
}

void UTabButton::OnHovered()
{
	Button_TextBlock->SetColorAndOpacity(ActiveColor);
}

void UTabButton::OnUnhovered()
{
	Button_TextBlock->SetColorAndOpacity(bIsActive ? ActiveColor : InactiveColor);
}
