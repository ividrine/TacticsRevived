// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/TacticsUserWidget.h"

void UTacticsUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
