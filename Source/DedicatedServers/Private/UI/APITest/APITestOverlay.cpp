#include "UI/APITest/APITestOverlay.h"
#include "UI/APITest/APITestManager.h"
#include "UI/API/Fleets/FleetsBox.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "UI/HTTP/HttpRequestTypes.h"
#include "UI/API/Fleets/FleetId.h"

void UAPITestOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	check(APITestManagerClass);

	APITestManager = NewObject<UAPITestManager>(this, APITestManagerClass);

	check(FleetsBox);
	check(FleetsBox->Button_GetFleets);

	FleetsBox->Button_GetFleets->OnClicked.AddDynamic(this, &UAPITestOverlay::FleetsButtonClicked);
}

void UAPITestOverlay::FleetsButtonClicked()
{
	check(APITestManager);
	APITestManager->OnFleetsResponseReceived.AddDynamic(this, &UAPITestOverlay::FleetsResponseReceived);
	APITestManager->GetFleets();
	FleetsBox->Button_GetFleets->SetIsEnabled(false);

}

void UAPITestOverlay::FleetsResponseReceived(const FleetsResponse& Response, bool bWasSuccessful)
{
	if (APITestManager->OnFleetsResponseReceived.IsAlreadyBound(this, &UAPITestOverlay::FleetsResponseReceived))
	{
		APITestManager->OnFleetsResponseReceived.RemoveDynamic(this, &UAPITestOverlay::FleetsResponseReceived);
	}

	//FleetsBox->ScrollBox_Fleets->ClearChildren();

	if (bWasSuccessful)
	{
		for (const FString& FleetId : Response.FleetIds)
		{
			UFleetId* FleetIdWidget = CreateWidget<UFleetId>(this, FleetIdWidgetClass);
			FleetIdWidget->TextBlock_FleetId->SetText(FText::FromString(FleetId));
			FleetsBox->ScrollBox_Fleets->AddChild(FleetIdWidget);
		}
	}
	else
	{
		UFleetId* FleetIdWidget = CreateWidget<UFleetId>(this, FleetIdWidgetClass);
		FleetIdWidget->TextBlock_FleetId->SetText(FText::FromString("Error loading fleet ids"));
		FleetsBox->ScrollBox_Fleets->AddChild(FleetIdWidget);
	}

	FleetsBox->Button_GetFleets->SetIsEnabled(true);
}
