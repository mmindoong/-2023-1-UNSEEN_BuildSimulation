// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/ObjectData.h"
#include "Placer.generated.h"

class APlaceableObjectBase;
UCLASS()
class BUILDSIMULATION_API APlacer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlacer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	/* Parent Functions */
	UFUNCTION(BlueprintCallable, Category = "ParentFunctions")
	void SetupObjectPlacer();

	UFUNCTION(BlueprintCallable, Category = "ParentFunctions")
	void ActivateObjectPlacer();

	UFUNCTION(BlueprintCallable, Category = "ParentFunctions")
	void DeactivateObjectPlacer();

	UFUNCTION(BlueprintCallable, Category = "ParentFunctions")
	void Rotate(bool bLeft);
	
	UFUNCTION(BlueprintCallable, Category = "ParentFunctions")
	void CreateIndicatorMesh(bool bPlaceEnabled);

	UFUNCTION(BlueprintCallable, Category = "ParentFunctions")
	void UpdateMeshMaterial(bool bIsEnoughResource);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Facility", meta = (AllowPrivateAccess = "true"))
	TArray<FIntPoint> ObjectCellLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> ObjectMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMesh* IndicatorMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	int32 ResourceType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* PlaceAcceptedMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* PlaceRejectedMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placer", meta = (AllowPrivateAccess = "true"))
	TArray<UStaticMeshComponent*> PlaceIndicators;
	
	
	/* Public Getter & Setter */
	FORCEINLINE FObjectData* GetObjectData() const { return ObjectData; }
	FORCEINLINE void SetObjectData(FObjectData* InObjectData) { ObjectData = InObjectData; }

	FORCEINLINE FIntPoint GetObjectSize() const { return ObjectSize; }
	FORCEINLINE void SetObjectSize(FIntPoint InObjectSize) { ObjectSize = InObjectSize; }

	FORCEINLINE TArray<UStaticMeshComponent*> GetPlaceIndicators() { return PlaceIndicators; }
	FORCEINLINE void SetPlaceIndicators(TArray<UStaticMeshComponent*> InPlaceIndicators) { PlaceIndicators = InPlaceIndicators; }

	FORCEINLINE int32 GetBuildDirection() const { return BuildDirection; }
	FORCEINLINE void SetBuildDirection(int32 InBuildDirection) { BuildDirection = InBuildDirection; }

	FORCEINLINE FName GetRowName() const { return RowName; }
	FORCEINLINE void SetRowName(FName InRowName) { RowName = InRowName; }

	FORCEINLINE int32 GetResourceType() const { return ResourceType; }
	FORCEINLINE void SetResourceType(int32 InResourceType) { ResourceType = InResourceType; }

	
private:
	FObjectData* ObjectData;

	/* Setting Variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	FName RowName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* BuildingAcceptedMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* BuildingRejectedMaterial;

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

