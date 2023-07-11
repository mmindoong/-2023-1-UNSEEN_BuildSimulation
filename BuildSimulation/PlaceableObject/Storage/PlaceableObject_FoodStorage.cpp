// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaceableObject/Storage/PlaceableObject_FoodStorage.h"

#include <queue>
#include "Build/BuildManager.h"

APlaceableObject_FoodStorage::APlaceableObject_FoodStorage() : APlaceableObjectBase()
{
	InputResource = FConstructionCost();
	OutputResource = FConstructionCost();

	static ConstructorHelpers::FClassFinder<AActor> PAWNDATA(TEXT("/Game/Blueprints/Core/Unit/BP_UnitPawn.BP_UnitPawn_C"));
	if(PAWNDATA.Succeeded())
	{
		Unit = PAWNDATA.Class;
	}
	
	static ConstructorHelpers::FClassFinder<AActor> IRON_DATA(TEXT("/Game/Blueprints/Core/ItemActor/BP_Iron.BP_Iron_C"));
	if(IRON_DATA.Succeeded())
	{
		Resource = IRON_DATA.Class;
	}
	
	static ConstructorHelpers::FClassFinder<AActor> RICE_DATA(TEXT("/Game/Blueprints/Core/ItemActor/BP_Rice.BP_Rice_C"));
	if(RICE_DATA.Succeeded())
	{
		Output = RICE_DATA.Class;
	}
	
	if(UGameplayStatics::GetActorOfClass(GetWorld(), ABuildManager::StaticClass()))
	{
		BuildManager = Cast<ABuildManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ABuildManager::StaticClass()));	
	}
}

bool APlaceableObject_FoodStorage::FindClosestUnits(int32 NumActorsToFind, TArray<AActor*>& OutActors)
{
	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), Unit, AllActors);

	if(AllActors.Num() < NumActorsToFind)
		return false;

	// 우선순위 큐를 사용하여 거리가 작은 순서대로 정렬된 n개의 액터를 유지
	std::priority_queue<FActorDistancePair> ClosestActorsQueue;

	if(AllActors.Num() > 0)
	{
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

		// 우선순위 큐에서 액터를 추출하여 결과 배열에 추가
		while (!ClosestActorsQueue.empty())
		{
			OutActors.Add(ClosestActorsQueue.top().Actor);
			BuildManager->ResourceActorComponent->Units.Add(ClosestActorsQueue.top().Actor);
			ClosestActorsQueue.pop();
		}

		// 결과 배열을 거리가 가까운 순서로 정렬
		OutActors.Sort([this](const AActor& A, const AActor& B)
		{
			float DistanceA = FVector::Dist(A.GetActorLocation(), GetActorLocation());
			float DistanceB = FVector::Dist(B.GetActorLocation(), GetActorLocation());
			return DistanceA < DistanceB;
		});
	}
	return true;
}

bool APlaceableObject_FoodStorage::FindClosestResources(int32 NumActorsToFind, TArray<AActor*>& OutActors)
{
	TArray<AActor*> AllActors;
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), Resource, AllActors);
	
	if(AllActors.Num() < NumActorsToFind)
		return false;
	
	// 우선순위 큐를 사용하여 거리가 작은 순서대로 정렬된 n개의 액터를 유지
	std::priority_queue<FActorDistancePair> ClosestActorsQueue;

	if(AllActors.Num() > 0)
	{
		for (AActor* Actor : AllActors)
		{
			if(BuildManager->ResourceActorComponent->Irons.Find(Actor) != -1)
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

		// 우선순위 큐에서 액터를 추출하여 결과 배열에 추가
		while (!ClosestActorsQueue.empty())
		{
			OutActors.Add(ClosestActorsQueue.top().Actor);
			BuildManager->ResourceActorComponent->Irons.Add(ClosestActorsQueue.top().Actor);
			ClosestActorsQueue.pop();
		}

		// 결과 배열을 거리가 가까운 순서로 정렬
		OutActors.Sort([this](const AActor& A, const AActor& B)
		{
			float DistanceA = FVector::Dist(A.GetActorLocation(), GetActorLocation());
			float DistanceB = FVector::Dist(B.GetActorLocation(), GetActorLocation());
			return DistanceA < DistanceB;
		});
	}
	
	return true;
}

bool APlaceableObject_FoodStorage::FindClosestOutputs(int32 NumActorsToFind, TArray<AActor*>& OutActors)
{
	TArray<AActor*> AllActors;
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), Output, AllActors);
	
	if(AllActors.Num() < NumActorsToFind)
		return false;
	// 우선순위 큐를 사용하여 거리가 작은 순서대로 정렬된 n개의 액터를 유지
	std::priority_queue<FActorDistancePair> ClosestActorsQueue;

	if(AllActors.Num() > 0)
	{
		for (AActor* Actor : AllActors)
		{
			if(BuildManager->ResourceActorComponent->Rices.Find(Actor) != -1)
				continue;

			if(IsOverlappingActor(this))
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

		// 우선순위 큐에서 액터를 추출하여 결과 배열에 추가
		while (!ClosestActorsQueue.empty())
		{
			OutActors.Add(ClosestActorsQueue.top().Actor);
			BuildManager->ResourceActorComponent->Rices.Add(ClosestActorsQueue.top().Actor);
			ClosestActorsQueue.pop();
		}

		// 결과 배열을 거리가 가까운 순서로 정렬
		OutActors.Sort([this](const AActor& A, const AActor& B)
		{
			float DistanceA = FVector::Dist(A.GetActorLocation(), GetActorLocation());
			float DistanceB = FVector::Dist(B.GetActorLocation(), GetActorLocation());
			return DistanceA < DistanceB;
		});
	}
	
	return true;
}
