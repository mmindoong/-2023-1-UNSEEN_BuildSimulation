// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/PlaceableObjectsData.h"
#include "UObject/NoExportTypes.h"
#include "BSGameSingleton.generated.h"


/**
 * 
 */
UCLASS()
class BUILDSIMULATION_API UBSGameSingleton : public UObject
{
	GENERATED_BODY()

public:
	UBSGameSingleton();
	static UBSGameSingleton& Get();

public:
	FORCEINLINE UDataTable* GetPlaceableObjectDataTable() { return  PlaceableObjectTable; }

	UPROPERTY()
	int32 MaxRow;
	
private:
	UDataTable* PlaceableObjectTable;
};
