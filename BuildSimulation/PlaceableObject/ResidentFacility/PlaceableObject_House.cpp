// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaceableObject/ResidentFacility/PlaceableObject_House.h"
#include "Build/BuildManager.h"
#include <algorithm>
#include <queue>

#include "AI/PawnUnit.h"

APlaceableObject_House::APlaceableObject_House() 
{
	InputResource = FConstructionCost();
	FConstructionCost Output = FConstructionCost(0, FFoodData(), 0, 0,0, 0, FCitizen(1, 0, 0, 0));
	OutputResource = Output;
	
	
	static ConstructorHelpers::FClassFinder<AActor> PAWNDATA(TEXT("/Game/Blueprints/Core/Unit/BP_UnitPawn.BP_UnitPawn_C"));
	if(PAWNDATA.Succeeded())
	{
		Unit = PAWNDATA.Class;
	}

	static ConstructorHelpers::FClassFinder<AActor> ROCK_DATA(TEXT("/Game/Blueprints/Core/ItemActor/BP_Rock.BP_Rock_C"));
	if(ROCK_DATA.Succeeded())
	{
		Resource = ROCK_DATA.Class;
	}
	
	if(UGameplayStatics::GetActorOfClass(GetWorld(), ABuildManager::StaticClass()))
	{
		BuildManager = Cast<ABuildManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ABuildManager::StaticClass()));	
	}
}

void APlaceableObject_House::BeginPlay()
{
	Super::BeginPlay();
}


bool APlaceableObject_House::FindClosestUnits(int32 NumActorsToFind, TArray<AActor*>& OutActors)
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

bool APlaceableObject_House::FindClosestResources(int32 NumActorsToFind, TArray<AActor*>& OutActors)
{
	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), Resource, AllActors);
	
	if(AllActors.Num() < NumActorsToFind)
		return false;
	// �켱���� ť�� ����Ͽ� �Ÿ��� ���� ������� ���ĵ� n���� ���͸� ����
	std::priority_queue<FActorDistancePair> ClosestActorsQueue;

	for (AActor* Actor : AllActors)
	{
		if(BuildManager->ResourceActorComponent->Rocks.Find(Actor) != -1)
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
		BuildManager->ResourceActorComponent->Rocks.Add(ClosestActorsQueue.top().Actor);
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

