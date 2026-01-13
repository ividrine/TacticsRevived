#pragma once

#include "Widgets/Input/SButton.h"

class SCircularButton : public SButton
{

protected:

	// Utility
	FVector2D GetCenter(const FGeometry& AllottedGeometry) const;
	float     GetRadius(const FGeometry& AllottedGeometry) const;
	bool IsInCircle(const FGeometry& Geo, const FPointerEvent& Event);

	// Overrides
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;
	virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

private:
	bool bIsHovered;
};