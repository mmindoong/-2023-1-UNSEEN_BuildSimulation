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
	void SpawnGrid(FVector CenterLocation, FVector TileSize, FVector2D TileCount, bool UseEnvironment=false);

	UFUNCTION(BlueprintCallable, Category = "GridSystem")
	void SetGridOffsetFromGround(float Offset);

	UFUNCTION(BlueprintCallable, Category = "Main")
	void PressedLMB();


	// Getter & Setter
	FORCEINLINE const FVector& GetGridBottomLeftCornerLocation() const { return GridBottomLeftCornerLocation; }
	FORCEINLINE void SetGridBottomLeftCornerLocation(const FVector& InLocation) { GridBottomLeftCornerLocation = InLocation; }

	FORCEINLINE const FVector GetGridCenterLocation() const { return GridCenterLocation; }
	FORCEINLINE void SetGridCenterLocation(const FVector& InLocation) { GridCenterLocation = InLocation; }

	FORCEINLINE const bool& GetInteractStarted() const { return InteractStarted; }
	FORCEINLINE void SetInteractStarted(const bool& InInteractStarted) { InteractStarted = InInteractStarted; }

	FORCEINLINE const bool& GetBuildToolEnabled() const { return BuildToolEnabled; }
	FORCEINLINE void SetBuildToolEnabled(const bool& InBuildToolEnabled) { BuildToolEnabled = InBuildToolEnabled; }

	FORCEINLINE APlaceableObejct_Base_Class* GetPlaceableObjectUnderCursor() const { return PlaceableObjectUnderCursor; }
	FORCEINLINE void SetPlaceableObjectUnderCursor(APlaceableObejct_Base_Class* InPlaceableObjectUnderCursor) { PlaceableObjectUnderCursor = InPlaceableObjectUnderCursor; }

	FORCEINLINE APlaceableObejct_Base_Class* GetSelectedPlaceableObject() const { return SelectedPlaceableObject; }
	FORCEINLINE void SetSelectedPlaceableObject(APlaceableObejct_Base_Class* InSelectedPlaceableObject) { SelectedPlaceableObject = InSelectedPlaceableObject; }

	FORCEINLINE const bool& GetPlaceableObjectSelected() const { return PlaceableObjectSelected; }
	FORCEINLINE void SetPlaceableObjectSelected(const bool& InPlaceableObjectSelected) { PlaceableObjectSelected = InPlaceableObjectSelected; }


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

	FVector SnapVectorToVector(FVector V1, FVector V2);
	float SnapFlaotToFloat(float CurrentLocation, float GridSize);
	void CalculateCenterandBottomLeft();
	bool TraceforGround(FVector& Location);
	
	void SelectPlaceableObject();


};
