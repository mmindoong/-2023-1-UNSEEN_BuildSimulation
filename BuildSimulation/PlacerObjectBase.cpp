// Fill out your copyright notice in the Description page of Project Settings.


#include "PlacerObjectBase.h"

// Sets default values
APlacerObjectBase::APlacerObjectBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlacerObjectBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlacerObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

