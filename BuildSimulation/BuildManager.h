#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlaceableObjectBase.h"
#include "PlaceableObjectsData.h"
#include "BuildManager.generated.h"

DECLARE_DELEGATE_OneParam(FUpdateResourceAmount, FConstructionCost PlayerResources);

UCLASS()
class BUILDSIMULATION_API ABuildManager : public AActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	ABuildManager();
	FUpdateResourceAmount UpdateResourceAmountEvent;
	
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void BuildPlaceableObject();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "GridSystem")
	void SpawnGrid(FVector CenterLocation, FVector TileSize, FIntPoint TileCount, bool UseEnvironment=false);

	UFUNCTION(BlueprintCallable, Category = "Cell")
	TArray<FIntPoint> GetCellsinRectangularArea(FVector CenterLocation, FIntPoint TileCount);

	UFUNCTION(BlueprintCallable, Category = "Data|Occupancy")
	void SetOccupancyData(FIntPoint Cell, bool IsOccupied);

	UFUNCTION(BlueprintCallable, Category = "Data|Object")
	void SetObjectData(FIntPoint Cell, APlaceableObjectBase* PlaceableObject);
	
	FORCEINLINE const FVector GetGridCenterLocation() const { return GridCenterLocation; }
	FORCEINLINE void SetGridCenterLocation(const FVector& InLocation) { GridCenterLocation = InLocation; }

	FORCEINLINE FVector GetGridTileSize() const { return GridTileSize; }
	FORCEINLINE void SetGridTileSize(const FVector& InGridTileSize) { GridTileSize = InGridTileSize; }

	FORCEINLINE FIntPoint GetGridTileCount() const { return GridTileCount; }
	FORCEINLINE void SetGridTileCount(const FIntPoint& InGridTileCount) { GridTileCount = InGridTileCount; }

	FORCEINLINE bool GetDemolitionToolEnabled() const { return DemolitionToolEnabled; }
	FORCEINLINE void SetDemolitionToolEnabled(const bool& InDemolitionToolEnabled) { DemolitionToolEnabled = InDemolitionToolEnabled; }

protected:
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* InstancedStaticMeshComponent;

	UFUNCTION(BlueprintCallable, Category = "Main")
	void UpdateResouresValue(FConstructionCost Resource, bool Add, bool Subtract);

	UFUNCTION(BlueprintCallable, Category = "GridSystem")
	void SetGridOffsetFromGround(float Offset);

	UFUNCTION(BlueprintCallable, Category = "Main")
	void PressedLMB();
	

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnGrid", meta = (AllowPrivateAccess = "true"))
	FVector GridCenterLocation = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnGrid", meta = (AllowPrivateAccess = "true"))
	FVector GridTileSize = FVector(200.0f, 200.0f, 100.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnGrid", meta = (AllowPrivateAccess = "true"))
	FIntPoint GridTileCount = FIntPoint(3, 3);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnGrid", meta = (AllowPrivateAccess = "true"))
	FVector GridBottomLeftCornerLocation = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnGrid", meta = (AllowPrivateAccess = "true"))
	bool IsTileMap = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnGrid", meta = (AllowPrivateAccess = "true"))
	float GridOffsetFromGround = -2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact", meta = (AllowPrivateAccess = "true"))
	bool InteractStarted = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact", meta = (AllowPrivateAccess = "true"))
	APlaceableObjectBase* PlaceableObjectUnderCursor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact", meta = (AllowPrivateAccess = "true"))
	APlaceableObjectBase* SelectedPlaceableObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact", meta = (AllowPrivateAccess = "true"))
	bool PlaceableObjectSelected = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enabled", meta = (AllowPrivateAccess = "true"))
	bool BuildToolEnabled = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enabled", meta = (AllowPrivateAccess = "true"))
	bool DemolitionToolEnabled = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	TMap<FIntPoint, int32> OccupancyData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	TMap<FIntPoint, APlaceableObjectBase*> ObjectData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	APlayerController* PlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	FConstructionCost PlayerResources;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class APlaceableObjectBase> PlaceableObjectBaseClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object", meta = (AllowPrivateAccess = "true"))
	APlaceableObjectBase* PlaceableObjectBase;
	
	// Funtion
	FVector SnapVectorToVector(FVector V1, FVector V2);
	
	float SnapFlaotToFloat(float CurrentLocation, float GridSize);
	
	void CalculateCenterandBottomLeft();
	
	bool TraceforGround(FVector& Location);
	
	void SelectPlaceableObject();
	

	// Getter & Setter
	FORCEINLINE FVector GetGridBottomLeftCornerLocation() const { return GridBottomLeftCornerLocation; }
	FORCEINLINE void SetGridBottomLeftCornerLocation(const FVector& InLocation) { GridBottomLeftCornerLocation = InLocation; }

	FORCEINLINE bool GetInteractStarted() const { return InteractStarted; }
	FORCEINLINE void SetInteractStarted(const bool& InInteractStarted) { InteractStarted = InInteractStarted; }

	FORCEINLINE bool GetBuildToolEnabled() const { return BuildToolEnabled; }
	FORCEINLINE void SetBuildToolEnabled(const bool& InBuildToolEnabled) { BuildToolEnabled = InBuildToolEnabled; }

	FORCEINLINE APlaceableObjectBase* GetPlaceableObjectUnderCursor() const { return PlaceableObjectUnderCursor; }
	FORCEINLINE void SetPlaceableObjectUnderCursor(APlaceableObjectBase* InPlaceableObjectUnderCursor) { PlaceableObjectUnderCursor = InPlaceableObjectUnderCursor; }

	FORCEINLINE APlaceableObjectBase* GetSelectedPlaceableObject() const { return SelectedPlaceableObject; }
	FORCEINLINE void SetSelectedPlaceableObject(APlaceableObjectBase* InSelectedPlaceableObject) { SelectedPlaceableObject = InSelectedPlaceableObject; }

	FORCEINLINE bool GetPlaceableObjectSelected() const { return PlaceableObjectSelected; }
	FORCEINLINE void SetPlaceableObjectSelected(const bool& InPlaceableObjectSelected) { PlaceableObjectSelected = InPlaceableObjectSelected; }

	FORCEINLINE TMap<FIntPoint, int32> GetOccupancyData() const { return OccupancyData; }
	FORCEINLINE void SetOccupancyData(const TMap<FIntPoint, int32> InOccupancyData) { OccupancyData = InOccupancyData; }

	FORCEINLINE TMap<FIntPoint, APlaceableObjectBase*> GetObjectData() const { return ObjectData; }
	FORCEINLINE void SetObjectData(const TMap<FIntPoint, APlaceableObjectBase*> InObjectData) { ObjectData = InObjectData; }

	FORCEINLINE APlayerController* GetPlayerController() const {return PlayerController;}
	FORCEINLINE void SetPlayerController(APlayerController* InPlayerController) { PlayerController = InPlayerController; }

	FORCEINLINE FConstructionCost GetPlayerResources() const {return PlayerResources;}
	FORCEINLINE void SetPlayerResources(const FConstructionCost InPlayerResources) { PlayerResources = InPlayerResources; }

	FORCEINLINE APlaceableObjectBase* GetPlaceableObjectBase() const {return PlaceableObjectBase;}
	FORCEINLINE void SetPlaceableObjectBase(APlaceableObjectBase* InPlaceableObjectBase) { PlaceableObjectBase = InPlaceableObjectBase; }
	
};
