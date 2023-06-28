// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaceableObject/Storage/PlaceableObject_FoodStorage.h"

APlaceableObject_FoodStorage::APlaceableObject_FoodStorage() : APlaceableObjectBase()
{
	ManPower = 4;
	InputResource = FConstructionCost();
	OutputResource = FConstructionCost();
}
