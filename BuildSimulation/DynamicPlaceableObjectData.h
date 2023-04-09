// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "DynamicPlaceableObjectData.generated.h"

class AGridActor;

USTRUCT(BlueprintType)
struct FDynamicPlaceableObjectData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AGridActor* BuildManager;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint ObjectCenterCell;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Direction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Height;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Side;
};


