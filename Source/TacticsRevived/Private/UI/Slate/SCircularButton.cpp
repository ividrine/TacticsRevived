#include "UI/Slate/SCircularButton.h"
#include "Framework/Application/SlateApplication.h"


FVector2D SCircularButton::GetCenter(const FGeometry& Geo) const
{
    return Geo.GetLocalSize() * 0.5f;
}

float SCircularButton::GetRadius(const FGeometry& Geo) const
{
    FVector2D Size = Geo.GetLocalSize();
    return FMath::Min(Size.X, Size.Y) * 0.5f;
}

bool SCircularButton::IsInCircle(const FGeometry& Geo, const FPointerEvent& Event)
{
    FVector2D Local = Geo.AbsoluteToLocal(Event.GetScreenSpacePosition());
    return FVector2D::Distance(Local, GetCenter(Geo)) <= GetRadius(Geo);
}

FReply SCircularButton::OnMouseButtonDown(const FGeometry& Geo, const FPointerEvent& Event)
{
    if (IsInCircle(Geo, Event)) return SButton::OnMouseButtonDown(Geo, Event);
    return FReply::Unhandled();
}

FReply SCircularButton::OnMouseButtonUp(const FGeometry& Geo, const FPointerEvent& Event)
{
    if (IsInCircle(Geo, Event)) return SButton::OnMouseButtonUp(Geo, Event);
    return FReply::Unhandled();
}

FReply SCircularButton::OnMouseMove(const FGeometry& Geo, const FPointerEvent& Event)
{
    if (IsInCircle(Geo, Event))
    {
        if (!bIsHovered)
        {
            bIsHovered = true;
            SButton::OnMouseEnter(Geo, Event);
        }

        return SButton::OnMouseMove(Geo, Event);
    }
    else
    {
        if (bIsHovered)
        {
            bIsHovered = false;
            SButton::OnMouseLeave(Event);
        }

        return FReply::Unhandled();
    }
}

void SCircularButton::OnMouseEnter(const FGeometry& Geo, const FPointerEvent& Event)
{
    // Only trigger hover if actually inside the circle
    if (IsInCircle(Geo, Event))
    {
        bIsHovered = true;
        SButton::OnMouseEnter(Geo, Event);
    }
}

void SCircularButton::OnMouseLeave(const FPointerEvent& Event)
{
    if (bIsHovered)
    {
        bIsHovered = false;
        SButton::OnMouseLeave(Event);
    }
}