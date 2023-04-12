#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlaceableObejct_Base_Class.h"

#include "GridActor.generated.h"

UCLASS()
class BUILDSIMULATION_API AGridActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGridActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "GridSystem")
	void SpawnGrid(FVector CenterLocation, FVector TileSize, FIntPoint TileCount, bool UseEnvironment=false);

	UFUNCTION(BlueprintCallable, Category = "GridSystem")
	void SetGridOffsetFromGround(float Offset);

	UFUNCTION(BlueprintCallable, Category = "Main")
	void PressedLMB();

	UFUNCTION(BlueprintCallable, Category = "Cell")
	TArray<FIntPoint> GetCellsinRectangularArea(FVector CenterLocation, FIntPoint TileCount);

	UFUNCTION(BlueprintCallable, Category = "Data|Occupancy")
	void SetOccupancyData(FIntPoint Cell, bool IsOccupied);

	UFUNCTION(BlueprintCallable, Category = "Data|Object")
	void SetObjectData(FIntPoint Cell, APlaceableObejct_Base_Class* PlaceableObject);

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
	APlaceableObejct_Base_Class* PlaceableObjectUnderCursor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact", meta = (AllowPrivateAccess = "true"))
	APlaceableObejct_Base_Class* SelectedPlaceableObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact", meta = (AllowPrivateAccess = "true"))
	bool PlaceableObjectSelected = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enabled", meta = (AllowPrivateAccess = "true"))
	bool BuildToolEnabled = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enabled", meta = (AllowPrivateAccess = "true"))
	bool DemolitionToolEnabled = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	TMap<FIntPoint, int32> OccupancyData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	TMap<FIntPoint, APlaceableObejct_Base_Class*> ObjectData;


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

	FORCEINLINE APlaceableObejct_Base_Class* GetPlaceableObjectUnderCursor() const { return PlaceableObjectUnderCursor; }
	FORCEINLINE void SetPlaceableObjectUnderCursor(APlaceableObejct_Base_Class* InPlaceableObjectUnderCursor) { PlaceableObjectUnderCursor = InPlaceableObjectUnderCursor; }

	FORCEINLINE APlaceableObejct_Base_Class* GetSelectedPlaceableObject() const { return SelectedPlaceableObject; }
	FORCEINLINE void SetSelectedPlaceableObject(APlaceableObejct_Base_Class* InSelectedPlaceableObject) { SelectedPlaceableObject = InSelectedPlaceableObject; }

	FORCEINLINE bool GetPlaceableObjectSelected() const { return PlaceableObjectSelected; }
	FORCEINLINE void SetPlaceableObjectSelected(const bool& InPlaceableObjectSelected) { PlaceableObjectSelected = InPlaceableObjectSelected; }

	FORCEINLINE TMap<FIntPoint, int32> GetOccupancyData() const { return OccupancyData; }
	FORCEINLINE void SetOccupancyData(const TMap<FIntPoint, int32> InOccupancyData) { OccupancyData = InOccupancyData; }

	FORCEINLINE TMap<FIntPoint, APlaceableObejct_Base_Class*> GetObjectData() const { return ObjectData; }
	FORCEINLINE void SetObjectData(const TMap<FIntPoint, APlaceableObejct_Base_Class*> InObjectData) { ObjectData = InObjectData; }

};
