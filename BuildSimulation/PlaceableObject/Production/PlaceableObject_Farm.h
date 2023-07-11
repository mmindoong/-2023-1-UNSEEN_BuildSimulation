// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlaceableObject/PlaceableObjectBase.h"
#include "PlaceableObject_Farm.generated.h"

/**
 * 
 */
UCLASS()
class BUILDSIMULATION_API APlaceableObject_Farm : public APlaceableObjectBase
{
	GENERATED_BODY()

	APlaceableObject_Farm();
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	ABuildManager* BuildManager;
	
	UFUNCTION(BlueprintCallable)
	bool FindClosestUnits(int32 NumActorsToFind, TArray<AActor*>& OutActors);

	UFUNCTION(BlueprintCallable)
	bool FindClosestResources(int32 NumActorsToFind, TArray<AActor*>& OutActors);
	
	TSubclassOf<AActor> Unit;
	TSubclassOf<AActor> Resource;
};

struct FActorDistancePair
{
	AActor* Actor;
	float Distance;

	FActorDistancePair(AActor* InActor, float InDistance)
		: Actor(InActor), Distance(InDistance)
	{}

	// 우선순위 큐에서 거리가 작은 순서로 정렬되도록 연산자 오버로딩
	bool operator<(const FActorDistancePair& Other) const
	{
		return Distance > Other.Distance;
	}
};
