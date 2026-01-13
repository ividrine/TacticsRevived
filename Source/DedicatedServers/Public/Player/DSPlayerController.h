// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Enums/DSEnums.h"
#include "DSPlayerController.generated.h"

class UInputMappingContext;
class UChatComponent;
class ULoadingMatchWidget;

struct FMatchSucceededMessageDetail;

USTRUCT(BlueprintType)
struct FPlayerIconRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icon")
	TObjectPtr<UTexture2D> Icon;
};

UCLASS()
class DEDICATEDSERVERS_API ADSPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	// Constructor
	ADSPlayerController();

	// Overrides
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UFUNCTION(BlueprintCallable)
	void ShowLoadingScreen(const FMatchSucceededMessageDetail& MatchDetail);

	UFUNCTION(Client, Reliable)
	void ClientHideLoadingScreen();
	void ClientHideLoadingScreen_Implementation();

	UFUNCTION(Server, Reliable)
	void ServerNotifyLevelLoaded();
	void ServerNotifyLevelLoaded_Implementation();

	// Getters
	bool IsClientReady() const { return bIsClientReady; }

	UPROPERTY(BlueprintReadOnly)
	FString Username;

	UPROPERTY(BlueprintReadOnly)
	FString PlayerSessionId;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UChatComponent> ChatComponent;


protected:

	virtual void PostInitializeComponents() override;
	

	UPROPERTY(EditDefaultsOnly, Category = "Options")
	TSubclassOf<ULoadingMatchWidget> LoadingScreenClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Options")
	TObjectPtr<UInputMappingContext> PlayerMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Options")
	TObjectPtr <UTexture2D> DefaultIcon;

	UPROPERTY(EditDefaultsOnly, Category = "Options")
	TObjectPtr<UDataTable> IconDataTable;
	

private:

	UPROPERTY(EditAnywhere, Category = Chat)
	TSubclassOf<UChatComponent> ChatComponentClass;

	bool bIsClientReady = false;

	TWeakObjectPtr<ULoadingMatchWidget> LoadingScreenWidget;

	float CurrentLoadProgress = 0.0f;
	FTimerHandle ProgressTimerHandle;

	TObjectPtr<UTexture2D> LoadPlayerIcon(const FString& IconId);
	
};
