// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Data/PlaceableObjectsData.h"
#include "Data/DynamicPlaceableObjectData.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlaceableObjectBase.generated.h"

/* Delegate Declare */
DECLARE_DELEGATE_TwoParams(FUpdatePlaceableObject, APlaceableObjectBase* PlaceableObject, bool IsRemove);

UCLASS()
class BUILDSIMULATION_API APlaceableObjectBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlaceableObjectBase();

	/* Delegate Instance */
	FUpdatePlaceableObject UpdatePlaceableObjectCursorEvent;

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
	void SetObjectSelectedMode(bool IsSelected);
	
	UFUNCTION(BlueprintCallable, Category = "ParentFunctions")
	void SwapObjectHighlighting(bool IsEnable);

	UFUNCTION(BlueprintCallable, Category = "ParentFunctions")
	void DemolitionPlaceableObject();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ObjectMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting", meta = (ExposeOnSpawn = "true"))
	FDataTableRowHandle ObjectNameInTable;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting", meta = (ExposeOnSpawn = "true"))
	FDynamicPlaceableObjectData ObjectDynamicData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting", meta = (ExposeOnSpawn = "true"))
	TArray<FIntPoint> OccupiedCells;
	
	FORCEINLINE FIntPoint GetOccupiedCenterCell() const { return OccupiedCenterCell; }
	FORCEINLINE void SetOccupiedCenterCell(FIntPoint InOccupiedCenterCell) { OccupiedCenterCell = InOccupiedCenterCell; }
	
	FORCEINLINE FIntPoint GetObjectSize() const { return ObjectSize; }
	FORCEINLINE void SetObjectSize(FIntPoint InObjectSize) { ObjectSize = InObjectSize; }

	FORCEINLINE FDynamicPlaceableObjectData GetObjectDynamicData() const { return ObjectDynamicData; }
	FORCEINLINE void SetObjectDynamicData(FDynamicPlaceableObjectData InObjectDynamicData) { ObjectDynamicData = InObjectDynamicData; }

	FORCEINLINE int32 GetObjectSide() const { return ObjectSide; }
	FORCEINLINE void SetObjectSide(int32 InObjectSide) { ObjectSide = InObjectSide; }

	FORCEINLINE FDataTableRowHandle GetObjectNameInTable() const { return ObjectNameInTable; }
	FORCEINLINE void SetObjectNameInTable(FDataTableRowHandle InObjectNameInTable) { ObjectNameInTable = InObjectNameInTable; }

private:
	/* Local Function Library */
	void LSwapObjectOutline(bool IsEnable);
	
	FPlaceableObjectData* ObjectData;

	/* Setting Variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	class UDataTable* PlaceableObjectTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	int32 ObjectSide = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	float StartingHealthPercent;
	
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

	/* Object Variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object", meta = (AllowPrivateAccess = "true"))
	TArray<UStaticMeshComponent*> Meshesforoutline;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object", meta = (AllowPrivateAccess = "true"))
	bool bObjectSelected;
	

	/* Getter & Setter */
	FORCEINLINE FPlaceableObjectData* GetObjectData() const { return ObjectData; }
	FORCEINLINE void SetObjectData(FPlaceableObjectData* InObjectData) { ObjectData = InObjectData; }

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
