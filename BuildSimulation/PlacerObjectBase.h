// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlaceableObjectsData.h"
#include "DynamicPlaceableObjectData.h"
#include "PlacerObjectBase.generated.h"

class APlaceableObjectBase;
UCLASS()
class BUILDSIMULATION_API APlacerObjectBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlacerObjectBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Delegate Event Dispatchers  */
	UFUNCTION()
	void CallUpdateResourceAmount(FConstructionCost NewResourceAmount, bool IsEnoughResource);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* Parent Functions */
	UFUNCTION(BlueprintCallable, Category = "ParentFunctions")
	void SetupObjectPlacer();

	UFUNCTION(BlueprintCallable, Category = "ParentFunctions")
	void ActivateObjectPlacer();

	UFUNCTION(BlueprintCallable, Category = "ParentFunctions")
	void DeactivateObjectPlacer();

	UFUNCTION(BlueprintCallable, Category = "ParentFunctions")
	void RotateObjectPlacer(bool bLeft);

	UFUNCTION(BlueprintCallable, Category = "ParentFunctions")
	void HidePlaceIndicators();

	UFUNCTION(BlueprintCallable, Category = "ParentFunctions")
	void CreateIndicatorMesh(bool bPlaceEnabled);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ObjectMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMesh* IndicatorMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* PlaceAcceptedMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* PlaceRejectedMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placer", meta = (AllowPrivateAccess = "true"))
	TArray<UStaticMeshComponent*> PlaceIndicators;
	
	
	/* Public Getter & Setter */
	FORCEINLINE bool GetbCanbeDraggged() const { return bCanbeDraggged; }
	FORCEINLINE void SetbCanbeDraggged(bool InbCanbeDraggged) { bCanbeDraggged = InbCanbeDraggged; }

	FORCEINLINE FPlaceableObjectData* GetObjectData() const { return ObjectData; }
	FORCEINLINE void SetObjectData(FPlaceableObjectData* InObjectData) { ObjectData = InObjectData; }

	FORCEINLINE FIntPoint GetObjectSize() const { return ObjectSize; }
	FORCEINLINE void SetObjectSize(FIntPoint InObjectSize) { ObjectSize = InObjectSize; }

	FORCEINLINE TArray<UStaticMeshComponent*> GetPlaceIndicators() { return PlaceIndicators; }
	FORCEINLINE void SetPlaceIndicators(TArray<UStaticMeshComponent*> InPlaceIndicators) { PlaceIndicators = InPlaceIndicators; }

	FORCEINLINE FDataTableRowHandle GetObjectNameInTable() { return ObjectNameInTable; }
	FORCEINLINE void SetObjectNameInTable(FDataTableRowHandle InObjectNameInTable) { ObjectNameInTable = InObjectNameInTable; }

	FORCEINLINE int32 GetBuildDirection() const { return BuildDirection; }
	FORCEINLINE void SetBuildDirection(int32 InBuildDirection) { BuildDirection = InBuildDirection; }

private:
	FPlaceableObjectData* ObjectData;

	/* Setting Variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	class UDataTable* PlaceableObjectTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	FDataTableRowHandle ObjectNameInTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* BuildingAcceptedMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* BuildingRejectedMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	bool bCanbeDraggged;

	/* Object Variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APlaceableObjectBase> PlaceableObjectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object", meta = (AllowPrivateAccess = "true"))
	FIntPoint ObjectSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object", meta = (AllowPrivateAccess = "true"))
	int32 BuildDirection;
	
	/* Placer Variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placer", meta = (AllowPrivateAccess = "true"))
	FVector PlacerScale;
	
	/* Getter & Setter */
	FORCEINLINE TSubclassOf<APlaceableObjectBase> GetPlaceableObjectClass() { return PlaceableObjectClass; }
	FORCEINLINE void SetPlaceableObjectClass(TSubclassOf<APlaceableObjectBase> InPlaceableObjectClass) { PlaceableObjectClass = InPlaceableObjectClass; }
	
	FORCEINLINE FVector GetPlacerScale() { return PlacerScale; }
	FORCEINLINE void SetPlacerScale(FVector InPlacerScale) { PlacerScale = InPlacerScale; }
};

