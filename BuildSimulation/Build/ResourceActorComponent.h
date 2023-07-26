// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ResourceActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDSIMULATION_API UResourceActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UResourceActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actors", meta = (AllowPrivateAccess = "true"))
	TSet<AActor*> Units; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Construction", meta = (AllowPrivateAccess = "true"))
	TSet<AActor*>  Rocks;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Construction", meta = (AllowPrivateAccess = "true"))
	TSet<AActor*>  Woods;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Construction", meta = (AllowPrivateAccess = "true"))
	TSet<AActor*>  Irons;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actors", meta = (AllowPrivateAccess = "true"))
	TSet<AActor*> Rices;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actors", meta = (AllowPrivateAccess = "true"))
	TSet<AActor*>  Coals;
		
};
