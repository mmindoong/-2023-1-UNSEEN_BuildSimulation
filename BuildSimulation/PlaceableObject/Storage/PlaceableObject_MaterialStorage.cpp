// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaceableObject/Storage/PlaceableObject_MaterialStorage.h"

#include <queue>

#include "Build/BuildManager.h"

APlaceableObject_MaterialStorage::APlaceableObject_MaterialStorage() : APlaceableObjectBase()
{
	InputResource = FConstructionCost();
	OutputResource = FConstructionCost();
	

	
}



