// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gate.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "BSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BUILDSIMULATION_API ABSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABSPlayerController();

	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnBuildEvnet")) //bp에서 Display 시킬 함수 이름
	void K2_OnBuildEvent();
	// HUD Section

};
