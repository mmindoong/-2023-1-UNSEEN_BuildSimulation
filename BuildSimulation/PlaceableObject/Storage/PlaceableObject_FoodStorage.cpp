// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaceableObject/Storage/PlaceableObject_FoodStorage.h"

#include <queue>
#include "Build/BuildManager.h"
#include "Engine/StreamableManager.h"

APlaceableObject_FoodStorage::APlaceableObject_FoodStorage() : APlaceableObjectBase()
{
	InputResource = FConstructionCost();
	OutputResource = FConstructionCost();

	
	
}

