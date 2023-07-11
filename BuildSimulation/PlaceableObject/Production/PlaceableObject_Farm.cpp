// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaceableObject/Production/PlaceableObject_Farm.h"

#include <queue>

#include "Build/BuildManager.h"

APlaceableObject_Farm::APlaceableObject_Farm(): APlaceableObjectBase()
{
	InputResource = FConstructionCost();
	FConstructionCost Output = FConstructionCost(0, FFoodData(4, 0, 0), 0, 0,0, 0, FCitizen());
	OutputResource = Output;

	static ConstructorHelpers::FClassFinder<AActor> PAWNDATA(TEXT("/Game/Blueprints/Core/Unit/BP_UnitPawn.BP_UnitPawn_C"));
	if(PAWNDATA.Succeeded())
	{
		Unit = PAWNDATA.Class;
	}
	
	static ConstructorHelpers::FClassFinder<AActor> WOOD_DATA(TEXT("/Game/Blueprints/Core/ItemActor/BP_Wood.BP_Wood_C"));
	if(WOOD_DATA.Succeeded())
	{
		Resource = WOOD_DATA.Class;
	}
	
	if(UGameplayStatics::GetActorOfClass(GetWorld(), ABuildManager::StaticClass()))
	{
		BuildManager = Cast<ABuildManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ABuildManager::StaticClass()));	
	}
}

bool APlaceableObject_Farm::FindClosestUnits(int32 NumActorsToFind, TArray<AActor*>& OutActors)
{
	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), Unit, AllActors);

	if(AllActors.Num() < NumActorsToFind)
		return false;
	// �켱���� ť�� ����Ͽ� �Ÿ��� ���� ������� ���ĵ� n���� ���͸� ����
	std::priority_queue<FActorDistancePair> ClosestActorsQueue;

	for (AActor* Actor : AllActors)
	{
		if(BuildManager->ResourceActorComponent->Units.Find(Actor) != -1)
			continue;
		
		float Distance = FVector::Dist(Actor->GetActorLocation(), GetActorLocation());

		if (ClosestActorsQueue.size() < NumActorsToFind)
		{
			ClosestActorsQueue.emplace(Actor, Distance);
		}
		else if (Distance < ClosestActorsQueue.top().Distance)
		{
			ClosestActorsQueue.pop();
			ClosestActorsQueue.emplace(Actor, Distance);
		}
	}

	// �켱���� ť���� ���͸� �����Ͽ� ��� �迭�� �߰�
	while (!ClosestActorsQueue.empty())
	{
		OutActors.Add(ClosestActorsQueue.top().Actor);
		BuildManager->ResourceActorComponent->Units.Add(ClosestActorsQueue.top().Actor);
		ClosestActorsQueue.pop();
	}

	// ��� �迭�� �Ÿ��� ����� ������ ����
	OutActors.Sort([this](const AActor& A, const AActor& B)
	{
		float DistanceA = FVector::Dist(A.GetActorLocation(), GetActorLocation());
		float DistanceB = FVector::Dist(B.GetActorLocation(), GetActorLocation());
		return DistanceA < DistanceB;
	});
	return true;
}

bool APlaceableObject_Farm::FindClosestResources(int32 NumActorsToFind, TArray<AActor*>& OutActors)
{
	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), Resource, AllActors);
	
	if(AllActors.Num() < NumActorsToFind)
		return false;
	// �켱���� ť�� ����Ͽ� �Ÿ��� ���� ������� ���ĵ� n���� ���͸� ����
	std::priority_queue<FActorDistancePair> ClosestActorsQueue;

	for (AActor* Actor : AllActors)
	{
		if(BuildManager->ResourceActorComponent->Rices.Find(Actor) != -1)
			continue;
		
		float Distance = FVector::Dist(Actor->GetActorLocation(), GetActorLocation());

		if (ClosestActorsQueue.size() < NumActorsToFind)
		{
			ClosestActorsQueue.emplace(Actor, Distance);
		}
		else if (Distance < ClosestActorsQueue.top().Distance)
		{
			ClosestActorsQueue.pop();
			ClosestActorsQueue.emplace(Actor, Distance);
		}
	}

	// �켱���� ť���� ���͸� �����Ͽ� ��� �迭�� �߰�
	while (!ClosestActorsQueue.empty())
	{
		OutActors.Add(ClosestActorsQueue.top().Actor);
		BuildManager->ResourceActorComponent->Rices.Add(ClosestActorsQueue.top().Actor);
		ClosestActorsQueue.pop();
	}

	// ��� �迭�� �Ÿ��� ����� ������ ����
	OutActors.Sort([this](const AActor& A, const AActor& B)
	{
		float DistanceA = FVector::Dist(A.GetActorLocation(), GetActorLocation());
		float DistanceB = FVector::Dist(B.GetActorLocation(), GetActorLocation());
		return DistanceA < DistanceB;
	});
	return true;
}


