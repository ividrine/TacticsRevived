// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APITestOverlay.generated.h"

class UFleetsBox;
class UAPITestManager;
class UFleetId;
struct FleetsResponse;

UCLASS()
class DEDICATEDSERVERS_API UAPITestOverlay : public UUserWidget
{
	GENERATED_BODY()

	public:

		UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UAPITestManager> APITestManagerClass;

		UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UFleetId> FleetIdWidgetClass;
	
	protected:

		virtual void NativeConstruct() override;

	private:

		UPROPERTY(meta = (BindWidget))
		TObjectPtr<UFleetsBox> FleetsBox;

		UPROPERTY()
		TObjectPtr<UAPITestManager> APITestManager;

		UFUNCTION()
		void FleetsButtonClicked();

		UFUNCTION()
		void FleetsResponseReceived(const FleetsResponse& Response, bool bWasSuccessful);
	
};
