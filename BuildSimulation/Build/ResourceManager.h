// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResourceManager.generated.h"

UCLASS()
class BUILDSIMULATION_API AResourceManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AResourceManager();

protected:

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actors", meta = (AllowPrivateAccess = "true"))
	TSet<AActor*> Units; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials", meta = (AllowPrivateAccess = "true"))
	TSet<AActor*>  Rocks;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials", meta = (AllowPrivateAccess = "true"))
	TSet<AActor*>  Woods;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials", meta = (AllowPrivateAccess = "true"))
	TSet<AActor*>  Irons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials", meta = (AllowPrivateAccess = "true"))
	TSet<AActor*>  Coals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Foods", meta = (AllowPrivateAccess = "true"))
	TSet<AActor*> Rices;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Foods", meta = (AllowPrivateAccess = "true"))
	TSet<AActor*> Meats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Foods", meta = (AllowPrivateAccess = "true"))
	TSet<AActor*> Fruits;

};
