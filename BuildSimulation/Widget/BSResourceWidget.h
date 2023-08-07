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
	// Player Resource�� �޾Ƽ� ������Ʈ ������, ����� ���� HUD�� ���� ���������
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

