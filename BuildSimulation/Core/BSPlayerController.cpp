// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/BSPlayerController.h"
#include "Widget/BSHUD.h"

ABSPlayerController::ABSPlayerController()
{
	// BSHUDWidget Class를 상속받은 WBP_HUD를 Class 애샛으로 설정
	/*static ConstructorHelpers::FClassFinder<UBSHUD> BSHUDWidgetRef(TEXT("/Game/Blueprints/Widget/WBP_BSHUD.WBP_BSHUD_C"));
	if(BSHUDWidgetRef.Class)
	{
		BSHUDWidgetClass = BSHUDWidgetRef.Class;
	}*/
	
}

void ABSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetShowMouseCursor(true);
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	bEnableMouseOverEvents= true;
	bEnableTouchOverEvents = true;

	/*BSHUDWidget = CreateWidget<UBSHUD>(this, BSHUDWidgetClass);
	if (BSHUDWidget)
	{
		BSHUDWidget->AddToViewport();
	}*/
}

void ABSPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}




