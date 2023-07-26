// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/PawnUnit.h"
#include "Build/BuildManager.h"
#include "PlaceableObject/PlaceableObjectBase.h"
#include "PlaceableObject_House.generated.h"

/**
 * 
 */
UCLASS()
class BUILDSIMULATION_API APlaceableObject_House : public APlaceableObjectBase
{
	GENERATED_BODY()

	APlaceableObject_House();

protected:
	virtual void BeginPlay() override;

private:
	
	
};




