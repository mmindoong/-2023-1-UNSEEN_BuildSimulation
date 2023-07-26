// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NaturalObject.generated.h"

UCLASS()
class BUILDSIMULATION_API ANaturalObject : public AActor
{
	GENERATED_BODY()
	
public:	
	ANaturalObject();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	int32 ResourceType = -1;

	FORCEINLINE int32 GetResourceType() const { return ResourceType; }
	FORCEINLINE void SetResourceType(int32 InResourceType) { ResourceType = InResourceType; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> ResourceMeshComponent;

};
