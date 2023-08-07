// Fill out your copyright notice in the Description page of Project Settings.

#include "PlaceableObjectBase.h"

#include <queue>

#include "Data/ObjectData.h"
#include "Game/BSGameSingleton.h"


/*M+M+++M+++Mf+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   APlaceableObjectBase
  
  @Summary:  Constructor
  
  @Modifies: [PlaceableObjectTable, SphereVisual, ObjectNameInTable].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
APlaceableObjectBase::APlaceableObjectBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	ObjectMesh->SetupAttachment(RootComponent);
	ObjectMesh->SetCollisionProfileName("BlockAll"); // WorldStatic Obejct

	if(UGameplayStatics::GetActorOfClass(GetWorld(), AResourceManager::StaticClass()))
	{
		ResourceManager = Cast<AResourceManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AResourceManager::StaticClass()));	
	}
	
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   BeginPlay
  
  @Summary:  Called when the game starts
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void APlaceableObjectBase::BeginPlay()
{
	Super::BeginPlay();

	if (ObjectMesh != nullptr)
	{
		ObjectMesh->OnBeginCursorOver.AddDynamic(this, &APlaceableObjectBase::OnBeginCursorOver);
		ObjectMesh->OnEndCursorOver.AddDynamic(this, &APlaceableObjectBase::OnEndCursorOver);
		UE_LOG(LogTemp, Warning, TEXT("AddDynamic"));
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("NULL"));
	
	
	SetupOutline();
	
	
}


/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   Tick
  
  @Summary:  Called every frame
  
  @Args:     float DeltaTime
			 Delta Seconds between frames
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
// Called every frame
void APlaceableObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   EndPlay
  
  @Summary:  Called when the game ends
  
  @Args:     const EEndPlayReason::Type EndPlayReason
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void APlaceableObjectBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	UpdatePlaceableObjectCursorEvent.Unbind();
}


/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   OnBeginCursorOver
  
  @Summary:  Called when the mouse cursor is moved over this actor
             if mouse over events are enabled in the player controller
  
  @Args:     UPrimitiveComponent* TouchedComponent
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void APlaceableObjectBase::OnBeginCursorOver(UPrimitiveComponent* TouchedComponent)
{
	SwapObjectHighlighting(true);
	if(UpdatePlaceableObjectCursorEvent.IsBound())
	{
		UpdatePlaceableObjectCursorEvent.Execute(this, false);
	}
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   OnEndCursorOver
  
  @Summary:  Called when the mouse cursor is moved off this actor
			 if mouse over events are enabled in the player controller
  
  @Args:     UPrimitiveComponent* TouchedComponent
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void APlaceableObjectBase::OnEndCursorOver(UPrimitiveComponent* TouchedComponent)
{
	SwapObjectHighlighting(false);
	if(UpdatePlaceableObjectCursorEvent.IsBound())
	{
		UpdatePlaceableObjectCursorEvent.Execute(this, true);
	}
}


/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   SetupPlaceableObject

  @Category: Parent Functions
  
  @Summary:  Setup PlaceableObject called when object construct

  @Modifies: [ObjectData, ObjectSize, ObjectDirection,
              OccupiedCenterCell].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void APlaceableObjectBase::SetupPlaceableObject()
{
	switch (GetBuildDirection())
	{
	case 0:
		ObjectMesh->SetWorldRotation(FRotator(0.0f, 0.0f, 0.0f));
		break;
	case 1:
		ObjectMesh->SetWorldRotation(FRotator(0.0f, 90.0f, 0.0f));
		break;
	case 2:
		ObjectMesh->SetWorldRotation(FRotator(0.0f, 180.0f, 0.0f));
		break;
	case 3:
		ObjectMesh->SetWorldRotation(FRotator(0.0f, -90.0f, 0.0f));
		break;
	}
	
	FName LocalRowName = GetRowName();
	FObjectData* OutRow = UBSGameSingleton::Get().GetPlaceableObjectDataTable()->FindRow<FObjectData>(LocalRowName, "");

	if (OutRow != nullptr)
	{
		// Set OutRow -> MaxHP, HP, BorderEnbaled, OutlineEnabled, HPBarEnabled
		ObjectMesh->SetStaticMesh(OutRow->ProceedingMesh.Get());
		SetObjectData(OutRow);
		SetObjectSize(GetObjectData()->ObjectSize);
		SetIsConstructionFacility(GetObjectData()->IsProductionFacility);
		SetHappinessFacilityType(GetObjectData()->HappinessFacilityType);
		SetIsResidentFacility(GetObjectData()->IsResidentFacility);
		// Set Object -> Object Direction, Object Size
		int32 b = ((FVector2D)(GetActorForwardVector())).Dot(FVector2D(0.0f, 1.0f)) >= 0.45 ? 1 : 3;
		int32 b2 = ((FVector2D)(GetActorForwardVector())).Dot(FVector2D(1.0f, 0.0f)) <= -0.45 ? 2 : b;
		int32 Direction = ((FVector2D)(GetActorForwardVector())).Dot(FVector2D(1.0f, 0.0f)) >= 0.45 ? 0 : b2;
		SetObjectDirection(Direction);
		FIntPoint ReturnValue = GetObjectDirection() == 0 || GetObjectDirection() == 2 ? FIntPoint(GetObjectSize().X, GetObjectSize().Y) : FIntPoint(GetObjectSize().Y, GetObjectSize().X);
		SetObjectSize(ReturnValue);
	}
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   SetupOutline

  @Category: Parent Functions
  
  @Summary:  Get Mesh Components and Setup to use Outline material
  
  @Modifies: [Meshesforoutline].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void APlaceableObjectBase::SetupOutline()
{
	TArray<UActorComponent*> MeshComponents = GetComponentsByClass(UStaticMeshComponent::StaticClass());
	if(MeshComponents.Num() > 0)
	{
		for (int32 i = 0; i < MeshComponents.Num(); i++)
			Meshesforoutline.Add(Cast<UStaticMeshComponent>(MeshComponents[i]));
		for (int32 j = 0; j < GetMeshesforoutline().Num(); j++)
			GetMeshesforoutline()[j]->SetRenderCustomDepth(true);
	}
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   SetObjectSeletecStat
  
  @Category: Parent Functions
  
  @Summary:  Called in BuildManager when Select Object to change selectd state
  
  @Args:     bool IsSelected
  
  @Modifies: [bObjectSelected]
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void APlaceableObjectBase::SetObjectSelectedMode(bool IsSelected) //???????
{
	SetbObjectSelected(IsSelected);
	if(GetbObjectSelected() == false)
	{
		SwapObjectHighlighting(false);
	}
	else if(GetbObjectSelected() == true)
	{
		SwapObjectHighlighting(true);
	}
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   SwapObjectHighlighting

  @Category: Parent Functions
  
  @Summary:  Swap Setting Outline, HP UI according to bool parameter
  
  @Args:     bool IsEnable
             check value when mouse cursor begin or over
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void APlaceableObjectBase::SwapObjectHighlighting(bool IsEnable)
{
	if(IsEnable)
	{
		LSwapObjectOutline(true);
	}
	else
	{
		LSwapObjectOutline(false);
	}
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   DestroyPlaceableObject
  
  @Category: Parent Functions
  
  @Summary:  Destory Actor
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void APlaceableObjectBase::DemolitionPlaceableObject()
{
	//todo : Set Occupancy Data in BuildManager before object is Destroy
	K2_DestroyActor();
}

bool APlaceableObjectBase::BuildCompleted()
{
	if(GetObjectData() != nullptr)
		ObjectMesh->SetStaticMesh(GetObjectData()->Mesh.Get());
	
	return true;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   FindClosestUnits
  
  @Category: Parent Function
  
  @Summary:  Find Units for Construction, Function etc
  
  @Args:     int32 NumActorsToFind, TArray<AActor*>& OutActors
  
  @Returns:  bool FindTrue
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M#*/
bool APlaceableObjectBase::FindClosestUnits(int32 NumActorsToFind, TArray<AActor*>& OutActors)
{
	TArray<AActor*> AllActors;
	TArray<AActor*> Units;
		
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), UBSGameSingleton::Get().GetUnitClass(), Units);
	
	for (AActor* Actor : Units)
	{
		if(!ResourceManager->Units.Contains(Actor) && IsValid(Actor))
		{
			AllActors.Add(Actor);
		}
	}
	if(NumActorsToFind > AllActors.Num()) //사용가능한 액터가 찾아야하는 액터보다 작을때
		return false;
	
	// 우선순위 큐를 사용하여 거리가 작은 순서대로 정렬된 n개의 액터를 유지
	std::priority_queue<FActorDistancePair> ClosestActorsQueue;

	if(AllActors.Num() > 0)
	{
		for (AActor* Actor : AllActors)
		{
			float Distance = FVector::Dist(Actor->GetActorLocation(), GetActorLocation());

			if (ClosestActorsQueue.size() < NumActorsToFind)
			{
				ClosestActorsQueue.emplace(Actor, Distance);
			}
			else if (Distance < ClosestActorsQueue.top().Distance)
			{
				ClosestActorsQueue.pop();
				ClosestActorsQueue.emplace(Actor, Distance);
			}
		}

		// 우선순위 큐에서 액터를 추출하여 결과 배열에 추가
		while (!ClosestActorsQueue.empty())
		{
			OutActors.Add(ClosestActorsQueue.top().Actor);
			ResourceManager->Units.Add(ClosestActorsQueue.top().Actor);
			ClosestActorsQueue.pop();
		}

		// 결과 배열을 거리가 가까운 순서로 정렬
		OutActors.Sort([this](const AActor& A, const AActor& B)
		{
			float DistanceA = FVector::Dist(A.GetActorLocation(), GetActorLocation());
			float DistanceB = FVector::Dist(B.GetActorLocation(), GetActorLocation());
			return DistanceA < DistanceB;
		});
	}
	return true;
}

// ResourceActors -> ResourceManager Actor 지정
bool APlaceableObjectBase::FindClosestInputs(int32 ResourceSetType, TSubclassOf<AActor> FindActor, int32 NumActorsToFind,
	TArray<AActor*>& OutActors)
{
	if(NumActorsToFind == 0)
		return false;
	TArray<AActor*> EnabledActors;
	TArray<AActor*> GetActors;

	TSet<AActor*> ResourceActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), FindActor, GetActors);
	
	for (AActor* Actor : GetActors)
	{
		switch (ResourceSetType)
		{
		case 0: 
			if(!ResourceManager->Units.Contains(Actor) && IsValid(Actor))
				EnabledActors.Add(Actor);
			break;
		case 1:
			if(!ResourceManager->Woods.Contains(Actor) && IsValid(Actor))
				EnabledActors.Add(Actor);
			break;
		case 2:
			if(!ResourceManager->Rocks.Contains(Actor)&& IsValid(Actor))
				EnabledActors.Add(Actor);
			break;
		case 3:
			if(!ResourceManager->Irons.Contains(Actor) && IsValid(Actor))
				EnabledActors.Add(Actor);
			break;
		case 4:
			if(!ResourceManager->Coals.Contains(Actor) && IsValid(Actor))
				EnabledActors.Add(Actor);
			break;
		case 5:
			if(!ResourceManager->Rices.Contains(Actor) && IsValid(Actor))
				EnabledActors.Add(Actor);
			break;
		case 6:
			if(!ResourceManager->Fruits.Contains(Actor) && IsValid(Actor))
				EnabledActors.Add(Actor);
			break;
		case 7:
			if(!ResourceManager->Meats.Contains(Actor) && IsValid(Actor))
				EnabledActors.Add(Actor);
			break;
		}
	}
	if(NumActorsToFind > EnabledActors.Num()) //사용가능한 액터가 찾아야하는 액터보다 작을때
		return false;
	
	// 우선순위 큐를 사용하여 거리가 작은 순서대로 정렬된 n개의 액터를 유지
	std::priority_queue<FActorDistancePair> ClosestActorsQueue;

	if(EnabledActors.Num() > 0)
	{
		for (AActor* Actor : EnabledActors)
		{
			float Distance = FVector::Dist(Actor->GetActorLocation(), GetActorLocation());

			if (ClosestActorsQueue.size() < NumActorsToFind)
			{
				ClosestActorsQueue.emplace(Actor, Distance);
			}
			else if (Distance < ClosestActorsQueue.top().Distance)
			{
				ClosestActorsQueue.pop();
				ClosestActorsQueue.emplace(Actor, Distance);
			}
		}

		// 우선순위 큐에서 액터를 추출하여 결과 배열에 추가
		while (!ClosestActorsQueue.empty())
		{
			OutActors.Add(ClosestActorsQueue.top().Actor);
			switch (ResourceSetType)
			{
			case 0:
				ResourceManager->Units.Add(ClosestActorsQueue.top().Actor);
				break;
			case 1:
				ResourceManager->Woods.Add(ClosestActorsQueue.top().Actor);
				break;
			case 2:
				ResourceManager->Rocks.Add(ClosestActorsQueue.top().Actor);
				break;
			case 3:
				ResourceManager->Irons.Add(ClosestActorsQueue.top().Actor);
				break;
			case 4:
				ResourceManager->Coals.Add(ClosestActorsQueue.top().Actor);
				break;
			case 5:
				ResourceManager->Rices.Add(ClosestActorsQueue.top().Actor);
				break;
			case 6:
				ResourceManager->Fruits.Add(ClosestActorsQueue.top().Actor);
				break;
			case 7:
				ResourceManager->Meats.Add(ClosestActorsQueue.top().Actor);
				break;
			}
			ClosestActorsQueue.pop();
		}

		// 결과 배열을 거리가 가까운 순서로 정렬
		OutActors.Sort([this](const AActor& A, const AActor& B)
		{
			float DistanceA = FVector::Dist(A.GetActorLocation(), GetActorLocation());
			float DistanceB = FVector::Dist(B.GetActorLocation(), GetActorLocation());
			return DistanceA < DistanceB;
		});
	}
	return true;
}

bool APlaceableObjectBase::FindClosestMaterialOutputs(int32 NumActorsToFind, TArray<AActor*>& OutActors)
{
	TArray<AActor*> AllActors;
	TArray<AActor*> Irons;
	TArray<AActor*> Rocks;
	TArray<AActor*> Coals;
	TArray<AActor*> Woods;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), UBSGameSingleton::Get().GetIronClass(), Irons);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), UBSGameSingleton::Get().GetRockClass(), Rocks);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), UBSGameSingleton::Get().GetCoalClass(), Coals);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), UBSGameSingleton::Get().GetWoodClass(), Woods);
	AActor* Castle= UGameplayStatics::GetActorOfClass(GetWorld(), UBSGameSingleton::Get().GetStartCastleClass());

	TArray<AActor*> OtherStorages;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), UBSGameSingleton::Get().GetMaterialStorageClass(), OtherStorages);
	// 다른 food storage에서 보관하고 있는지 확인
	for(AActor* iron : Irons)
	{
		bool IsNototherStorage = true;
		if(!ResourceManager->Irons.Contains(iron)  && !IsOverlappingActor(iron) && !Castle->IsOverlappingActor(iron))
		{
			for(AActor* otherStorage : OtherStorages)
			{
				if(otherStorage->IsOverlappingActor(iron))
				{
					IsNototherStorage = false;
				}
			}
			if(IsNototherStorage)
			{
				AllActors.Add(iron);
				//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString::Printf(TEXT("%s"), *(wood->GetName())));
			}
		}
	}
	for(AActor* rock : Rocks)
	{
		bool IsNototherStorage = true;
		if(!ResourceManager->Rocks.Contains(rock)  && !IsOverlappingActor(rock) && !Castle->IsOverlappingActor(rock))
		{
			for(AActor* otherStorage : OtherStorages)
			{
				if(otherStorage->IsOverlappingActor(rock))
				{
					IsNototherStorage = false;
				}
			}
			if(IsNototherStorage)
			{
				AllActors.Add(rock);
				//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString::Printf(TEXT("%s"), *(wood->GetName())));
			}
		}
	}
	for(AActor* coal : Coals)
	{
		bool IsNototherStorage = true;
		if(!ResourceManager->Coals.Contains(coal)  && !IsOverlappingActor(coal) && !Castle->IsOverlappingActor(coal))
		{
			for(AActor* otherStorage : OtherStorages)
			{
				if(otherStorage->IsOverlappingActor(coal))
				{
					IsNototherStorage = false;
				}
			}
			if(IsNototherStorage)
			{
				AllActors.Add(coal);
				//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString::Printf(TEXT("%s"), *(wood->GetName())));
			}
		}
	}
	for(AActor* wood : Woods)
	{
		bool IsNototherStorage = true;
		if(!ResourceManager->Woods.Contains(wood)  && !IsOverlappingActor(wood) && !Castle->IsOverlappingActor(wood))
		{
			for(AActor* otherStorage : OtherStorages)
			{
				if(otherStorage->IsOverlappingActor(wood))
				{
					IsNototherStorage = false;
				}
			}
			if(IsNototherStorage)
			{
				AllActors.Add(wood);
				//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString::Printf(TEXT("%s"), *(wood->GetName())));
			}
		}
	}
	
	if(NumActorsToFind > AllActors.Num())
		return false;
	
	// 우선순위 큐를 사용하여 거리가 작은 순서대로 정렬된 n개의 액터를 유지
	std::priority_queue<FActorDistancePair> ClosestActorsQueue;

	
	if(AllActors.Num() > 0)
	{
		for (AActor* Actor : AllActors)
		{
			if(UKismetSystemLibrary::GetDisplayName(Actor).Contains(TEXT("Iron")) && IsOverlappingActor(Actor) && ResourceManager->Irons.Contains(Actor))
				continue;
			else if(UKismetSystemLibrary::GetDisplayName(Actor).Contains(TEXT("Rock"))&& IsOverlappingActor(Actor) && ResourceManager->Rocks.Contains(Actor))
				continue;
			else if(UKismetSystemLibrary::GetDisplayName(Actor).Contains(TEXT("Coal")) && IsOverlappingActor(Actor) && ResourceManager->Coals.Contains(Actor))
				continue;
			else if(UKismetSystemLibrary::GetDisplayName(Actor).Contains(TEXT("Wood")) && IsOverlappingActor(Actor) && ResourceManager->Woods.Contains(Actor))
				continue;
		
			float Distance = FVector::Dist(Actor->GetActorLocation(), GetActorLocation());

			if (ClosestActorsQueue.size() < NumActorsToFind)
			{
				ClosestActorsQueue.emplace(Actor, Distance);
			}
			else if (Distance < ClosestActorsQueue.top().Distance)
			{
				ClosestActorsQueue.pop();
				ClosestActorsQueue.emplace(Actor, Distance);
			}
		}

		// 우선순위 큐에서 액터를 추출하여 결과 배열에 추가
		while (!ClosestActorsQueue.empty())
		{
			OutActors.Add(ClosestActorsQueue.top().Actor);
			if(UKismetSystemLibrary::GetDisplayName(ClosestActorsQueue.top().Actor).Contains(TEXT("Iron")))
				ResourceManager->Irons.Add(ClosestActorsQueue.top().Actor);
			else if(UKismetSystemLibrary::GetDisplayName(ClosestActorsQueue.top().Actor).Contains(TEXT("Rock")))
				ResourceManager->Rocks.Add(ClosestActorsQueue.top().Actor);
			else if(UKismetSystemLibrary::GetDisplayName(ClosestActorsQueue.top().Actor).Contains(TEXT("Coal")))
				ResourceManager->Coals.Add(ClosestActorsQueue.top().Actor);
			else if(UKismetSystemLibrary::GetDisplayName(ClosestActorsQueue.top().Actor).Contains(TEXT("Wood")))
				ResourceManager->Woods.Add(ClosestActorsQueue.top().Actor);
			
			ClosestActorsQueue.pop();
		}

		// 결과 배열을 거리가 가까운 순서로 정렬
		OutActors.Sort([this](const AActor& A, const AActor& B)
		{
			float DistanceA = FVector::Dist(A.GetActorLocation(), GetActorLocation());
			float DistanceB = FVector::Dist(B.GetActorLocation(), GetActorLocation());
			return DistanceA < DistanceB;
		});
	}
	return true;
}

bool APlaceableObjectBase::FindClosestFoodOutpus(int32 NumActorsToFind, TArray<AActor*>& OutActors)
{
	TArray<AActor*> AllActors;
	TArray<AActor*> Rices;
	TArray<AActor*> Fruits;
	TArray<AActor*> Meats;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), UBSGameSingleton::Get().GetRiceClass(), Rices);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), UBSGameSingleton::Get().GetFruitClass(), Fruits);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), UBSGameSingleton::Get().GetMeatClass(), Meats);
	AActor* Castle= UGameplayStatics::GetActorOfClass(GetWorld(), UBSGameSingleton::Get().GetStartCastleClass());

	TArray<AActor*> OtherStorages;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), UBSGameSingleton::Get().GetFoodStorageClass(), OtherStorages);
	
	// 다른 food storage에서 보관하고 있는지 확인
	for(AActor* rice : Rices)
	{
		bool IsNototherStorage = true;
		if(!ResourceManager->Rices.Contains(rice)  && !IsOverlappingActor(rice) && !Castle->IsOverlappingActor(rice))
		{
			for(AActor* otherStorage : OtherStorages)
			{
				if(otherStorage->IsOverlappingActor(rice))
				{
					IsNototherStorage = false;
				}
			}
			if(IsNototherStorage)
			{
				AllActors.Add(rice);
			}
		}
	}
	for(AActor* fruit : Fruits)
	{
		bool IsNototherStorage = true;
		if(!ResourceManager->Fruits.Contains(fruit)  && !IsOverlappingActor(fruit) && !Castle->IsOverlappingActor(fruit))
		{
			for(AActor* otherStorage : OtherStorages)
			{
				if(otherStorage->IsOverlappingActor(fruit))
				{
					IsNototherStorage = false;
				}
			}
			if(IsNototherStorage)
			{
				AllActors.Add(fruit);
			}
		}
	}
	for(AActor* meat : Meats)
	{
		bool IsNototherStorage = true;
		if(!ResourceManager->Meats.Contains(meat)  && !IsOverlappingActor(meat) && !Castle->IsOverlappingActor(meat))
		{
			for(AActor* otherStorage : OtherStorages)
			{
				if(otherStorage->IsOverlappingActor(meat))
				{
					IsNototherStorage = false;
				}
			}
			if(IsNototherStorage)
			{
				AllActors.Add(meat);
			}
		}
	}
	
	if(NumActorsToFind > AllActors.Num())
		return false;
	
	// 우선순위 큐를 사용하여 거리가 작은 순서대로 정렬된 n개의 액터를 유지
	std::priority_queue<FActorDistancePair> ClosestActorsQueue;
	
	if(AllActors.Num() > 0)
	{
		for (AActor* Actor : AllActors)
		{
			if(UKismetSystemLibrary::GetDisplayName(Actor).Contains(TEXT("Rice")) && IsOverlappingActor(Actor) && ResourceManager->Rices.Contains(Actor))
				continue;
			else if(UKismetSystemLibrary::GetDisplayName(Actor).Contains(TEXT("Fruit"))&& IsOverlappingActor(Actor) && ResourceManager->Fruits.Contains(Actor))
				continue;
			else if(UKismetSystemLibrary::GetDisplayName(Actor).Contains(TEXT("Meat")) && IsOverlappingActor(Actor) && ResourceManager->Meats.Contains(Actor))
				continue;
			
			float Distance = FVector::Dist(Actor->GetActorLocation(), GetActorLocation());

			if (ClosestActorsQueue.size() < NumActorsToFind)
			{
				ClosestActorsQueue.emplace(Actor, Distance);
			}
			else if (Distance < ClosestActorsQueue.top().Distance)
			{
				ClosestActorsQueue.pop();
				ClosestActorsQueue.emplace(Actor, Distance);
			}
		}

		// 우선순위 큐에서 액터를 추출하여 결과 배열에 추가
		while (!ClosestActorsQueue.empty())
		{
			OutActors.Add(ClosestActorsQueue.top().Actor);
			if(UKismetSystemLibrary::GetDisplayName(ClosestActorsQueue.top().Actor).Contains(TEXT("Rice")))
				ResourceManager->Rices.Add(ClosestActorsQueue.top().Actor);
			else if(UKismetSystemLibrary::GetDisplayName(ClosestActorsQueue.top().Actor).Contains(TEXT("Fruit")))
				ResourceManager->Fruits.Add(ClosestActorsQueue.top().Actor);
			else if(UKismetSystemLibrary::GetDisplayName(ClosestActorsQueue.top().Actor).Contains(TEXT("Rice")))
				ResourceManager->Meats.Add(ClosestActorsQueue.top().Actor);
			
			ClosestActorsQueue.pop();
		}

		// 결과 배열을 거리가 가까운 순서로 정렬
		OutActors.Sort([this](const AActor& A, const AActor& B)
		{
			float DistanceA = FVector::Dist(A.GetActorLocation(), GetActorLocation());
			float DistanceB = FVector::Dist(B.GetActorLocation(), GetActorLocation());
			return DistanceA < DistanceB;
		});
	}

	return true;
}

void APlaceableObjectBase::RemoveResourceActor(AActor* RemoveActor)
{
	if(UKismetSystemLibrary::GetDisplayName(RemoveActor).Contains(TEXT("Unit")) && ResourceManager->Units.Contains(RemoveActor))
	{
		ResourceManager->Units.Remove(RemoveActor);
	}
	else if(UKismetSystemLibrary::GetDisplayName(RemoveActor).Contains(TEXT("Wood")) && ResourceManager->Woods.Contains(RemoveActor))
	{
		ResourceManager->Woods.Remove(RemoveActor);
	}
	else if(UKismetSystemLibrary::GetDisplayName(RemoveActor).Contains(TEXT("Rock")) && ResourceManager->Rocks.Contains(RemoveActor))
	{
		ResourceManager->Rocks.Remove(RemoveActor);
	}
	else if(UKismetSystemLibrary::GetDisplayName(RemoveActor).Contains(TEXT("Iron")) && ResourceManager->Irons.Contains(RemoveActor))
	{
		ResourceManager->Irons.Remove(RemoveActor);
	}
	else if(UKismetSystemLibrary::GetDisplayName(RemoveActor).Contains(TEXT("Coal")) && ResourceManager->Coals.Contains(RemoveActor))
	{
		ResourceManager->Coals.Remove(RemoveActor);
	}
	else if(UKismetSystemLibrary::GetDisplayName(RemoveActor).Contains(TEXT("Rice")) && ResourceManager->Rices.Contains(RemoveActor))
	{
		ResourceManager->Rices.Remove(RemoveActor);
	}
	else if(UKismetSystemLibrary::GetDisplayName(RemoveActor).Contains(TEXT("Fruit")) && ResourceManager->Fruits.Contains(RemoveActor))
	{
		ResourceManager->Fruits.Remove(RemoveActor);
	}
	else if(UKismetSystemLibrary::GetDisplayName(RemoveActor).Contains(TEXT("Meat")) && ResourceManager->Meats.Contains(RemoveActor))
	{
		ResourceManager->Meats.Remove(RemoveActor);
	}
	
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   LSwapObjectOutline
  
  @Summary:  Local Function for SetDepthStencilValue for Outline according to bool parameter
  
  @Args:     bool IsEnable
             check value when mouse cursor begin or over
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void APlaceableObjectBase::LSwapObjectOutline(bool IsEnable)
{
	for (int32 i= 0; i < GetMeshesforoutline().Num(); i++)
	{
		if (IsEnable)
		{
				GetMeshesforoutline()[i]->SetCustomDepthStencilValue(1);
		}
		else
		{
			GetMeshesforoutline()[i]->SetCustomDepthStencilValue(0);
		}
			
	}
}
