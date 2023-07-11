// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/BSResourceWidget.h"



/*void UBSResourceWidget::UpdateResource(const FConstructionCost& InPlayerResource,  FConstructionCost& PlayerResource)
{
	// Set Resource Amount 변수, 실제 UI업데이트는 bp에서 진행
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, FString::Printf(TEXT("UpdateResource")));

	PlayerResource = InPlayerResource;
	UpdatePanelText(PlayerResource);
}

void UBSResourceWidget::UpdatePanelText(const FConstructionCost& PlayerResource)
{
	if(GoldText)
		GoldText->SetText(FText::FromString(FString::FromInt(PlayerResource.Gold)));
	if(FruitText)
		FruitText->SetText(FText::FromString(FString::FromInt(PlayerResource.Food.Fruit)));
	if(RiceText)
		RiceText->SetText(FText::FromString(FString::FromInt(PlayerResource.Food.Rice)));
	if(MeatText)
		MeatText->SetText(FText::FromString(FString::FromInt(PlayerResource.Food.Meat)));
	if(WoodText)
		WoodText->SetText(FText::FromString(FString::FromInt(PlayerResource.Wood)));
	if(RockText)
		RockText->SetText(FText::FromString(FString::FromInt(PlayerResource.Rock)));
	if(CoalText)
		CoalText->SetText(FText::FromString(FString::FromInt(PlayerResource.Coal)));
	if(IronText)
		IronText->SetText(FText::FromString(FString::FromInt(PlayerResource.Iron)));
}

void UBSResourceWidget::NativeConstruct()
{
	Super::NativeConstruct();
	GoldText = Cast<UTextBlock>(GetWidgetFromName(TEXT("GoldAmountText")));
	FruitText = Cast<UTextBlock>(GetWidgetFromName(TEXT("FruitAmountText")));
	RiceText = Cast<UTextBlock>(GetWidgetFromName(TEXT("RiceAmountText")));
	MeatText = Cast<UTextBlock>(GetWidgetFromName(TEXT("MeatAmountText")));
	WoodText = Cast<UTextBlock>(GetWidgetFromName(TEXT("WoodAmountText")));
	RockText = Cast<UTextBlock>(GetWidgetFromName(TEXT("RockAmountText")));
	CoalText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CoalAmountText")));
	IronText = Cast<UTextBlock>(GetWidgetFromName(TEXT("IronAmountText")));

}*/
