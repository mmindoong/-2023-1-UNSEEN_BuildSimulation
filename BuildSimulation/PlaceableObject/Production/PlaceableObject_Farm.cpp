// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaceableObject/Production/PlaceableObject_Farm.h"

APlaceableObject_Farm::APlaceableObject_Farm(): APlaceableObjectBase()
{
	ManPower = 1;
	InputResource = FConstructionCost();
	FConstructionCost Output = FConstructionCost(0, FFoodData(4, 0, 0), 0, 0,0, 0, FCitizen());
	OutputResource = Output;
}


