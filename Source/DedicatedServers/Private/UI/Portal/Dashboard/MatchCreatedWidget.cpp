// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/Dashboard/MatchCreatedWidget.h"
#include "Components/Button.h"
#include "DedicatedServers/DedicatedServers.h"
#include "UI/Common/Progress/RadialProgress.h"

void UMatchCreatedWidget::NativeConstruct()
{
    Super::NativeConstruct();

    TotalDuration = 0.0f;
    StartTime = 0.0f;
    TimerPercentage = 0.0f;
    TargetPercentage = 0.0f;
    bIsTimerActive = false;
    
}

void UMatchCreatedWidget::NativeDestruct()
{
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
    }

    // Stop ticking
    bIsTimerActive = false;

    Super::NativeDestruct();
}

void UMatchCreatedWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (bIsTimerActive)
    {
        // Smoothly interpolate TimerPercentage towards TargetPercentage
        TimerPercentage = FMath::FInterpTo(TimerPercentage, TargetPercentage, InDeltaTime, 15.0f);

        // Clamp the percentage to ensure it stays between 0 and 1
        TimerPercentage = FMath::Clamp(TimerPercentage, 0.0f, 1.0f);

        RadialProgress->SetPercentValue(TimerPercentage);
    }
}

void UMatchCreatedWidget::Show(int acceptanceTime)
{
    TotalDuration = FMath::Max(static_cast<float>(acceptanceTime), 0.0f);
    StartTime = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;
    TimerPercentage = 0.0f;
    TargetPercentage = 0.0f;
    bIsTimerActive = TotalDuration > 0.0f;

    SetVisibility(ESlateVisibility::Visible);

    // Start the timer to call UpdateTimer every 0.05 seconds
    if (bIsTimerActive && GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMatchCreatedWidget::UpdateTimer, 0.05f, true);
    }
}

void UMatchCreatedWidget::Close()
{
    TargetPercentage = 1.0f;
    TimerPercentage = 1.0f;
    bIsTimerActive = false;

    SetVisibility(ESlateVisibility::Hidden);

    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
    }

}

void UMatchCreatedWidget::UpdateTimer()
{
    if (!GetWorld())
    {
        bIsTimerActive = false;
        return;
    }

    // Calculate elapsed time since the countdown started
    float CurrentTime = GetWorld()->GetTimeSeconds();
    float ElapsedTime = CurrentTime - StartTime;

    if (ElapsedTime < TotalDuration)
    {
        // Calculate the target percentage (0 at start, 1 at end)
        TargetPercentage = ElapsedTime / TotalDuration;

        // Clamp the target percentage
        TargetPercentage = FMath::Clamp(TargetPercentage, 0.0f, 1.0f);

        // Optional: Log for debugging
        // UE_LOG(LogTemp, Log, TEXT("ElapsedTime: %f, TargetPercentage: %f, TimerPercentage: %f"), ElapsedTime, TargetPercentage, TimerPercentage);
    }
    else
    {
        // Timer has finished
        TargetPercentage = 1.0f;
        TimerPercentage = 1.0f;
        bIsTimerActive = false;

        // Stop the timer
        if (GetWorld())
        {
            GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
        }

        // Log to confirm completion
        UE_LOG(LogTemp, Log, TEXT("Countdown completed in %f seconds"), ElapsedTime);
    }
}
