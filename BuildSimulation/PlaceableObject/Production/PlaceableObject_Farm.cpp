// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaceableObject/Production/PlaceableObject_Farm.h"

#include <queue>

#include "Build/BuildManager.h"
#include "Game/BSGameSingleton.h"

APlaceableObject_Farm::APlaceableObject_Farm(): APlaceableObjectBase()
{
	InputResource = FConstructionCost();
	FConstructionCost Output = FConstructionCost(0,4, 0, 0, 0, 0,0, 0, 0,0);
	OutputResource = Output;
	
}



