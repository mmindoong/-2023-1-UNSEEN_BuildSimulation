// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "ConstructionCost.generated.h"


USTRUCT(BlueprintType)
struct FFoodData
{
	GENERATED_BODY()
	FFoodData()
	: Rice(0)
	, Fruit(0)
	, Meat(0)
	{};

	FFoodData(int32 InRice, int32 InFruit, int32 InMeat)
	: Rice(InRice)
	, Fruit(InFruit)
	, Meat(InMeat)
	{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Rice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Fruit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Meat;

};

USTRUCT(BlueprintType)
struct FCitizen
{
	GENERATED_BODY()
	FCitizen()
	: TotalNum(0)
	, UsedNum(0)
	, Happiness(0)
	, Health(0)
	{};

	FCitizen(int32 InNum, int32 InUsedNum, int32 InHappiness, int32 InHealth)
	: TotalNum(InNum)
	, UsedNum(InUsedNum)
	, Happiness(InHappiness)
	, Health(InHealth)
	{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TotalNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UsedNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Happiness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Health;
};


USTRUCT(BlueprintType)
struct FConstructionCost: public FTableRowBase
{
	GENERATED_BODY()
	FConstructionCost()
	: Gold(0)
	, Food(FFoodData())
	, Wood(0)
	, Rock(0)
	, Iron(0)
	, Coal(0)
	, Citizen(FCitizen())
	{};

	FConstructionCost(int32 InGold, FFoodData InFood, int32 InWood, int32 InRock, int32 InIron, int32 InCoal, FCitizen InCitizen)
	: Gold(InGold)
	, Food(InFood)
	, Wood(InWood)
	, Rock(InRock)
	, Iron(InIron)
	, Coal(InCoal)
	, Citizen(InCitizen)
	{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	int32 Gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	FFoodData Food;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	int32 Wood;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	int32 Rock;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	int32 Iron;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	int32 Coal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	FCitizen Citizen;
};
