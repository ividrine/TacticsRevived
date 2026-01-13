#pragma once

#include "CoreMinimal.h"

namespace Colors
{
    // With Opacity
    const FLinearColor WhiteOpacity = FLinearColor(1.0f, 1.0f, 1.0f, 0.15f);
    const FLinearColor BlueOpacity = FLinearColor(0.0f, 0.280401f, 1.0f, 0.3f);
    const FLinearColor DarkBlueOpacity = FLinearColor(0.0f, 0.280401f, 1.0f, 0.5f);
    const FLinearColor OrangeOpacity = FLinearColor(1.0f, 0.188499f, 0.f, 0.3f);
    const FLinearColor BrightRedOpacity = FLinearColor(1.0f, 0.f, 0.134151f, 0.3f);
    const FLinearColor DarkRedOpacity = FLinearColor(0.927083f, 0.067966f, 0.0f, 0.3f);
    const FLinearColor Transparent = FLinearColor(0.f, 0.f, 0.f, 0.f);

    // Disabled Colors
    const FLinearColor BlueOpacityDisabled = FLinearColor(0.0f, 0.280401f, 1.0f, 0.1f);
    const FLinearColor OrangeOpacityDisabled = FLinearColor(1.0f, 0.188499f, 0.f, 0.1f);
}