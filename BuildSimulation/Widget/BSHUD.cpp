// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/BSHUD.h"

#include "Build/BuildManager.h"
#include "Kismet/GameplayStatics.h"

void UBSHUD::NativeConstruct()
{
	Super::NativeConstruct();
	
	AActor* BuildManagerActor = UGameplayStatics::GetActorOfClass(GetWorld(), ABuildManager::StaticClass());
	
	/*
	FStringClassReference MyWidgetClassRef(TEXT("Game/Blueprints/Widget/Resource/W_ResourceWidget.W_ResourceWidget_C"));

	if ( UClass* MyWidgetClass = MyWidgetClassRef.TryLoadClass<UUserWidget>() )
	{
		UUserWidget* MyWidgets = CreateWidget<UUserWidget>(GetWorld(), UBSResourceWidget::StaticClass());
		ResourceWidget = Cast<UBSResourceWidget>(MyWidgets);
	}
	*/
	//ResourceWidget = Cast<UBSResourceWidget>(GetWidgetFromName(TEXT("W_ResourceWidget")));
	
	
	if(BuildManagerActor)
	{
		BuildManager = Cast<ABuildManager>(BuildManagerActor);
		// Binding event when UI construct
		BuildManager->UpdateResourceAmountEvent.BindUFunction(ResourceWidget, FName("BindBuildManagerResource"));
	}
	
}
