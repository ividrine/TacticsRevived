// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Common/Progress/RadialProgress.h"

void URadialProgress::SetPercentValue(float percentValue)
{
	UFunction* Function = FindFunction(FName(TEXT("SetPercent")));
	if (Function)
	{
		struct PercentParams
		{
			int Percent;
		};

		// Initialize the parameter struct
		PercentParams Params;
		Params.Percent = percentValue;

		ProcessEvent(Function, &Params);
	}
}
