#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlaceableObject//PlaceableObjectBase.h"
#include "Placer/PlacerObjectBase.h"
#include "Data/PlaceableObjectsData.h"
#include "GridSystemComponent.h"
#include "ResourceActorComponent.h"
#include "Containers/Map.h"
#include "BuildManager.generated.h"

/* Delegate Declare */
DECLARE_DELEGATE_OneParam(FUpdateResourceAmount, FConstructionCost NewResourceAmout);
DECLARE_DELEGATE(FBuildNotEnoughResource);

UCLASS()
class BUILDSIMULATION_API ABuildManager : public AActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	ABuildManager();

	/* Delegate Instance */
	FUpdateResourceAmount OnResourceChanged;
	FBuildNotEnoughResource BuildNotEnoughResourceEvent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UGridSystemComponent* GridSystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UResourceActorComponent* ResourceActorComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	TArray<FName> BuildingsAvailableForConstruction;

	
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
	void DetectMouseDrag();

	UFUNCTION(BlueprintCallable, Category = "Main")
	void RotateObject(bool bLeft);

	/* Build Functions */
	UFUNCTION(BlueprintCallable, Category = "Build")
	void SpawnTileMap(FVector CenterLocation, FVector TileSize, FIntPoint TileCount);

	UFUNCTION(BlueprintCallable, Category = "Build")
	void SetupNatural();

	UFUNCTION(BlueprintCallable, Category = "Build")
	void BuildPlaceableObject();

	UFUNCTION(BlueprintCallable, Category = "Build")
	void PlaceEditorObject(APlaceableObjectBase* PlaceableObject, FName RowName);

	UFUNCTION(BlueprintCallable, Category = "Build")
	void DrawPlacementIndicators();
	
	UFUNCTION(BlueprintCallable, Category="Tool")
	void DestorySelectedPlaceableObject();

	
	/* Data Functions */
	UFUNCTION(BlueprintCallable, Category = "Data|Occupancy")
	bool CheckOccupancyData(FIntPoint Cell);
	
	UFUNCTION(BlueprintCallable, Category = "Data|Occupancy")
	void ChangeOccupancyData(FIntPoint Cell, bool IsOccupied);

	UFUNCTION(BlueprintCallable, Category = "Data|Object")
	void SearchHappinessFacility_Resident(APlaceableObjectBase* PlaceableObject);

	UFUNCTION(BlueprintCallable, Category = "Data|TileMap")
	void SetupTileMapData(FIntPoint Cell, int32 InSoil);

	UFUNCTION(BlueprintCallable, Category = "Data|TileMap")
	bool CheckTileMapData(FIntPoint Cell);

	UFUNCTION(BlueprintCallable, Category = "Data|TileMap")
	bool CheckResourceMapData(int32 ActivePlacerResourceTypeResourceType, FIntPoint Cell);
	
	UFUNCTION(BlueprintCallable, Category = "Data|Object")
	void SetupObjectData(FIntPoint Cell, APlaceableObjectBase* PlaceableObject);
	
	
	/* Resource Functions */
	UFUNCTION(BlueprintCallable, Category = "Resource")
	void UpdateResourcesValue(FConstructionCost Resource, bool Add, bool Subtract);
	
	FORCEINLINE FConstructionCost GetPlayerResources() const {return PlayerResources;}
	FORCEINLINE void SetPlayerResources(const FConstructionCost InPlayerResources) { PlayerResources = InPlayerResources; };//OnResourceChanged.Execute(GetPlayerResources()); };

protected:
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* InstancedStaticMeshComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(VisibleAnywhere)
	UStaticMesh* IndicatorMesh;


private:
	
	/* Settings Variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	FLinearColor PlayerOutlineColor = FLinearColor::Green;
	
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

	/* Data Variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	TMap<FIntPoint, int32> OccupancyData; // True : 1, False : 0

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	TMap<FIntPoint, int32> TileMapData; // Green : 0, Yellow : 1, Red : 2
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	TMap<FIntPoint, int32> ResourceTypeData; // Wood : 0, Rock : 1, Iron : 2, Coal : 3
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	TMap<FIntPoint, APlaceableObjectBase*> ObjectData;
	
	TMultiMap<FIntPoint, int32> HappinessTypeData; // 1:Road, 2:Well, 3:Market, 4:Church, 5:Bank
	
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


	void SetOutlineColor(int32 ObjectSide);
	// Resource Check
	bool CheckifEnoughResources(FConstructionCost InCost);

	
	/* Getter & Setter */

	FORCEINLINE APlaceableObjectBase* GetPlaceableObjectUnderCursor() const { return PlaceableObjectUnderCursor; }
	FORCEINLINE void SetPlaceableObjectUnderCursor(APlaceableObjectBase* InPlaceableObjectUnderCursor) { PlaceableObjectUnderCursor = InPlaceableObjectUnderCursor; }

	FORCEINLINE APlaceableObjectBase* GetSelectedPlaceableObject() const { return SelectedPlaceableObject; }
	FORCEINLINE void SetSelectedPlaceableObject(APlaceableObjectBase* InSelectedPlaceableObject) { SelectedPlaceableObject = InSelectedPlaceableObject; }
	
	FORCEINLINE TMap<FIntPoint, int32> GetOccupancyData() const { return OccupancyData; }
	FORCEINLINE void SetOccupancyData(const TMap<FIntPoint, int32> InOccupancyData) { OccupancyData = InOccupancyData; }

	FORCEINLINE TMap<FIntPoint, int32> GetTileMapData() const { return TileMapData; }
	FORCEINLINE void SetTileMapData(const TMap<FIntPoint, int32> InTileMapData) { TileMapData = InTileMapData; }

	FORCEINLINE TMap<FIntPoint, APlaceableObjectBase*> GetObjectData() const { return ObjectData; }
	FORCEINLINE void SetObjectData(const TMap<FIntPoint, APlaceableObjectBase*> InObjectData) { ObjectData = InObjectData; }

	FORCEINLINE APlayerController* GetPlayerController() const {return PlayerController;}
	FORCEINLINE void SetPlayerController(APlayerController* InPlayerController) { PlayerController = InPlayerController; }

	FORCEINLINE APlaceableObjectBase* GetPlaceableObjectBase() const {return PlaceableObjectBase;}
	FORCEINLINE void SetPlaceableObjectBase(APlaceableObjectBase* InPlaceableObjectBase) { PlaceableObjectBase = InPlaceableObjectBase; }

	FORCEINLINE UMaterialParameterCollectionInstance* GetPCI() const { return PCI; }
	FORCEINLINE void SetPCI(UMaterialParameterCollectionInstance* InPCI) { PCI = InPCI; }

	FORCEINLINE bool GetbTraceComplex() const { return bTraceComplex; }
	FORCEINLINE void SetbTraceComplex(bool InbTraceComplex) { bTraceComplex = InbTraceComplex; }

	FORCEINLINE float GetStartDragInstance() const { return StartDragInstance; }
	FORCEINLINE void SetStartDragInstance(float InStartDragInstance) { StartDragInstance= InStartDragInstance; }

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



