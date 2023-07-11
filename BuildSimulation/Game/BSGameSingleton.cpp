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
	
}

UBSGameSingleton& UBSGameSingleton::Get()
{
	// 싱글톤 클래스 레퍼런스 받기
	UBSGameSingleton* Singleton = CastChecked<UBSGameSingleton>(GEngine->GameSingleton);
	if(Singleton)
	{
		UE_LOG(LogTemp, Error, TEXT("Valid Game Singleton"));
		return *Singleton;
	}
	UE_LOG(LogTemp, Error, TEXT("Invalid Game Singleton"));
	return *NewObject<UBSGameSingleton>();
}
