// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "DynamicPlaceableObjectData.generated.h"

class ABuildManager;

USTRUCT(BlueprintType)
struct FDynamicPlaceableObjectData
{
	GENERATED_BODY()
	
	FDynamicPlaceableObjectData()
	{
		HasData = false;
		ObjectCenterCell = FIntPoint(1, 1);
		Direction = 0;
		Height = 0.0f;
		Side = 0;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint ObjectCenterCell;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Direction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Height;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Side;
};


