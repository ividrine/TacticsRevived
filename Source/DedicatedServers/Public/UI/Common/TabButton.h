// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TabButton.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class DEDICATEDSERVERS_API UTabButton : public UUserWidget
{
	GENERATED_BODY()
	
	
public:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Button_TextBlock;

	UPROPERTY(BlueprintReadOnly)
	bool bIsActive = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tab Button Options")
	FSlateFontInfo TextFont;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tab Button Options")
	FLinearColor ActiveColor = FLinearColor(FLinearColor::White);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tab Button Options")
	FLinearColor InactiveColor = FLinearColor(FLinearColor::White);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tab Button Options")
	bool DisableUnderline = false;

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnHovered();

	UFUNCTION()
	void OnUnhovered();
	
};
