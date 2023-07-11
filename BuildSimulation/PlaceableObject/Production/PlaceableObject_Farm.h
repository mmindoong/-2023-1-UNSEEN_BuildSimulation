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

	// �켱���� ť���� �Ÿ��� ���� ������ ���ĵǵ��� ������ �����ε�
	bool operator<(const FActorDistancePair& Other) const
	{
		return Distance > Other.Distance;
	}
};
