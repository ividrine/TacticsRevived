// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QueueWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UQueueWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    virtual void NativeConstruct() override;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UTextBlock* TimerText;

    UFUNCTION()
    bool IsPaused();

    UFUNCTION()
    void UpdateTimer();

    UFUNCTION()
    void Start();

    UFUNCTION()
    void Stop();

    UFUNCTION()
    void Pause();

    UFUNCTION()
    void Resume();

protected:
    UPROPERTY(Transient, BlueprintReadOnly, meta = (BindWidgetAnim))
    TObjectPtr<UWidgetAnimation> SlideInAnim;

private:

    void SlideIn();
    void SlideOut();

    float CurrentTime = 0.0f;
    FTimerHandle TimerHandle;
    bool bIsPaused;
	
	
};
