// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/ObjectData.h"
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
	FORCEINLINE TSubclassOf<AActor> GetUnitClass() { return Unit;}
	FORCEINLINE TSubclassOf<AActor> GetWoodClass() { return Actor_Wood;}
	FORCEINLINE TSubclassOf<AActor> GetIronClass() { return Actor_Iron;}
	FORCEINLINE TSubclassOf<AActor> GetRockClass() { return Actor_Rock;}
	FORCEINLINE TSubclassOf<AActor> GetCoalClass() { return Actor_Coal;}
	FORCEINLINE TSubclassOf<AActor> GetRiceClass() { return Actor_Rice;}
	FORCEINLINE TSubclassOf<AActor> GetMeatClass() { return Actor_Meat;}
	FORCEINLINE TSubclassOf<AActor> GetFruitClass() { return Actor_Fruit;}
	FORCEINLINE TSubclassOf<AActor> GetStartCastleClass() { return Actor_Castle; }
	FORCEINLINE TSubclassOf<AActor> GetFoodStorageClass() { return Actor_FoodStorage; }
	FORCEINLINE TSubclassOf<AActor> GetMaterialStorageClass() { return Actor_MaterialStorage; }

	
	UPROPERTY()
	int32 MaxRow;
	
private:
	UDataTable* PlaceableObjectTable;

	TSubclassOf<AActor> Unit;
	TSubclassOf<AActor> Actor_Wood;
	TSubclassOf<AActor> Actor_Iron;
	TSubclassOf<AActor> Actor_Rock;
	TSubclassOf<AActor> Actor_Coal;
	TSubclassOf<AActor> Actor_Rice;
	TSubclassOf<AActor> Actor_Meat;
	TSubclassOf<AActor> Actor_Fruit;
	TSubclassOf<AActor> Actor_Castle;
	TSubclassOf<AActor> Actor_FoodStorage;
	TSubclassOf<AActor> Actor_MaterialStorage;

};
