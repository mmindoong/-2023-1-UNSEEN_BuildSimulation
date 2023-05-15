// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/BSGameMode.h"

ABSGameMode::ABSGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> CharacterClassRef(TEXT("/Script/BuildSimulation.BSPawn"));
	if(CharacterClassRef.Class)
	{
		DefaultPawnClass = CharacterClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/BuildSimulation.BSPlayerController"));
	if(PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
}
