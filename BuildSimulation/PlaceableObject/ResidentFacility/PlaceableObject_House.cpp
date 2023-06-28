// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaceableObject/ResidentFacility/PlaceableObject_House.h"

APlaceableObject_House::APlaceableObject_House()
{
	ManPower = 0;
	InputResource = FConstructionCost();
	FConstructionCost Output = FConstructionCost(0, FFoodData(), 0, 0,0, 0, FCitizen(1, 0, 0, 0));
	OutputResource = Output;
}
