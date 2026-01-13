// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/API/APIData.h"

FString UAPIData::GetAPIEndpoint(FString Path) const
{
	return FString(APIUrl + "/" + APIVersion + "/" + Path);
}

FString UAPIData::GetWebSocketUrl() const
{
	return FString(WSUrl);
}
