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
		UWorld* World = GetWorld(); // 현재 World 인스턴스를 가져옵니다.

		if (World)
		{
			FVector SpawnLocation(0.f, 0.f, 0.f); // 스폰 위치를 설정합니다.
			FRotator SpawnRotation(0.f, 0.f, 0.f); // 스폰 회전을 설정합니다.

			AActor* SpawnedActor = World->SpawnActor<AActor>(BlueprintActorClass, SpawnLocation, SpawnRotation); // Blueprint Actor를 스폰합니다.

			if (SpawnedActor)
			{
				// Blueprint Actor가 성공적으로 스폰되었을 때의 추가 작업을 수행합니다.
			}
		}
	}
	
}

void APlaceableObject_SmallHouse::SetObjectSelectedMode(bool IsSelected)
{
	Super::SetObjectSelectedMode(IsSelected);

	// Selected 모드일때만 활성화
	if(IsSelected )
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, FString::Printf(TEXT("Child Function")));
		
		
	}
		
}


