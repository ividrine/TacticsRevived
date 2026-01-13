// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MatchCreatedWidget.generated.h"


class UButton;
class URadialProgress;

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UMatchCreatedWidget : public UUserWidget
{
	GENERATED_BODY()
	
	

public:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void Show(int acceptanceTime);

	UFUNCTION()
	void Close();

	UPROPERTY(BlueprintReadOnly, Category = "Timer")
	float TimerPercentage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> AcceptMatch_Button;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> DeclineMatch_Button;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<URadialProgress> RadialProgress;

protected:
	void UpdateTimer();

private:
	float TotalDuration;

	// Time when the countdown started
	float StartTime;

	// Timer handle for managing the countdown
	FTimerHandle TimerHandle;

	// Target percentage for interpolation
	float TargetPercentage;

	// Whether the timer is active
	bool bIsTimerActive;
	
	
};
