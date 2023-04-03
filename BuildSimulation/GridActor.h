#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
	
	// Getter
	FORCEINLINE const FVector& GetGridBottomLeftCornerLocation() const { return GridBottomLeftCornerLocation; }
	FORCEINLINE const FVector& GetGridCenterLocation() const { return GridCenterLocation; }

	// Setter
	FORCEINLINE void SetGridBottomLeftCornerLocation(const FVector& InLocation);
	FORCEINLINE void SetGridCenterLocation(const FVector& InLocation);
	

protected:
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* InstancedStaticMeshComponent;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnGrid", meta = (AllowPrivateAccess = "true"))
	FVector GridCenterLocation = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnGrid", meta = (AllowPrivateAccess = "true"))
	FVector GridTileSize = FVector(200.0f, 200.0f, 100.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnGrid", meta = (AllowPrivateAccess = "true"))
	FVector2D GridTileCount = FVector2D(3.0f, 3.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnGrid", meta = (AllowPrivateAccess = "true"))
	FVector GridBottomLeftCornerLocation = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnGrid", meta = (AllowPrivateAccess = "true"))
	bool IsTileMap = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnGrid", meta = (AllowPrivateAccess = "true"))
	float GridOffsetFromGround = -2.0f;

	UFUNCTION()
	FVector SnapVectorToVector(FVector V1, FVector V2);

	UFUNCTION()
	float SnapFlaotToFloat(float CurrentLocation, float GridSize);

	UFUNCTION()
	void CalculateCenterandBottomLeft();

	UFUNCTION()
	bool TraceforGround(FVector& Location);


};
