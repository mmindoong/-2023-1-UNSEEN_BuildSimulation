// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Build/BuildManager.h"
#include "PlaceableObject/PlaceableObjectBase.h"
#include "PlaceableObject_Farm.generated.h"

/**
 * 
 */
UCLASS()
class BUILDSIMULATION_API APlaceableObject_Farm : public APlaceableObjectBase
{
	GENERATED_BODY()

	APlaceableObject_Farm();

private:

	/*UFUNCTION(BlueprintCallable)
	bool FindClosestUnits(int32 NumActorsToFind, TArray<AActor*>& OutActors);

	UFUNCTION(BlueprintCallable)
	bool FindClosestResources(int32 NumActorsToFind, TArray<AActor*>& OutActors);
	
	TSubclassOf<AActor> Unit;
	TSubclassOf<AActor> Resource;*/
};
