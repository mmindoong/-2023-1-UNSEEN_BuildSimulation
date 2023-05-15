// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BSResourceWidget.h"
#include "Blueprint/UserWidget.h"
#include "Build/BuildManager.h"
#include "BSHUD.generated.h"

/**
 * 
 */
UCLASS()
class BUILDSIMULATION_API UBSHUD : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY()
	ABuildManager* BuildManager;

	UPROPERTY()
	UBSResourceWidget* ResourceWidget;
};
