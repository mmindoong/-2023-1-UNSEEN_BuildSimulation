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
public:
	UBSHUD(const FObjectInitializer& ObjectInitializer);
	
protected:
	//virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void UpdateResource(const FConstructionCost& InPlayerResource); //Player Resource를 받아서 업데이트 시켜줌

private:
	UPROPERTY()
	TObjectPtr<class UBSResourceWidget> ResourceWidget;

	UPROPERTY()
	FConstructionCost PlayerResource;	
};
