// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPlacer_Base_Class.h"

// Sets default values
AObjectPlacer_Base_Class::AObjectPlacer_Base_Class()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AObjectPlacer_Base_Class::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AObjectPlacer_Base_Class::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

