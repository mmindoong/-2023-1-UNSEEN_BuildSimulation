// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Placer/Placer.h"
#include "PlaceableObject/PlaceableObjectBase.h"
#include "GridSystemComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDSIMULATION_API UGridSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGridSystemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* GridSystem Functions */
	UFUNCTION(BlueprintCallable, Category = "GridSystem")
	FVector GetCellLocation(FIntPoint InCell, FVector InCameraLocation, bool& bSuccess);
	
	UFUNCTION(BlueprintCallable, Category = "GridSystem")
	FIntPoint GetCellfromWorldLocation(FVector Location);

	UFUNCTION(BlueprintCallable, Category = "GridSystem")
	void CalculateCenterandBottomLeft();

	UFUNCTION(BlueprintCallable, Category = "GridSystem")
	FVector2D GetCenterOfRectangularArea(FIntPoint AreaCenterCell, FIntPoint AreaSize);

	UFUNCTION(BlueprintCallable, Category = "GridSystem")
	TArray<FIntPoint> GetCellsinRectangularArea(FIntPoint CenterLocation, FIntPoint TileCount);

	UFUNCTION(BlueprintCallable, Category = "GridSystem")
	void ChangeObjectforPlacement(FName NewObjectRow);

	UFUNCTION(BlueprintCallable, Category = "GridSystem")
	FVector2D GetCenterofCell(FIntPoint InCell);
	
	/* Tool Functions */
	UFUNCTION(BlueprintCallable, Category="Tool")
	void ActivateBuildingTool(FName ObjectforBuilding);

	UFUNCTION(BlueprintCallable, Category="Tool")
	void DeactivateBuildingTool();

	UFUNCTION(BlueprintCallable, Category="Tool")
	void ActivateDemolitionTool();

	UFUNCTION(BlueprintCallable, Category="Tool")
	void DeactivateDemolitionTool();
	
	/* Public Getter & Setter */
	FORCEINLINE const FVector GetGridCenterLocation() const { return GridCenterLocation; }
	FORCEINLINE void SetGridCenterLocation(const FVector& InLocation) { GridCenterLocation = InLocation; }

	FORCEINLINE FVector GetGridTileSize() const { return GridTileSize; }
	FORCEINLINE void SetGridTileSize(const FVector& InGridTileSize) { GridTileSize = InGridTileSize; }

	FORCEINLINE FIntPoint GetGridTileCount() const { return GridTileCount; }
	FORCEINLINE void SetGridTileCount(const FIntPoint& InGridTileCount) { GridTileCount = InGridTileCount; }
	
	FORCEINLINE FVector GetGridBottomLeftCornerLocation() const { return GridBottomLeftCornerLocation; }
	FORCEINLINE void SetGridBottomLeftCornerLocation(const FVector& InLocation) { GridBottomLeftCornerLocation = InLocation; }
	
	FORCEINLINE APlacer* GetActivePlacer() const { return ActivePlacer; }
	FORCEINLINE void SetActivePlacer(APlacer* InActivatePlacer) { ActivePlacer = InActivatePlacer; }

	FORCEINLINE FName GetObjectForPlacement() const { return ObjectForPlacement; }
	FORCEINLINE void SetObjectForPlacement(FName InObjectForPlacement) { ObjectForPlacement = InObjectForPlacement; }
	
	FORCEINLINE bool GetbObjectForPlacementIsSelected() const { return bObjectForPlacementIsSelected; }
	FORCEINLINE void SetbObjectForPlacementIsSelected(bool InbObjectForPlacementIsSelected) { bObjectForPlacementIsSelected = InbObjectForPlacementIsSelected; }

	FORCEINLINE FVector GetLocationUnderCursorCamera() const { return LocationUnderCursorCamera; }
	FORCEINLINE void SetLocationUnderCursorCamera(FVector InLocationUnderCursorCamera) { LocationUnderCursorCamera = InLocationUnderCursorCamera; }
	
	FORCEINLINE FIntPoint GetCellUnderCursor() const { return CellUnderCursor; }
	FORCEINLINE void SetCellUnderCursor(FIntPoint InCellUnderCursor) { CellUnderCursor = InCellUnderCursor; }

	/*Enabled Getter Setter*/
	FORCEINLINE bool GetbBuildObjecEnabled() const { return bBuildObjecEnabled; }
	FORCEINLINE void SetbBuildObjecEnabled(bool InbBuildObjecEnabled) { bBuildObjecEnabled = InbBuildObjecEnabled; }

	FORCEINLINE bool GetbBuildToolEnabled() const { return bBuildToolEnabled; }
	FORCEINLINE void SetbBuildToolEnabled(const bool& InBuildToolEnabled) { bBuildToolEnabled = InBuildToolEnabled; }
	
	FORCEINLINE bool GetbPlacerIndicatorEnabled() const { return bPlacerIndicatorEnabled; }
	FORCEINLINE void SetbPlacerIndicatorEnabled(bool InbPlacerIndicatorEnabled) { bPlacerIndicatorEnabled =InbPlacerIndicatorEnabled; }
	
	FORCEINLINE bool GetbDemolitionToolEnabled() const { return bDemolitionToolEnabled; }
	FORCEINLINE void SetbDemolitionToolEnabled(const bool& InDemolitionToolEnabled) { bDemolitionToolEnabled = InDemolitionToolEnabled; }

	FORCEINLINE bool GetbPlaceableObjectSelected() const { return bPlaceableObjectSelected; }
	FORCEINLINE void SetbPlaceableObjectSelected(const bool& InPlaceableObjectSelected) { bPlaceableObjectSelected = InPlaceableObjectSelected; }
private:
	FObjectData* PlaceableObjectData;
	
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
	float StartTraceHeight = 3000.0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Grid", meta = (AllowPrivateAccess = "true"))
	float EndTraceHeight = -3000.0;
	
	
	/* Object Variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object", meta = (AllowPrivateAccess = "true"))
	APlacer* ActivePlacer;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object", meta = (AllowPrivateAccess = "true"))
	FName ObjectForPlacement;

	/* Enabled Variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enabled", meta = (AllowPrivateAccess = "true"))
	bool bBuildToolEnabled = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enabled", meta = (AllowPrivateAccess = "true"))
	bool bDemolitionToolEnabled = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enabled", meta = (AllowPrivateAccess = "true"))
	bool bPlacerIndicatorEnabled = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enabled", meta = (AllowPrivateAccess = "true"))
	bool bBuildObjecEnabled = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enabled", meta = (AllowPrivateAccess = "true"))
	bool bObjectForPlacementIsSelected = false;

	/* Under Cursor Variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnderCursor", meta = (AllowPrivateAccess = "true"))
	FVector LocationUnderCursorCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnderCursor", meta = (AllowPrivateAccess = "true"))
	FIntPoint CellUnderCursor;

	/* Interact Variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact", meta = (AllowPrivateAccess = "true"))
	bool bPlaceableObjectSelected = false;

	
	/* Local Function Library */
	FVector SnapVectorToVector(FVector V1, FVector V2);
	
	float SnapFlaotToFloat(float CurrentLocation, float GridSize);
	
	FVector2D GetCellCenterToLocation(FIntPoint InCell);
	

	/* Getter & Setter */
	// Grid System Variables
	FORCEINLINE float GetStartTraceHeight() const { return StartTraceHeight; }
	FORCEINLINE void SetStartTraceHeight(float InStartTraceHeight) { StartTraceHeight = InStartTraceHeight; }

	FORCEINLINE float GetEndTraceHeight() const { return EndTraceHeight; }
	FORCEINLINE void SetInEndTraceHeight(float InEndTraceHeight) { EndTraceHeight = InEndTraceHeight; }
	
	FORCEINLINE FObjectData* GetPlaceableObjectData() const { return PlaceableObjectData; }
	FORCEINLINE void SetPlaceableObjectData(FObjectData* InPlaceableObjectData) { PlaceableObjectData = InPlaceableObjectData; }

	
};


