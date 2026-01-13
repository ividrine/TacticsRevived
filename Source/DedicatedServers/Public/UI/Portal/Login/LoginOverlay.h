// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginOverlay.generated.h"

class UPortalManager;
class ULoginPage;
class UButton;

UCLASS()
class DEDICATEDSERVERS_API ULoginOverlay : public UUserWidget
{
	GENERATED_BODY()
	
	
public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPortalManager> PortalManagerClass;

protected:
	virtual void NativeConstruct() override;

private:

	UPROPERTY()
	TObjectPtr<UPortalManager> PortalManager;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<ULoginPage> LoginPage;

	UFUNCTION()
	void LoginButtonClicked();

	UFUNCTION()
	void ExitButtonClicked();

	void AutoSignIn();
};
