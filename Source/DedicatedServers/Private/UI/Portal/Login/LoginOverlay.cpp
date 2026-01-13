// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/Login/LoginOverlay.h"
#include "UI/Portal/Login/LoginPage.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Player/DSLocalPlayerSubsystem.h"
#include "UI/Portal/PortalManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

void ULoginOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	check(PortalManagerClass);

	PortalManager = NewObject<UPortalManager>(this, PortalManagerClass);

	LoginPage->Button_Login->OnClicked.AddDynamic(this, &ULoginOverlay::LoginButtonClicked);
	LoginPage->Button_Exit->OnClicked.AddDynamic(this, &ULoginOverlay::ExitButtonClicked);
	PortalManager->SignInStatusMessageDelegate.AddDynamic(LoginPage, &ULoginPage::UpdateStatusMessage);
	PortalManager->LoadingDelegate.AddDynamic(LoginPage, &ULoginPage::UpdateLoadingIndicator);

	AutoSignIn();
}

void ULoginOverlay::LoginButtonClicked()
{
	const FString Username = LoginPage->TextBox_Username->GetText().ToString();
	const FString Password = LoginPage->TextBox_Password->GetText().ToString();
	if (UDSLocalPlayerSubsystem* DSLocalPlayerSubsystem = PortalManager->GetDSLocalPlayerSubsystem(); IsValid(DSLocalPlayerSubsystem))
	{
		DSLocalPlayerSubsystem->Username = Username;
		DSLocalPlayerSubsystem->Password = Password;
	}
	PortalManager->SignIn(Username, Password);
}

void ULoginOverlay::ExitButtonClicked()
{
	UKismetSystemLibrary::QuitGame(this, UGameplayStatics::GetPlayerController(this, 0), EQuitPreference::Quit, true);
}

void ULoginOverlay::AutoSignIn()
{
	if (UDSLocalPlayerSubsystem* DSLocalPlayerSubsystem = PortalManager->GetDSLocalPlayerSubsystem(); IsValid(DSLocalPlayerSubsystem))
	{
		const FString& Username = DSLocalPlayerSubsystem->Username;
		const FString& Password = DSLocalPlayerSubsystem->Password;
		if (Username.IsEmpty() || Password.IsEmpty()) return;

		LoginPage->Button_Login->SetIsEnabled(false);
		PortalManager->SignIn(Username, Password);
	}
}
