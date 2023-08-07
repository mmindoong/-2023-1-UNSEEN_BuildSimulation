// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
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
	// Player Resource를 받아서 업데이트 시켜줌, 변경된 값은 HUD의 값을 변경시켜줌
	void UpdateResource(const FConstructionCost& InPlayerResource,  FConstructionCost& PlayerResource);

	void UpdatePanelText(const FConstructionCost& PlayerResource);

protected:
	virtual void NativeConstruct() override;
	
private:
	UTextBlock* GoldText;
	UTextBlock* FruitText;
	UTextBlock* RiceText;
	UTextBlock* MeatText;
	UTextBlock* WoodText;
	UTextBlock* IronText;
	UTextBlock* RockText;
	UTextBlock* CoalText;
	
};

