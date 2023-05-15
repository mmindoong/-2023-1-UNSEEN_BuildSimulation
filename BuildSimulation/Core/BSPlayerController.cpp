// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/BSPlayerController.h"

ABSPlayerController::ABSPlayerController()
{
	
	
}

void ABSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetShowMouseCursor(true);
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	bEnableMouseOverEvents= true;
	bEnableTouchOverEvents = true;
}
