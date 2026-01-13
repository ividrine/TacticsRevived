// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingMatchWidget.generated.h"


class UTextBlock;
class UImage;
class UTexture2D;
class UProgressBar;

UCLASS()
class DEDICATEDSERVERS_API ULoadingMatchWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<UTextBlock> Player1UsernameText;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<UImage> Player1IconImage;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<UTextBlock> Player2UsernameText;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<UImage> Player2IconImage;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<UProgressBar> LoadingProgressBar;

    UFUNCTION(BlueprintCallable)
    void UpdatePlayerInfo(const FString& Player1Username, UTexture2D* Player1Icon, const FString& Player2Username, UTexture2D* Player2Icon) const;

    UFUNCTION(BlueprintCallable)
    void UpdateLoadingProgress(float Progress);
	
};
