#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlaceableObjectBase.h"
#include "PlacerObjectBase.h"
#include "PlaceableObjectsData.h"
#include "BuildManager.generated.h"

/* Delegate Declare */
DECLARE_DELEGATE_TwoParams(FUpdateResourceAmount, FConstructionCost NewResourceAmout, bool IsEnoughResource);

UCLASS()
class BUILDSIMULATION_API ABuildManager : public AActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	ABuildManager();

	/* Delegate Instance */
	FUpdateResourceAmount FUpdateResourceAmountEvent;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaTime) override;

	/* Delegate Event Dispatchers  */
	UFUNCTION(BlueprintCallable, Category = "Main")
	void CallUpdatePlaceableObjectUnderCursor(APlaceableObjectBase* PlaceableObjectBase, bool IsRemove);

public:
	/* Main Functions */
	UFUNCTION(BlueprintCallable, Category = "Main")
	void BuildPlaceableObject();
	
	UFUNCTION(BlueprintCallable, Category = "Main")
	void PressedLMB();

	UFUNCTION(BlueprintCallable, Category = "Main")
	void ReleasedLMB();

	UFUNCTION(BlueprintCallable, Category = "Main")
	void UpdateBuildingManagerValues();
	
	UFUNCTION(BlueprintCallable, Category = "Main")
	void SelectPlaceableObject();

	UFUNCTION(BlueprintCallable, Category = "Main")
	void DeselectPlaceableObject();

	UFUNCTION(BlueprintCallable, Category = "Main")
	void CancelDragObjectPlacing();

	UFUNCTION(BlueprintCallable, Category = "Main")
	void DetectMouseDrag();

	UFUNCTION(BlueprintCallable, Category = "Main")
	void RotateObject(bool bLeft);
	
	/* GridSystem Functions */
	UFUNCTION(BlueprintCallable, Category = "GridSystem")
	void SpawnGrid(FVector CenterLocation, FVector TileSize, FIntPoint TileCount);

	UFUNCTION(BlueprintCallable, Category = "GridSystem")
	TArray<FIntPoint> GetCellsinRectangularArea(FVector CenterLocation, FIntPoint TileCount);

	/* Data Functions */
	UFUNCTION(BlueprintCallable, Category = "Data|Occupancy")
	void SetOccupancyData(FIntPoint Cell, bool IsOccupied);

	UFUNCTION(BlueprintCallable, Category = "Data|Object")
	void SetObjectData(FIntPoint Cell, APlaceableObjectBase* PlaceableObject);

	/* Tool Functions */
	UFUNCTION(BlueprintCallable, Category="Tool")
	void ActivateBuildingTool(FDataTableRowHandle ObjectforBuilding);

	UFUNCTION(BlueprintCallable, Category="Tool")
	void DeactivateBuildingTool();

	UFUNCTION(BlueprintCallable, Category="Tool")
	void ActivateDemolitionTool();

	UFUNCTION(BlueprintCallable, Category="Tool")
	void DeactivateDemolitionTool();
	
	/* Resource Functions */
	UFUNCTION(BlueprintCallable, Category = "Resource")
	void UpdateResourcesValue(FConstructionCost Resource, bool Add, bool Subtract);
	
	/* Public Getter & Setter */
	FORCEINLINE const FVector GetGridCenterLocation() const { return GridCenterLocation; }
	FORCEINLINE void SetGridCenterLocation(const FVector& InLocation) { GridCenterLocation = InLocation; }

	FORCEINLINE FVector GetGridTileSize() const { return GridTileSize; }
	FORCEINLINE void SetGridTileSize(const FVector& InGridTileSize) { GridTileSize = InGridTileSize; }

	FORCEINLINE FIntPoint GetGridTileCount() const { return GridTileCount; }
	FORCEINLINE void SetGridTileCount(const FIntPoint& InGridTileCount) { GridTileCount = InGridTileCount; }

	FORCEINLINE bool GetbDemolitionToolEnabled() const { return bDemolitionToolEnabled; }
	FORCEINLINE void SetbDemolitionToolEnabled(const bool& InDemolitionToolEnabled) { bDemolitionToolEnabled = InDemolitionToolEnabled; }

protected:
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* InstancedStaticMeshComponent;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;
	

private:
	FPlaceableObjectData* PlaceableObjectData;
	
	/* Settings Variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Grid", meta = (AllowPrivateAccess = "true"))
	FVector GridCenterLocation = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Grid", meta = (AllowPrivateAccess = "true"))
	FVector GridTileSize = FVector(200.0f, 200.0f, 100.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Grid", meta = (AllowPrivateAccess = "true"))
	FIntPoint GridTileCount = FIntPoint(3, 3);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Grid", meta = (AllowPrivateAccess = "true"))
	FVector GridBottomLeftCornerLocation = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Grid", meta = (AllowPrivateAccess = "true"))
	float GridOffsetFromGround = -2.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	FLinearColor PlayerOutlineColor = FLinearColor::Red;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	FLinearColor EnemyOutlineColor = FLinearColor::Red;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	FLinearColor NeturalOutlineColor = FLinearColor::Yellow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	UMaterialParameterCollection* Collection;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	UMaterialParameterCollectionInstance* PCI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	bool bTraceComplex = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	float StartDragInstance;
	
	/* Interact Variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact", meta = (AllowPrivateAccess = "true"))
	bool bInteractStarted = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact", meta = (AllowPrivateAccess = "true"))
	bool bPlaceableObjectSelected = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact", meta = (AllowPrivateAccess = "true"))
	bool bDragStarted = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact", meta = (AllowPrivateAccess = "true"))
	bool bDragWasInterrupted = false;

	/* Enabled Variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enabled", meta = (AllowPrivateAccess = "true"))
	bool bObjectForPlacementIsSelected = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enabled", meta = (AllowPrivateAccess = "true"))
	bool bBuildToolEnabled = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enabled", meta = (AllowPrivateAccess = "true"))
	bool bDemolitionToolEnabled = false;

	/* Data Variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	TMap<FIntPoint, int32> OccupancyData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	TMap<FIntPoint, APlaceableObjectBase*> ObjectData;

	/* Player Variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	APlayerController* PlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	FConstructionCost PlayerResources;

	/* Object Variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class APlaceableObjectBase> PlaceableObjectBaseClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class APlacerObjectBase> PlacerObjectBaseClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object", meta = (AllowPrivateAccess = "true"))
	APlaceableObjectBase* PlaceableObjectBase;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object", meta = (AllowPrivateAccess = "true"))
	APlaceableObjectBase* PlaceableObjectUnderCursor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object", meta = (AllowPrivateAccess = "true"))
	APlaceableObjectBase* SelectedPlaceableObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object", meta = (AllowPrivateAccess = "true"))
	APlacerObjectBase* ActivePlacer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object", meta = (AllowPrivateAccess = "true"))
	FDataTableRowHandle ObjectForPlacement;

	/* UnderCursor Variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnderCursor", meta = (AllowPrivateAccess = "true"))
	FVector LocationUnderCursorCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnderCursor", meta = (AllowPrivateAccess = "true"))
	FVector LocationUnderCursorVisibility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnderCursor", meta = (AllowPrivateAccess = "true"))
	AActor* ActorUnderCursor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnderCursor", meta = (AllowPrivateAccess = "true"))
	FIntPoint CellUnderCursor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnderCursor", meta = (AllowPrivateAccess = "true"))
	FIntPoint LastCellUnderCursor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnderCursor", meta = (AllowPrivateAccess = "true"))
	bool bCellUnderCursorHasChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnderCursor", meta = (AllowPrivateAccess = "true"))
	FVector StartLocationUnderCursor;
	
	/* Local Function Library */
	FVector LSnapVectorToVector(FVector V1, FVector V2);
	
	float LSnapFlaotToFloat(float CurrentLocation, float GridSize);
	
	void LCalculateCenterandBottomLeft();

	void LSetOutlineColor(int32 ObjectSide);
	
	void LChangeObjectforPlacement(FDataTableRowHandle NewObjectRow);

	FIntPoint LGetCellfromWorldLocation(FVector Location);

	bool LCheckifEnoughResources(FConstructionCost InCost);

	/* Getter & Setter */
	FORCEINLINE FVector GetGridBottomLeftCornerLocation() const { return GridBottomLeftCornerLocation; }
	FORCEINLINE void SetGridBottomLeftCornerLocation(const FVector& InLocation) { GridBottomLeftCornerLocation = InLocation; }

	FORCEINLINE bool GetbObjectForPlacementIsSelected() const { return bObjectForPlacementIsSelected; }
	FORCEINLINE void SetbObjectForPlacementIsSelected(bool InbObjectForPlacementIsSelected) { bObjectForPlacementIsSelected = InbObjectForPlacementIsSelected; }

	FORCEINLINE bool GetbInteractStarted() const { return bInteractStarted; }
	FORCEINLINE void SetbInteractStarted(const bool& InInteractStarted) { bInteractStarted = InInteractStarted; }

	FORCEINLINE bool GetbBuildToolEnabled() const { return bBuildToolEnabled; }
	FORCEINLINE void SetbBuildToolEnabled(const bool& InBuildToolEnabled) { bBuildToolEnabled = InBuildToolEnabled; }

	FORCEINLINE APlaceableObjectBase* GetPlaceableObjectUnderCursor() const { return PlaceableObjectUnderCursor; }
	FORCEINLINE void SetPlaceableObjectUnderCursor(APlaceableObjectBase* InPlaceableObjectUnderCursor) { PlaceableObjectUnderCursor = InPlaceableObjectUnderCursor; }

	FORCEINLINE APlaceableObjectBase* GetSelectedPlaceableObject() const { return SelectedPlaceableObject; }
	FORCEINLINE void SetSelectedPlaceableObject(APlaceableObjectBase* InSelectedPlaceableObject) { SelectedPlaceableObject = InSelectedPlaceableObject; }

	FORCEINLINE bool GetbPlaceableObjectSelected() const { return bPlaceableObjectSelected; }
	FORCEINLINE void SetbPlaceableObjectSelected(const bool& InPlaceableObjectSelected) { bPlaceableObjectSelected = InPlaceableObjectSelected; }

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

	FORCEINLINE UMaterialParameterCollectionInstance* GetPCI() const { return PCI; }
	FORCEINLINE void SetPCI(UMaterialParameterCollectionInstance* InPCI) { PCI = InPCI; }

	FORCEINLINE bool GetbTraceComplex() const { return bTraceComplex; }
	FORCEINLINE void SetbTraceComplex(bool InbTraceComplex) { bTraceComplex = InbTraceComplex; }

	FORCEINLINE float GetStartDragInstance() const { return StartDragInstance; }
	FORCEINLINE void SetStartDragInstance(float InStartDragInstance) { StartDragInstance= InStartDragInstance; }

	FORCEINLINE bool GetbDragStarted() const { return bDragStarted; }
	FORCEINLINE void SetbDragStarted(bool InbDragStarted) { bDragStarted = InbDragStarted; }

	FORCEINLINE bool GetbDragWasInterrupted() const { return bDragWasInterrupted; }
	FORCEINLINE void SetbDragWasInterrupted(bool InbDragWasInterrupted) { bDragWasInterrupted = InbDragWasInterrupted; }

	FORCEINLINE APlacerObjectBase* GetActivePlacer() const { return ActivePlacer; }
	FORCEINLINE void SetActivePlacer(APlacerObjectBase* InActivatePlacer) { ActivePlacer = InActivatePlacer; }

	FORCEINLINE FDataTableRowHandle GetObjectForPlacement() const { return ObjectForPlacement; }
	FORCEINLINE void SetObjectForPlacement(FDataTableRowHandle InObjectForPlacement) { ObjectForPlacement = InObjectForPlacement; }

	FORCEINLINE FPlaceableObjectData* GetPlaceableObjectData() const { return PlaceableObjectData; }
	FORCEINLINE void SetPlaceableObjectData(FPlaceableObjectData* InPlaceableObjectData) { PlaceableObjectData = InPlaceableObjectData; }

	FORCEINLINE FVector GetLocationUnderCursorCamera() const { return LocationUnderCursorCamera; }
	FORCEINLINE void SetLocationUnderCursorCamera(FVector InLocationUnderCursorCamera) { LocationUnderCursorCamera = InLocationUnderCursorCamera; }

	FORCEINLINE FVector GetLocationUnderCursorVisibility() const { return LocationUnderCursorVisibility; }
	FORCEINLINE void SetLocationUnderCursorVisibility(FVector InLocationUnderCursorVisibility) { LocationUnderCursorVisibility = InLocationUnderCursorVisibility; }

	FORCEINLINE AActor* GetActorUnderCursor() const { return ActorUnderCursor; }
	FORCEINLINE void SetActorUnderCursor(AActor* InActorUnderCursor) { ActorUnderCursor = InActorUnderCursor; }

	FORCEINLINE FIntPoint GetCellUnderCursor() const { return CellUnderCursor; }
	FORCEINLINE void SetCellUnderCursor(FIntPoint InCellUnderCursor) { CellUnderCursor = InCellUnderCursor; }

	FORCEINLINE FIntPoint GetLastCellUnderCursor() const { return LastCellUnderCursor; }
	FORCEINLINE void SetLastCellUnderCursor(FIntPoint InLastCellUnderCursor) { LastCellUnderCursor = InLastCellUnderCursor; }

	FORCEINLINE bool GetbCellUnderCursorHasChanged() const { return bCellUnderCursorHasChanged; }
	FORCEINLINE void SetbCellUnderCursorHasChanged(bool InbCellUnderCursorHasChanged) { bCellUnderCursorHasChanged= InbCellUnderCursorHasChanged ;}

	FORCEINLINE FVector GetStartLocationUnderCursor() const { return StartLocationUnderCursor; }
	FORCEINLINE void SetStartLocationUnderCursor(FVector InStartLocationUnderCursor) { StartLocationUnderCursor = InStartLocationUnderCursor; }
	
};
