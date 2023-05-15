// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
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
	
};
