// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Build/BuildManager.h"
#include "PlaceableObject/PlaceableObjectBase.h"
#include "HappinessBuilding.generated.h"

/**
 * 
 */
UCLASS()
class BUILDSIMULATION_API AHappinessBuilding : public APlaceableObjectBase
{
	GENERATED_BODY()
	AHappinessBuilding();

	UPROPERTY()
	ABuildManager* BuildManager;
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnDeactivateCheckedState")) //bp에서 Display 시킬 함수 이름
	void K2_OnDeactivateCheckedState();

	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnDeactivateCheckedState")) //bp에서 Display 시킬 함수 이름
	void K2_MoveToItem();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	TMap<int32, int32> InputResourceTypeforNumActorstoFind;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	TMap<int32, TSubclassOf<AActor> > InputResourceTypeforClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cycle", meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> UnitActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cycle", meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> InputResourceActors;

	UFUNCTION(BlueprintCallable, Category = "Find Actors")
	void FailtoFind();

	
};
