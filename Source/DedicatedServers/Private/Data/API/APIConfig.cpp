// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/API/APIConfig.h"
#include "Data/API/APIData.h"

FString UAPIConfig::GetAPIEndpoint(FString Path)
{
	#if UE_BUILD_SHIPPING
		return Production->GetAPIEndpoint(Path);
	#else
		return Development->GetAPIEndpoint(Path);
	#endif
}

FString UAPIConfig::GetWebSocketUrl()
{
	#if UE_BUILD_SHIPPING
		return Production->GetWebSocketUrl();
	#else
		return Development->GetWebSocketUrl();
	#endif
}
