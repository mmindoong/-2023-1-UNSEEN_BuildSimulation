// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "Math.h"
#include "ConstructionCost.h"
#include "ObjectData.generated.h"

class APlaceableObjectBase;
class APlacer;

USTRUCT(BlueprintType)
struct FObjectData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FObjectData()
	: PlacerClass(nullptr)
	, PlacaebleObjectClass(nullptr)
	, ConstructionCost()
	, ObjectSize()
	, Icon(nullptr)
	, ProceedingMesh(nullptr)
	, Mesh(nullptr)
	, Name()
	, Description()
	, ResourceType(-1)
	, IsProductionFacility(false)
	, IsResidentFacility(false)
	, HappinessFacilityType(-1)
	{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlaceableObjectData")
	TSubclassOf<APlacer> PlacerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlaceableObjectData")
	TSubclassOf<APlaceableObjectBase> PlacaebleObjectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlaceableObjectData")
	FConstructionCost ConstructionCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlaceableObjectData")
	FIntPoint ObjectSize;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlaceableObjectData")
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlaceableObjectData")
	TObjectPtr<UStaticMesh> ProceedingMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlaceableObjectData")
	TObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlaceableObjectData")
	FString Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlaceableObjectData")
	FText Description;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlaceableObjectData")
	int32 ResourceType; // 0:Units 1:Wood 2:Rock 3:Iron 4:Coal

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlaceableObjectData")
	bool IsProductionFacility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlaceableObjectData")
	bool IsResidentFacility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlaceableObjectData")
	int32 HappinessFacilityType; // 1:Road, 2:Well, 3:Market, 4:Church, 5:Bank
};
