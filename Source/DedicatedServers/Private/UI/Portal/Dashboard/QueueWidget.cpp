// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/Dashboard/QueueWidget.h"
#include "Animation/WidgetAnimation.h"
#include "Components/TextBlock.h"

void UQueueWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

bool UQueueWidget::IsPaused()
{
    return bIsPaused;
}

void UQueueWidget::UpdateTimer()
{
    CurrentTime += 1.0f;
    int32 Minutes = FMath::FloorToInt(CurrentTime / 60.0f);
    int32 Seconds = FMath::FloorToInt(CurrentTime) % 60;
    FString TimeString = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
    if (TimerText)
    {
        TimerText->SetText(FText::FromString(TimeString));
    }
}

void UQueueWidget::Start()
{
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UQueueWidget::UpdateTimer, 1.0f, true);
    SlideIn();
}

void UQueueWidget::Stop()
{
    
    GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
    CurrentTime = 0.0f;
    SlideOut();
}

void UQueueWidget::Pause()
{
    bIsPaused = true;
    GetWorld()->GetTimerManager().PauseTimer(TimerHandle);
}

void UQueueWidget::Resume()
{
    bIsPaused = false;
    GetWorld()->GetTimerManager().UnPauseTimer(TimerHandle);
}

void UQueueWidget::SlideIn()
{
    if (SlideInAnim) {
        StopAnimation(SlideInAnim);
        PlayAnimationForward(SlideInAnim);
    }
}

void UQueueWidget::SlideOut()
{
    if (SlideInAnim) {
        UE_LOG(LogTemp, Log, TEXT("SlideOut called"));
        StopAnimation(SlideInAnim);
        PlayAnimationReverse(SlideInAnim);
    }
}


