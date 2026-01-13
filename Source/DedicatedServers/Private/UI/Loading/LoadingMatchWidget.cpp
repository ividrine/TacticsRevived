// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Loading/LoadingMatchWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"

void ULoadingMatchWidget::UpdatePlayerInfo(const FString& Player1Username, UTexture2D* Player1Icon, const FString& Player2Username, UTexture2D* Player2Icon) const
{
    if (Player1UsernameText)
        Player1UsernameText->SetText(FText::FromString(Player1Username));
    if (Player1IconImage)
        Player1IconImage->SetBrushFromTexture(Player1Icon);
    if (Player2UsernameText)
        Player2UsernameText->SetText(FText::FromString(Player2Username));
    if (Player2IconImage)
        Player2IconImage->SetBrushFromTexture(Player2Icon);
}

void ULoadingMatchWidget::UpdateLoadingProgress(float Progress)
{
    if (LoadingProgressBar)
    {
        LoadingProgressBar->SetPercent(FMath::Clamp(Progress, 0.0f, 1.0f));
    }
}
