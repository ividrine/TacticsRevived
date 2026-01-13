// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/CircularButton.h"
#include "UI/Slate/SCircularButton.h"
#include "Components/ButtonSlot.h"
#include "Framework/SlateDelegates.h"

TSharedRef<SWidget> UCircularButton::RebuildWidget()
{
    const TSharedRef<SCircularButton> ButtonRef = SNew(SCircularButton)
        .OnClicked(BIND_UOBJECT_DELEGATE(FOnClicked, SlateHandleClicked))
        .OnPressed(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandlePressed))
        .OnReleased(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandleReleased))
        .OnHovered_UObject(this, &ThisClass::SlateHandleHovered)
        .OnUnhovered_UObject(this, &ThisClass::SlateHandleUnhovered)
        .ButtonStyle(&GetStyle())
        .ClickMethod(GetClickMethod())
        .TouchMethod(GetTouchMethod())
        .IsFocusable(GetIsFocusable());

    MyButton = ButtonRef;

    if (GetChildrenCount())
    {
        if (UButtonSlot* ButtonSlot = Cast<UButtonSlot>(GetContentSlot()))
        {
            ButtonSlot->BuildSlot(ButtonRef);
        }
    }

    return ButtonRef;
}

void UCircularButton::ReleaseSlateResources(bool bReleaseChildren)
{
    Super::ReleaseSlateResources(bReleaseChildren);
    MyButton.Reset();
}


