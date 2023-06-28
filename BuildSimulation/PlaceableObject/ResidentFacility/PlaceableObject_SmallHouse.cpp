// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaceableObject/ResidentFacility/PlaceableObject_SmallHouse.h"

APlaceableObject_SmallHouse::APlaceableObject_SmallHouse() : APlaceableObjectBase()
{
	static ConstructorHelpers::FClassFinder<AActor> BP_UNITDATA(TEXT("/Game/Blueprints/Core/Unit/BP_Unit"));
	
	if(BP_UNITDATA.Succeeded())
	{
		BlueprintActorClass = BP_UNITDATA.Class;
		UE_LOG(LogTemp, Log, TEXT("[BP_Unit] BP_unit Asset Load"));
	}

	if(BlueprintActorClass)
	{
		UWorld* World = GetWorld(); // ���� World �ν��Ͻ��� �����ɴϴ�.

		if (World)
		{
			FVector SpawnLocation(0.f, 0.f, 0.f); // ���� ��ġ�� �����մϴ�.
			FRotator SpawnRotation(0.f, 0.f, 0.f); // ���� ȸ���� �����մϴ�.

			AActor* SpawnedActor = World->SpawnActor<AActor>(BlueprintActorClass, SpawnLocation, SpawnRotation); // Blueprint Actor�� �����մϴ�.

			if (SpawnedActor)
			{
				// Blueprint Actor�� ���������� �����Ǿ��� ���� �߰� �۾��� �����մϴ�.
			}
		}
	}
	
}

void APlaceableObject_SmallHouse::SetObjectSelectedMode(bool IsSelected)
{
	Super::SetObjectSelectedMode(IsSelected);

	// Selected ����϶��� Ȱ��ȭ
	if(IsSelected )
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, FString::Printf(TEXT("Child Function")));
		
		
	}
		
}


