// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/PawnUnit.h"
#include "Build/ResourceManager.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Data/PlaceableObjectsData.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlaceableObjectBase.generated.h"

/* Delegate Declare */
DECLARE_DELEGATE_TwoParams(FUpdatePlaceableObject, APlaceableObjectBase* PlaceableObject, bool IsRemove);
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

UCLASS()
class BUILDSIMULATION_API APlaceableObjectBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlaceableObjectBase();

	/* Delegate Instance */
	FUpdatePlaceableObject UpdatePlaceableObjectCursorEvent;

	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnDisplayDetailWidget")) //bp에서 Display 시킬 함수 이름
	void K2_OnDisplayDetailWidget();

	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnHideDetailWidget")) //bp에서 Display 시킬 함수 이름
	void K2_OnHideDetailWidget();

	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnUpdateHappinessData")) //bp에서 Display 시킬 함수 이름
	void K2_OnUpdateHappinessData();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/* Event Function */
	UFUNCTION()
	void OnBeginCursorOver(UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
	void OnEndCursorOver(UPrimitiveComponent* TouchedComponent);

public:	
	/* Parent Functions */
	UFUNCTION(BlueprintCallable, Category = "ParentFunctions")
	void SetupPlaceableObject();

	UFUNCTION(BlueprintCallable, Category = "ParentFunctions")
	void SetupOutline();

	UFUNCTION(BlueprintCallable, Category = "ParentFunctions")
	virtual void SetObjectSelectedMode(bool IsSelected);
	
	UFUNCTION(BlueprintCallable, Category = "ParentFunctions")
	void SwapObjectHighlighting(bool IsEnable);

	UFUNCTION(BlueprintCallable, Category = "ParentFunctions")
	void DemolitionPlaceableObject();

	UFUNCTION(BlueprintCallable, Category = "ParentFunctions")
	virtual bool BuildCompleted();

	/* Parent Function - Find Actors */
	UFUNCTION(BlueprintCallable, Category = "Find Actors")
	bool FindClosestUnits(int32 NumActorsToFind, TArray<AActor*>& OutActors);

	UFUNCTION(BlueprintCallable, Category = "Find Actors")
	bool FindClosestInputs(int32 ResourceSetType, TSubclassOf<AActor> ActorClass, int32 NumActorsToFind, TArray<AActor*>& OutActors);

	UFUNCTION(BlueprintCallable, Category = "Find Actors")
	bool FindClosestMaterialOutputs(int32 NumActorsToFind, TArray<AActor*>& OutActors);

	UFUNCTION(BlueprintCallable, Category = "Find Actors")
	bool FindClosestFoodOutpus(int32 NumActorsToFind, TArray<AActor*>& OutActors);
	
	UFUNCTION(BlueprintCallable, Category = "Find Actors")
	void RemoveResourceActor(AActor* RemoveActor);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Find Actors", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AResourceManager> ResourceManager;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> ObjectMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting", meta = (ExposeOnSpawn = "true"))
	TArray<FIntPoint> OccupiedCells;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting", meta = (ExposeOnSpawn = "true"))
	bool IsConstructing = true;

	/* Facility  Variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Facility", meta = (AllowPrivateAccess = "true"))
	FConstructionCost InputResource;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Facility", meta = (AllowPrivateAccess = "true"))
	FConstructionCost OutputResource;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Facility", meta = (AllowPrivateAccess = "true"))
	TArray<FIntPoint> ObjectCellLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Facility", meta = (AllowPrivateAccess = "true"))
	int32 HappinessFacilityType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Facility", meta = (AllowPrivateAccess = "true"))
	TArray<int32> OwnerofHappiness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting", meta = (ExposeOnSpawn = "true"))
	bool IsResidentFacility;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting", meta = (ExposeOnSpawn = "true"))
	bool IsDynamicData = true;

	FORCEINLINE bool GetIsResidentFacility() const { return IsResidentFacility; }
	FORCEINLINE void SetIsResidentFacility(bool InIsResidentFacility) { IsResidentFacility = InIsResidentFacility; }
	
	FORCEINLINE int32 GetHappinessFacilityType() const { return HappinessFacilityType; }
	FORCEINLINE void SetHappinessFacilityType(int32 InHappinessFacilityType) { HappinessFacilityType = InHappinessFacilityType; }
	
	FORCEINLINE FIntPoint GetOccupiedCenterCell() const { return OccupiedCenterCell; }
	FORCEINLINE void SetOccupiedCenterCell(FIntPoint InOccupiedCenterCell) { OccupiedCenterCell = InOccupiedCenterCell; }
	
	FORCEINLINE FIntPoint GetObjectSize() const { return ObjectSize; }
	FORCEINLINE void SetObjectSize(FIntPoint InObjectSize) { ObjectSize = InObjectSize; }

	FORCEINLINE int32 GetObjectSide() const { return ObjectSide; }
	FORCEINLINE void SetObjectSide(int32 InObjectSide) { ObjectSide = InObjectSide; }

	FORCEINLINE FPlaceableObjectData* GetObjectData() const { return ObjectData; }
	FORCEINLINE void SetObjectData(FPlaceableObjectData* InObjectData) { ObjectData = InObjectData; }

	FORCEINLINE FName GetRowName() const { return RowName; }
	FORCEINLINE void SetRowName(FName InRowName) { RowName = InRowName; }

	FORCEINLINE int32 GetBuildDirection() const { return BuildDirection; }
	FORCEINLINE void SetBuildDirection(int32 InBuildDirection) { BuildDirection = InBuildDirection; }

	FORCEINLINE bool GetIsConstructing() const { return IsConstructing; }
	FORCEINLINE void SetIsConstructing(bool InIsConstructing) { IsConstructing = InIsConstructing; }

	FORCEINLINE bool GetIsConstructionFacility() const { return IsConstructionFacility; }
	FORCEINLINE void SetIsConstructionFacility(bool InIsConstructionFacility) { IsConstructionFacility = InIsConstructionFacility; }
private:
	/* Local Function Library */
	void LSwapObjectOutline(bool IsEnable);

	FPlaceableObjectData* ObjectData;

	/* Setting Variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	FName RowName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	int32 ObjectSide = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	float StartingHealthPercent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	int32 BuildDirection = 0;
	
	/* ObjectData Variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Data", meta = (AllowPrivateAccess = "true"))
	FIntPoint ObjectSize = FIntPoint(1,1);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Data", meta = (AllowPrivateAccess = "true"))
	bool bOutlineEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Data", meta = (AllowPrivateAccess = "true"))
	bool bHPBarEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Data", meta = (AllowPrivateAccess = "true"))
	float HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Data", meta = (AllowPrivateAccess = "true"))
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Data", meta = (AllowPrivateAccess = "true"))
	int32 ObjectDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Data", meta = (AllowPrivateAccess = "true"))
	FIntPoint OccupiedCenterCell = FIntPoint(0, 0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Data", meta = (AllowPrivateAccess = "true"))
	float ObjectHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Data", meta = (AllowPrivateAccess = "true"))
	bool IsConstructionFacility = false;

	/* Object Variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object", meta = (AllowPrivateAccess = "true"))
	TArray<UStaticMeshComponent*> Meshesforoutline;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object", meta = (AllowPrivateAccess = "true"))
	bool bObjectSelected;
	

	/* Getter & Setter */
	FORCEINLINE bool GetbOutlineEnabled() const { return bOutlineEnabled; }
	FORCEINLINE void SetbOutlineEnabled(bool InOutlineEnabled) { bOutlineEnabled = InOutlineEnabled; }

	FORCEINLINE bool GetbHPBarEnabled() const { return bHPBarEnabled; }
	FORCEINLINE void SetbHPBarEnabled(bool InHPBarEnabled) { bHPBarEnabled = InHPBarEnabled; }

	FORCEINLINE float GetHP() const { return HP; }
	FORCEINLINE void SetHP(float InHP) { HP = InHP; }

	FORCEINLINE float GetMaxHP() const { return MaxHP; }
	FORCEINLINE void SetMaxHP(float InMaxHP) { HP = InMaxHP; }

	FORCEINLINE int32 GetObjectDirection() const { return ObjectDirection; }
	FORCEINLINE void SetObjectDirection(int InObjectDirection) { ObjectDirection = InObjectDirection; }
	
	FORCEINLINE float GetObjectHeight() const { return ObjectHeight; }
	FORCEINLINE void SetObjectHeight(float InObjectHeight) { ObjectHeight = InObjectHeight; }

	FORCEINLINE TArray<UStaticMeshComponent*> GetMeshesforoutline() const { return Meshesforoutline; }
	FORCEINLINE void SetMeshesforoutline(TArray<UStaticMeshComponent*> InMeshesforoutline) { Meshesforoutline = InMeshesforoutline; }

	FORCEINLINE float GetStartingHealthPercent() const { return StartingHealthPercent; }
	FORCEINLINE void SetStartingHealthPercent(float InStartingHealthPercent) { StartingHealthPercent = InStartingHealthPercent; }

	FORCEINLINE bool GetbObjectSelected() const { return bObjectSelected; }
	FORCEINLINE void SetbObjectSelected(bool InObjectSelected) { bObjectSelected = InObjectSelected; }
	
};

