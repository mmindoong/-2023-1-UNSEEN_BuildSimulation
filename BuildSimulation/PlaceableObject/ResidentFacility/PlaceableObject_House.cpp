// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaceableObject/ResidentFacility/PlaceableObject_House.h"
#include "Build/BuildManager.h"
#include <algorithm>
#include <queue>

#include "AI/PawnUnit.h"

APlaceableObject_House::APlaceableObject_House() 
{
	InputResource = FConstructionCost();
	FConstructionCost Output = FConstructionCost(0, FFoodData(), 0, 0,0, 0, 1, 0);
	OutputResource = Output;
	
}

void APlaceableObject_House::BeginPlay()
{
	Super::BeginPlay();
	
}


