// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "ConstructionCost.generated.h"


USTRUCT(BlueprintType)
struct FConstructionCost: public FTableRowBase
{
	GENERATED_BODY()
	FConstructionCost()
	: Gold(0)
	, Rice(0)
	, Fruit(0)
	, Meat(0)
	, Wood(0)
	, Rock(0)
	, Iron(0)
	, Coal(0)
	, TotalNum(0)
	, UsedNum(0)
	{};

	FConstructionCost(int32 InGold, int32 InRice, int32 InFruit, int32 InMeat, int32 InWood, int32 InRock, int32 InIron, int32 InCoal, int32 InTotalNum, int32 InUsedNum)
	: Gold(InGold)
	, Rice(InRice)
	, Fruit(InFruit)
	, Meat(InMeat)
	, Wood(InWood)
	, Rock(InRock)
	, Iron(InIron)
	, Coal(InCoal)
	, TotalNum(InTotalNum)
	, UsedNum(InUsedNum)
	{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	int32 Gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	int32 Rice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	int32 Fruit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	int32 Meat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	int32 Wood;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	int32 Rock;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	int32 Iron;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	int32 Coal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	int32 TotalNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	int32 UsedNum;
};
