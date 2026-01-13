// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "CircularButton.generated.h"

class SCircularButton;

UCLASS()
class TACTICSREVIVED_API UCircularButton : public UButton
{
	GENERATED_BODY()

protected:
    /** Builds the underlying Slate widget (uses our custom SButton) */
    virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	
};
