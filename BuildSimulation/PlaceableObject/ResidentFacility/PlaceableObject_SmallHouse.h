// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/BlueprintFactory.h"
#include "PlaceableObject/PlaceableObjectBase.h"
#include "PlaceableObject_SmallHouse.generated.h"

/**
 * 
 */
UCLASS()
class BUILDSIMULATION_API APlaceableObject_SmallHouse : public APlaceableObjectBase
{
	GENERATED_BODY()

public:
	APlaceableObject_SmallHouse();

	UPROPERTY(EditAnywhere, Category = "Blueprint Actor")
	TSubclassOf<AActor> BlueprintActorClass;
	
	virtual void SetObjectSelectedMode(bool IsSelected) override;
	
};
