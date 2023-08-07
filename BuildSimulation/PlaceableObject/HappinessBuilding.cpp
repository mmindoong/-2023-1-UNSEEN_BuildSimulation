// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaceableObject/HappinessBuilding.h"

#include "Blueprint/UserWidget.h"
#include "Game/BSGameSingleton.h"

AHappinessBuilding::AHappinessBuilding() : APlaceableObjectBase()
{
	if(UGameplayStatics::GetActorOfClass(GetWorld(), ABuildManager::StaticClass()))
	{
		BuildManager = Cast<ABuildManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ABuildManager::StaticClass()));
	}
}

void AHappinessBuilding::BeginPlay()
{
	Super::BeginPlay();
	FObjectData* OutRow = UBSGameSingleton::Get().GetPlaceableObjectDataTable()->FindRow<FObjectData>(RowName, "");

	if (OutRow != nullptr)
	{
		if(OutRow->ConstructionCost.UsedNum > 0)
		{
			InputResourceTypeforNumActorstoFind.Add(0, OutRow->ConstructionCost.UsedNum); // ResourceType: 0, Num : UsedNum
			InputResourceTypeforClass.Add(0, UBSGameSingleton::Get().GetUnitClass());
		}
		if(OutRow->ConstructionCost.Wood > 0)
		{
			InputResourceTypeforNumActorstoFind.Add(1, OutRow->ConstructionCost.Wood); // ResourceType: 1, Num : Woods
			InputResourceTypeforClass.Add(0, UBSGameSingleton::Get().GetWoodClass());
		}
		if(OutRow->ConstructionCost.Rock > 0)
		{
			InputResourceTypeforNumActorstoFind.Add(2, OutRow->ConstructionCost.Rock); // ResourceType: 2, Num : Rock
			InputResourceTypeforClass.Add(0, UBSGameSingleton::Get().GetRockClass());
		}
		if(OutRow->ConstructionCost.Iron > 0)
		{
			InputResourceTypeforNumActorstoFind.Add(3, OutRow->ConstructionCost.Iron); // ResourceType: 3, Num : Iron
			InputResourceTypeforClass.Add(0, UBSGameSingleton::Get().GetIronClass());
		}
		if(OutRow->ConstructionCost.Coal > 0)
		{
			InputResourceTypeforNumActorstoFind.Add(4, OutRow->ConstructionCost.Coal); // ResourceType: 4, Num : Coal
			InputResourceTypeforClass.Add(0, UBSGameSingleton::Get().GetCoalClass());
		}
		if(OutRow->ConstructionCost.Rice > 0)
		{
			InputResourceTypeforNumActorstoFind.Add(5, OutRow->ConstructionCost.Rice); // ResourceType: 5, Num : Rice
			InputResourceTypeforClass.Add(0, UBSGameSingleton::Get().GetRiceClass());
		}
		if(OutRow->ConstructionCost.Fruit > 0)
		{
			InputResourceTypeforNumActorstoFind.Add(6, OutRow->ConstructionCost.Fruit); // ResourceType: 6, Num : Fruit
			InputResourceTypeforClass.Add(0, UBSGameSingleton::Get().GetFruitClass());
		}
		if(OutRow->ConstructionCost.Meat > 0)
		{
			InputResourceTypeforNumActorstoFind.Add(7, OutRow->ConstructionCost.Meat); // ResourceType: 7, Num : Meat
			InputResourceTypeforClass.Add(0, UBSGameSingleton::Get().GetMeatClass());
		}
	}
	
	if (OutRow != nullptr)
	{
		if(IsDynamicData) //BuildPlaceableObject 함수 호출 여부
		{
			if(BuildManager->GetPlayerResources().UsedNum + OutRow->ConstructionCost.UsedNum > BuildManager->GetPlayerResources().TotalNum)
				FailtoFind();
			else
			{
				if(FindClosestUnits(OutRow->ConstructionCost.UsedNum, UnitActors))
				{
					for(int32 ResourceType = 0; ResourceType <= 7; ResourceType++)
					{
						if(InputResourceTypeforNumActorstoFind.Contains(ResourceType))
						{
							if(!FindClosestInputs(ResourceType, InputResourceTypeforClass[ResourceType], InputResourceTypeforNumActorstoFind[ResourceType], InputResourceActors))
								FailtoFind();
						}
					}
				}
				else
					FailtoFind();
				
			}
		}
		else
		{
			BuildManager->PlaceEditorObject(this, RowName);
			SetIsConstructing(false); // 건축 전/ 건축 후 상태 설정
			BuildCompleted();
		}
	}
}

void AHappinessBuilding::FailtoFind()
{
	// Remove ResourceManager's Actors
	for(AActor* unit : UnitActors)
		RemoveResourceActor(unit);
	for(AActor* Resource : InputResourceActors)
		RemoveResourceActor(Resource);
	
	// 실패 시 비활성화 ui 호출
	K2_OnDeactivateCheckedState();
}

