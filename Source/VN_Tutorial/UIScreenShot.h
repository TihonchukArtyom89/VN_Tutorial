// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UnrealClient.h"
#include "UIScreenShot.generated.h"

/**
 * 
 */
UCLASS()
class VN_TUTORIAL_API UUIScreenShot : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,Category="UI Screenshot")
		static void TakeScreenShotOfUI(FString name) {
		FScreenshotRequest::RequestScreenshot(name, true, false);
	}
	
	
	
	
};
