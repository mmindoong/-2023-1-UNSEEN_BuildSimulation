// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "ConstructionCost.generated.h"


USTRUCT(BlueprintType)
struct FFoodData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Rice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Fruit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Meat;
};


USTRUCT(BlueprintType)
struct FConstructionCost: public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConstructionCost")
	int32 Gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConstructionCost")
	FFoodData Food;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConstructionCost")
	int32 Wood;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConstructionCost")
	int32 Rock;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConstructionCost")
	int32 Iron;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConstructionCost")
	int32 Coal;

};
