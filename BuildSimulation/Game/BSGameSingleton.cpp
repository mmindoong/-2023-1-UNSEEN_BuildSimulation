// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BSGameSingleton.h"
#include "Engine/DataTable.h"


UBSGameSingleton::UBSGameSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Game/Blueprints/Data/DT_ObjectData.DT_ObjectData"));
	if (nullptr != DataTableRef.Object)
	{
		PlaceableObjectTable = DataTableRef.Object;
		check(PlaceableObjectTable->GetRowMap().Num() > 0);
	}

	static ConstructorHelpers::FClassFinder<AActor> PAWNDATA(TEXT("/Game/Blueprints/Core/Unit/BP_UnitPawn.BP_UnitPawn_C"));
	if(PAWNDATA.Succeeded())
	{
		Unit = PAWNDATA.Class;
	}

	static ConstructorHelpers::FClassFinder<AActor> IRON_DATA(TEXT("/Game/Blueprints/Core/ItemActor/BP_IronItem.BP_IronItem_C"));
	if(IRON_DATA.Succeeded())
	{
		Actor_Iron = IRON_DATA.Class;
	}
	static ConstructorHelpers::FClassFinder<AActor> ROCK_DATA(TEXT("/Game/Blueprints/Core/ItemActor/BP_RockItem.BP_RockItem_C"));
	if(ROCK_DATA.Succeeded())
	{
		Actor_Rock = ROCK_DATA.Class;
	}
	
	static ConstructorHelpers::FClassFinder<AActor> WOOD_DATA(TEXT("/Game/Blueprints/Core/ItemActor/BP_WoodItem.BP_WoodItem_C"));
	if(WOOD_DATA.Succeeded())
	{
		Actor_Wood = WOOD_DATA.Class;
	}
	static ConstructorHelpers::FClassFinder<AActor> COAL_DATA(TEXT("/Game/Blueprints/Core/ItemActor/BP_CoalItem.BP_CoalItem_C"));
	if(COAL_DATA.Succeeded())
	{
		Actor_Coal = COAL_DATA.Class;
	}

	static ConstructorHelpers::FClassFinder<AActor> RICE_DATA(TEXT("/Game/Blueprints/Core/ItemActor/BP_RiceItem.BP_RiceItem_C"));
	if(RICE_DATA.Succeeded())
	{
		Actor_Rice = RICE_DATA.Class;
	}

	static ConstructorHelpers::FClassFinder<AActor> FRUIT_DATA(TEXT("/Game/Blueprints/Core/ItemActor/BP_FruitItem.BP_FruitItem_C"));
	if(FRUIT_DATA.Succeeded())
	{
		Actor_Fruit = FRUIT_DATA.Class;
	}
	
	static ConstructorHelpers::FClassFinder<AActor> MEAT_DATA(TEXT("/Game/Blueprints/Core/ItemActor/BP_MeatItem.BP_MeatItem_C"));
	if(MEAT_DATA.Succeeded())
	{
		Actor_Meat = MEAT_DATA.Class;
	}
	
	static ConstructorHelpers::FClassFinder<AActor> CASTLE_DATA(TEXT("/Game/Blueprints/Core/PlaceableObject/BP_StartCastle.BP_StartCastle_C"));
	if(CASTLE_DATA.Succeeded())
	{
		Actor_Castle = CASTLE_DATA.Class;
	}

	static ConstructorHelpers::FClassFinder<AActor> FOODSTORAGE_DATA(TEXT("/Game/Blueprints/Core/PlaceableObject/Storage/BP_FoodStorage.BP_FoodStorage_C"));
	if(FOODSTORAGE_DATA.Succeeded())
	{
		Actor_FoodStorage = FOODSTORAGE_DATA.Class;
	}
	
	static ConstructorHelpers::FClassFinder<AActor> MATERIALSTORAGE_DATA(TEXT("/Game/Blueprints/Core/PlaceableObject/Storage/BP_MaterialStorage.BP_MaterialStorage_C"));
	if(MATERIALSTORAGE_DATA.Succeeded())
	{
		Actor_MaterialStorage = MATERIALSTORAGE_DATA.Class;
	}
	
}


UBSGameSingleton& UBSGameSingleton::Get()
{
	// 싱글톤 클래스 레퍼런스 받기
	UBSGameSingleton* Singleton = CastChecked<UBSGameSingleton>(GEngine->GameSingleton);
	if(Singleton)
	{
		return *Singleton;
	}
	UE_LOG(LogTemp, Error, TEXT("Invalid Game Singleton"));
	
	return *NewObject<UBSGameSingleton>();
	
}
