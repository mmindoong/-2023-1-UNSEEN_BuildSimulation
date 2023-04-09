// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "PlaceableObjectsData.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlaceableObejct_Base_Class.generated.h"

class AGridActor;

UCLASS()
class BUILDSIMULATION_API APlaceableObejct_Base_Class : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlaceableObejct_Base_Class();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "ParentFunctions")
	void SetupPlaceableObject();

	// Getter & Setter
	FORCEINLINE FPlaceableObjectData* GetObjectData() const { return ObjectData; }
	FORCEINLINE void SetObjectData(FPlaceableObjectData* InObjectData) { ObjectData = InObjectData; }

	FORCEINLINE FIntPoint GetObjectSize() const { return ObjectSize; }
	FORCEINLINE void SetObjectSize(FIntPoint InObjectSize) { ObjectSize = InObjectSize; }

	FORCEINLINE bool GetBorderEnabled() const { return BorderEnabled; }
	FORCEINLINE void SetBorderEnabled(bool InBorderEnabled) { BorderEnabled = InBorderEnabled; }

	FORCEINLINE bool GetOutlineEnabled() const { return OutlineEnabled; }
	FORCEINLINE void SetOutlineEnabled(bool InOutlineEnabled) { OutlineEnabled = InOutlineEnabled; }

	FORCEINLINE bool GetHPBarEnabled() const { return HPBarEnabled; }
	FORCEINLINE void SetHPBarEnabled(bool InHPBarEnabled) { HPBarEnabled = InHPBarEnabled; }

	FORCEINLINE float GetHP() const { return HP; }
	FORCEINLINE void SetHP(float InHP) { HP = InHP; }

	FORCEINLINE float GetMaxHP() const { return MaxHP; }
	FORCEINLINE void SetMaxHP(float InMaxHP) { HP = InMaxHP; }

	FORCEINLINE int32 GetObjectDirection() const { return ObjectDirection; }
	FORCEINLINE void SetObjectDirection(int InObjectDirection) { ObjectDirection = InObjectDirection; }

	FORCEINLINE FDataTableRowHandle GetObjectNameInTable() const { return ObjectNameInTable; }
	FORCEINLINE void SetObjectNameInTable(FDataTableRowHandle InObjectNameInTable) { ObjectNameInTable = InObjectNameInTable; }

	FORCEINLINE float GetStartingHealthPercent() const { return StartingHealthPercent; }
	FORCEINLINE void SetStartingHealthPercent(float InStartingHealthPercent) { StartingHealthPercent = InStartingHealthPercent; }

	FORCEINLINE AGridActor* GetBuildManager() const { return BuildManager; }
	FORCEINLINE void SetBuildManager(AGridActor* InBuildManager) { BuildManager = InBuildManager; }


private:
	FPlaceableObjectData* ObjectData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Data", meta = (AllowPrivateAccess = "true"))
	FIntPoint ObjectSize = FIntPoint(1,1);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Data", meta = (AllowPrivateAccess = "true"))
	bool BorderEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Data", meta = (AllowPrivateAccess = "true"))
	bool OutlineEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Data", meta = (AllowPrivateAccess = "true"))
	bool HPBarEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Data", meta = (AllowPrivateAccess = "true"))
	float HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Data", meta = (AllowPrivateAccess = "true"))
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Data", meta = (AllowPrivateAccess = "true"))
	int32 ObjectDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	class UDataTable* PlaceableObjectTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	FDataTableRowHandle ObjectNameInTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	float StartingHealthPercent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid System", meta = (AllowPrivateAccess = "true"))
	AGridActor* BuildManager;

};
