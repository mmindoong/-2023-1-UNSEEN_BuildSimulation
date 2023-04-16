#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlaceableObjectBase.h"
#include "PlaceableObjectsData.h"
#include "BuildManager.generated.h"

UCLASS()
class BUILDSIMULATION_API ABuildManager : public AActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	ABuildManager();
	
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void BuildPlaceableObject();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/* Delegate Event Dispatchers  */
	UFUNCTION()
	void CallUpdatePlaceableObjectUnderCursor(APlaceableObjectBase* PlaceableObjectBase, bool IsRemove);

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
	
	UFUNCTION(BlueprintCallable, Category = "Main")
	void UpdateResouresValue(FConstructionCost Resource, bool Add, bool Subtract);

	UFUNCTION(BlueprintCallable, Category = "GridSystem")
	void SetGridOffsetFromGround(float Offset);

	UFUNCTION(BlueprintCallable, Category = "Main")
	void PressedLMB();
	

private:
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
		
	/* Interact Variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact", meta = (AllowPrivateAccess = "true"))
	bool bInteractStarted = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact", meta = (AllowPrivateAccess = "true"))
	bool bPlaceableObjectSelected = false;

	/* Enabled Variables */
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
	APlaceableObjectBase* PlaceableObjectBase;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object", meta = (AllowPrivateAccess = "true"))
	APlaceableObjectBase* PlaceableObjectUnderCursor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object", meta = (AllowPrivateAccess = "true"))
	APlaceableObjectBase* SelectedPlaceableObject;
	
	/* Local Function Library */
	FVector LSnapVectorToVector(FVector V1, FVector V2);
	
	float LSnapFlaotToFloat(float CurrentLocation, float GridSize);
	
	void LCalculateCenterandBottomLeft();
	
	void SelectPlaceableObject();

	void LSetOutlineColor(int32 ObjectSide);	
	

	/* Getter & Setter */
	FORCEINLINE FVector GetGridBottomLeftCornerLocation() const { return GridBottomLeftCornerLocation; }
	FORCEINLINE void SetGridBottomLeftCornerLocation(const FVector& InLocation) { GridBottomLeftCornerLocation = InLocation; }

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
	
};
