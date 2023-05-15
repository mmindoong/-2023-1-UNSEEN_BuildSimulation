// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BSCharacterControlData.generated.h"

/**
 * 
 */
UCLASS()
class BUILDSIMULATION_API UBSCharacterControlData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, Category = Pawn)
	uint32 bUseContollerRotationYaw : 1;
	UPROPERTY(EditAnywhere, Category = Pawn)
	uint32 bUseContollerRotationPitch: 1;
	UPROPERTY(EditAnywhere, Category = Pawn)
	uint32 bUseContollerRotationRoll : 1;
	
	UPROPERTY(EditAnywhere, Category = PlayerController)
	uint32 bShowMouseCursor : 1;

	UPROPERTY(EditAnywhere, Category = PlayerController)
	uint32 EnableClickEvents : 1;

	UPROPERTY(EditAnywhere, Category = PlayerController)
	uint32 EnableMouseOverEvents : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	float TargetArmLength;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	FRotator RelativeRotation;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bUsePawnControlRotation : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bInheritPitch : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bInheritYaw : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bInheritRoll : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bDoCollisionTest : 1;
};
