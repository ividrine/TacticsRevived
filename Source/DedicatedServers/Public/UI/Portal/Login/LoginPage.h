// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginPage.generated.h"

class UEditableTextBox;
class UButton;
class UTextBlock;

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API ULoginPage : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> TextBox_Username;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> TextBox_Password;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Login;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Exit;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_StatusMessage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Login;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUserWidget> Widget_Spinner;

	UFUNCTION()
	void UpdateStatusMessage(const FString& Message);

	UFUNCTION()
	void UpdateLoadingIndicator(bool visible);
	
	
};
