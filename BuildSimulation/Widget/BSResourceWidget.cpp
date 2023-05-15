// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/BSResourceWidget.h"

void UBSResourceWidget::CallUpdateBuildManagerResource(FConstructionCost NewResourceAmount)
{
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString::Printf(TEXT("Call BuildManagerResource")));
}

void UBSResourceWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}
