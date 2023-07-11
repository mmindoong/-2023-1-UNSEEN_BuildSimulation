// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/BSHUD.h"

#include "Build/BuildManager.h"
#include "Interface/BSPawnHUDInterface.h"
#include "Kismet/GameplayStatics.h"

UBSHUD::UBSHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

/*void UBSHUD::NativeConstruct()
{
	Super::NativeConstruct();
	
	ResourceWidget = Cast<UBSResourceWidget>(GetWidgetFromName(TEXT("W_ResourceWidget")));
	IBSPawnHUDInterface* HUDPawn = Cast<IBSPawnHUDInterface>(GetOwningPlayerPawn());
	if(HUDPawn)
	{
		HUDPawn->SetupHUDWidget(this); //Pawn에서 Setup시켜주고 바인딩까지 호출
	}
	
}*/

void UBSHUD::UpdateResource(const FConstructionCost& InPlayerResource)
{
	//ResourceWidget->UpdateResource(InPlayerResource, PlayerResource);
}
