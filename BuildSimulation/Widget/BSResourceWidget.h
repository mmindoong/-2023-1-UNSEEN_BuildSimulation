// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/ConstructionCost.h"
#include "BSResourceWidget.generated.h"

/**
 * 
 */
UCLASS()
class BUILDSIMULATION_API UBSResourceWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void CallUpdateBuildManagerResource(FConstructionCost NewResourceAmount);
protected:
	virtual void NativeConstruct() override;
	
};
