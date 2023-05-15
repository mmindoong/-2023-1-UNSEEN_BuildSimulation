// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/BSPawn.h"

// Sets default values
ABSPawn::ABSPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABSPawn::BeginPlay()
{
	Super::BeginPlay();

	
	
}

// Called every frame
void ABSPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABSPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

