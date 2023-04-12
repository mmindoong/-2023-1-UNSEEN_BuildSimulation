// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "PlaceableObjectsData.h"
#include "DynamicPlaceableObjectData.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlaceableObejct_Base_Class.generated.h"

class AGridActor;

UCLASS()
class BUILDSIMULATION_API APlaceableObejct_Base_Class : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlaceableObejct_Base_Class();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* SphereVisual;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginCursorOver(UPrimitiveComponent* TouchedComponent);

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "ParentFunctions")
	void SetupPlaceableObject();

	UFUNCTION(BlueprintCallable, Category = "ParentFunctions")
	void SetupOutline();

	UFUNCTION(BlueprintCallable, Category = "ParentFunctions")
	void EnableObjectOutline(bool IsEnable);



private:
	FPlaceableObjectData* ObjectData;
	FDynamicPlaceableObjectData data = FDynamicPlaceableObjectData();
	FDynamicPlaceableObjectData* ObjectDynamicData = &data;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Data", meta = (AllowPrivateAccess = "true"))
	FIntPoint ObjectSize = FIntPoint(1,1);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Data", meta = (AllowPrivateAccess = "true"))
	bool OutlineEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Data", meta = (AllowPrivateAccess = "true"))
	bool HPBarEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Data", meta = (AllowPrivateAccess = "true"))
	float HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Data", meta = (AllowPrivateAccess = "true"))
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Data", meta = (AllowPrivateAccess = "true"))
	int32 ObjectDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Data", meta = (AllowPrivateAccess = "true"))
	FIntPoint OccupiedCenterCell = FIntPoint(0, 0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Data", meta = (AllowPrivateAccess = "true"))
	TArray<FIntPoint> OccupiedCells;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Data", meta = (AllowPrivateAccess = "true"))
	float ObjectHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object", meta = (AllowPrivateAccess = "true"))
	TArray<UStaticMeshComponent*> Meshesforoutline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	class UDataTable* PlaceableObjectTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	FDataTableRowHandle ObjectNameInTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	int32 ObjectSide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	float StartingHealthPercent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid System", meta = (AllowPrivateAccess = "true"))
	AGridActor* BuildManager;

	// Getter & Setter
	FORCEINLINE FPlaceableObjectData* GetObjectData() const { return ObjectData; }
	FORCEINLINE void SetObjectData(FPlaceableObjectData* InObjectData) { ObjectData = InObjectData; }

	FORCEINLINE FDynamicPlaceableObjectData* GetObjectDynamicData() const { return ObjectDynamicData; }
	FORCEINLINE void SetObjectDynamicData(FDynamicPlaceableObjectData* InObjectDynamicData) { ObjectDynamicData = InObjectDynamicData; }

	FORCEINLINE FIntPoint GetObjectSize() const { return ObjectSize; }
	FORCEINLINE void SetObjectSize(FIntPoint InObjectSize) { ObjectSize = InObjectSize; }

	FORCEINLINE bool GetOutlineEnabled() const { return OutlineEnabled; }
	FORCEINLINE void SetOutlineEnabled(bool InOutlineEnabled) { OutlineEnabled = InOutlineEnabled; }

	FORCEINLINE bool GetHPBarEnabled() const { return HPBarEnabled; }
	FORCEINLINE void SetHPBarEnabled(bool InHPBarEnabled) { HPBarEnabled = InHPBarEnabled; }

	FORCEINLINE float GetHP() const { return HP; }
	FORCEINLINE void SetHP(float InHP) { HP = InHP; }

	FORCEINLINE float GetMaxHP() const { return MaxHP; }
	FORCEINLINE void SetMaxHP(float InMaxHP) { HP = InMaxHP; }

	FORCEINLINE int32 GetObjectDirection() const { return ObjectDirection; }
	FORCEINLINE void SetObjectDirection(int InObjectDirection) { ObjectDirection = InObjectDirection; }

	FORCEINLINE FIntPoint GetOccupiedCenterCell() const { return OccupiedCenterCell; }
	FORCEINLINE void SetOccupiedCenterCell(FIntPoint InOccupiedCenterCell) { OccupiedCenterCell = InOccupiedCenterCell; }

	FORCEINLINE TArray<FIntPoint> GetOccupiedCells() const { return OccupiedCells; }
	FORCEINLINE void SetOccupiedCells(TArray<FIntPoint> InOccupiedCells) { OccupiedCells = InOccupiedCells; }

	FORCEINLINE float GetObjectHeight() const { return ObjectHeight; }
	FORCEINLINE void SetObjectHeight(float InObjectHeight) { ObjectHeight = InObjectHeight; }

	FORCEINLINE TArray<UStaticMeshComponent*> GetMeshesforoutline() const { return Meshesforoutline; }
	FORCEINLINE void SetMeshesforoutline(TArray<UStaticMeshComponent*> InMeshesforoutline) { Meshesforoutline = InMeshesforoutline; }

	FORCEINLINE FDataTableRowHandle GetObjectNameInTable() const { return ObjectNameInTable; }
	FORCEINLINE void SetObjectNameInTable(FDataTableRowHandle InObjectNameInTable) { ObjectNameInTable = InObjectNameInTable; }

	FORCEINLINE float GetStartingHealthPercent() const { return StartingHealthPercent; }
	FORCEINLINE void SetStartingHealthPercent(float InStartingHealthPercent) { StartingHealthPercent = InStartingHealthPercent; }

	FORCEINLINE AGridActor* GetBuildManager() const { return BuildManager; }
	FORCEINLINE void SetBuildManager(AGridActor* InBuildManager) { BuildManager = InBuildManager; }

	FORCEINLINE int32 GetObjectSide() const { return ObjectSide; }
	FORCEINLINE void SetObjectSide(int32 InObjectSide) { ObjectSide = InObjectSide; }
};
