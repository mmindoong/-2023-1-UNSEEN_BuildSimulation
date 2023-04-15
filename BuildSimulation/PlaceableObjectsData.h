// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "Math.h"
#include "ConstructionCost.h"
#include "PlaceableObjectsData.generated.h"

class APlaceableObjectBase;
class APlacerObjectBase;

USTRUCT(BlueprintType)
struct FPlaceableObjectData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlaceableObjectData")
	APlacerObjectBase* ObjectPlacerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlaceableObjectData")
	APlaceableObjectBase* PlacaebleObjectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlaceableObjectData")
	FConstructionCost ConstructionCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlaceableObjectData")
	FIntPoint ObjectSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlaceableObjectData")
	float MaxHeightDifferenceForConstruction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlaceableObjectData")
	int32 ReturnResourcesPercent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlaceableObjectData")
	float HealthPoints;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlaceableObjectData")
	bool CanBeDestroyedByPlayer;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlaceableObjectData")
	bool CanBeDamagedByInfantry;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlaceableObjectData")
	bool CanBeDamagedByDragon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlaceableObjectData")
	bool Flammable;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlaceableObjectData")
	UTexture2D* Icon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlaceableObjectData")
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlaceableObjectData")
	FString Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlaceableObjectData")
	FText Description;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlaceableObjectData")
	bool EnableOutline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlaceableObjectData")
	bool EnableHpBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlaceableObjectData")
	float HpBarVerticalOffset;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlaceableObjectData")
	float HpBarWidth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlaceableObjectData")
	int32 ResourceType;
};
