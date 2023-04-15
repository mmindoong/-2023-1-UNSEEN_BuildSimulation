// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildManager.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ABuildManager::ABuildManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InstancedStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedStaticMesh"));
	RootComponent = InstancedStaticMeshComponent;

	SetGridCenterLocation(GetActorLocation());
	SpawnGrid(GridCenterLocation, GridTileSize, GridTileCount);

}

void ABuildManager::BuildPlaceableObject()
{
	FVector SpawnLocation = GetActorLocation(); //todo : Spawn 위치를 Cell Under Cursor로 변경
	FRotator rotator;
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	GetWorld()->SpawnActor<AActor>(PlaceableObjectBaseClass, SpawnLocation, rotator, SpawnParams );
	SetPlaceableObjectBase(Cast<APlaceableObjectBase>(UGameplayStatics::GetActorOfClass(GetWorld(),ABuildManager::StaticClass())));
	
	// Build Manger에서 점유된 셀 정보 세팅해주기
	if(GetPlaceableObjectBase()->GetObjectDynamicData()->HasData)
	{
		TArray<FIntPoint> GetOccupiedCells = GetCellsinRectangularArea(FVector(GetPlaceableObjectBase()->GetOccupiedCenterCell().X,GetPlaceableObjectBase()->GetOccupiedCenterCell().Y,100.0f), GetPlaceableObjectBase()->GetObjectSize());
		for (FIntPoint cells : GetOccupiedCells)
		{
			SetOccupancyData(cells, true);
			SetObjectData(cells, GetPlaceableObjectBase());
		}
	}
	else // Editor 상에서 진행한 경우
	{
		TArray<FIntPoint> GetOccupiedCells = GetCellsinRectangularArea(GetPlaceableObjectBase()->GetActorLocation(), GetPlaceableObjectBase()->GetObjectSize());
		for (FIntPoint cells : GetOccupiedCells)
		{
			SetOccupancyData(cells, true);
			SetObjectData(cells, GetPlaceableObjectBase());
		}
	}
}


// Called when the game starts or when spawned
void ABuildManager::BeginPlay()
{
	Super::BeginPlay();
	SetPlayerController(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	
	
}

void ABuildManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	// Delegate Delete
	UpdateResourceAmountEvent.Unbind();
}

// Called every frame
void ABuildManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ABuildManager::SetGridOffsetFromGround(float Offset)
{
	GridOffsetFromGround = Offset;
	InstancedStaticMeshComponent->SetWorldLocation(FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + GridOffsetFromGround));
}


void ABuildManager::SpawnGrid(FVector CenterLocation, FVector TileSize, FIntPoint TileCount, bool UseEnvironment)
{
	// Save the variables for later. We'll need them as long as the grid is alive.
	SetGridCenterLocation(CenterLocation);
	SetGridTileSize(TileSize);
	SetGridTileCount(TileCount);

	// Even/Odd to Round
	GridTileCount.X = round(TileCount.X);
	GridTileCount.Y = round(TileCount.Y);

	InstancedStaticMeshComponent->ClearInstances();

	// Find the bottom left corner of our grid and Center grid, to start spawning tiles from there.
	CalculateCenterandBottomLeft();

	for (int Index = 0; Index <= round(GridTileCount.X) - 1; Index++)
	{
		int X = Index;

		for (int Index2 = 0; Index2 <= round(GridTileCount.Y) - 1; Index2++)
		{
			int Y = Index2;

			// Find the Tile transform based on it's index (X,Y) and the variables we defined before.
			FVector TileTransformLocation = GetGridBottomLeftCornerLocation() + FVector(0.0f, 0.0f, 2.0f) + GridTileSize * FVector((float)X, (float)Y, 0.0f);
			// Mesh Size
			FVector TileTransformScale = GridTileSize / FVector(100.0f, 100.0f, 100.0f);

			// If Use Environment
			if (UseEnvironment)
			{
				//Trace for Ground
				if(TraceforGround(TileTransformLocation))
				{
					FTransform TileTransform;
					TileTransform.SetLocation(TileTransformLocation);
					TileTransform.SetScale3D(TileTransformScale);
					// Add Instanced Static Mesh
					int32 InstanceIndex = InstancedStaticMeshComponent->AddInstance(TileTransform, true);
					if (IsTileMap == true)
					{
						bool isRColor = FMath::RandBool();
						float ReturnRValue = isRColor ? 1.0f : 0.0f;
						// SetCustomDataValue - R Parameter
						InstancedStaticMeshComponent->SetCustomDataValue(InstanceIndex, 0, ReturnRValue);
						// Get a random element from the array
						TArray<float> FloatArray = { 0.5f, 1.0f };
						float RandomElement = FloatArray[FMath::RandRange(0, FloatArray.Num() - 1)];
						float ReturnGValue = isRColor ? RandomElement : 1.0f;
						// SetCustomDataValue - G Parameter
						InstancedStaticMeshComponent->SetCustomDataValue(InstanceIndex, 1, ReturnGValue);
						// SetCustomDataValue - B Parameter
						InstancedStaticMeshComponent->SetCustomDataValue(InstanceIndex, 2, 0.0f);
					}
				}
				
			}
			else
			{
				FTransform TileTransform;
				TileTransform.SetLocation(TileTransformLocation);
				TileTransform.SetScale3D(TileTransformScale);
				// Add Instanced Static Mesh
				int32 InstanceIndex = InstancedStaticMeshComponent->AddInstance(TileTransform, true);
				if (IsTileMap == true)
				{
					bool isRColor = FMath::RandBool();
					float ReturnRValue = isRColor ? 1.0f : 0.0f;
					// SetCustomDataValue - R Parameter
					InstancedStaticMeshComponent->SetCustomDataValue(InstanceIndex, 0, ReturnRValue);
					// Get a random element from the array
					TArray<float> FloatArray = { 0.5f, 1.0f };
					float RandomElement = FloatArray[FMath::RandRange(0, FloatArray.Num() - 1)];
					float ReturnGValue = isRColor ? RandomElement : 1.0f;
					// SetCustomDataValue - G Parameter
					InstancedStaticMeshComponent->SetCustomDataValue(InstanceIndex, 1, ReturnGValue);
					// SetCustomDataValue - B Parameter
					InstancedStaticMeshComponent->SetCustomDataValue(InstanceIndex, 2, 0.0f);
				}
			}
			
		}
	}
}

FVector ABuildManager::SnapVectorToVector(FVector CurrentPosition, FVector SnapValue)
{

	FVector Return;
	Return.X = FMath::RoundToInt32(CurrentPosition.X / SnapValue.X)* SnapValue.X;
	Return.Y = FMath::RoundToInt32(CurrentPosition.Y / SnapValue.Y) * SnapValue.Y;
	Return.Z = FMath::RoundToInt32(CurrentPosition.Z / SnapValue.Z) * SnapValue.Z;


	return Return;
}

float ABuildManager::SnapFlaotToFloat(float CurrentLocation, float GridSize)
{
	float Return = FMath::RoundToInt32(CurrentLocation / GridSize) * GridSize;

	return Return;

}


void ABuildManager::CalculateCenterandBottomLeft()
{
	FVector NewVector = SnapVectorToVector(GridCenterLocation, GridTileSize);
	FIntPoint AlignVector;

	AlignVector.X = static_cast<float>(FMath::Fmod(GridTileCount.X, 2)) == 0.0f ? 0.0f : 1.0f;
	AlignVector.Y = static_cast<float>(FMath::Fmod(GridTileCount.Y, 2)) == 0.0f ? 0.0f : 1.0f;

	FIntPoint Divided = (GridTileCount - AlignVector) / 2;
	FVector GridCount = FVector(Divided.X * GridTileSize.X, Divided.Y * GridTileSize.Y, 1.0f);
	
	SetGridCenterLocation(NewVector);
	SetGridBottomLeftCornerLocation(NewVector - GridCount);
	
}

bool ABuildManager::TraceforGround(FVector& Location)
{
	FVector StartLocation = FVector(Location.X + 0.0f, Location.Y + 0.0f, Location.Z + 1000.0f);
	FVector EndLocation = FVector(Location.X + 0.0f, Location.Y + 0.0f, Location.Z - 1000.0f);
	float Radius = GridTileSize.X / 3.0f;
	FCollisionQueryParams Params(NAME_None, false);
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;

	bool IsHitResult = UKismetSystemLibrary::SphereTraceMulti(
		GetWorld(),
		StartLocation,
		EndLocation,
		Radius,
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel2),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResults,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		5.0f);


	if (HitResults.Num() > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("%s"), *HitResults[0].Location.ToString());
		Location.Z = SnapFlaotToFloat(HitResults[0].Location.Z, GridTileSize.Z) - 20.0f;
		return true;
	}
	return false;
}


void ABuildManager::PressedLMB()
{
	SetInteractStarted(true);
	SelectPlaceableObject();

}

void ABuildManager::SelectPlaceableObject()
{
	if (GetBuildToolEnabled() == false) 
	{
		// 1. Placeable Object 아래에 커서가 존재할 경우
		if(IsValid(GetPlaceableObjectUnderCursor()))
		{
			// 1-1. Placeable Object 객체가 기존에 존재했는지 체크->오브젝트 상태변경
			if (IsValid(GetSelectedPlaceableObject()))
			{
				if (GetPlaceableObjectUnderCursor() != GetSelectedPlaceableObject())
				{
					// todo: Set Object Seletectd State
					
				}
			}
			SetSelectedPlaceableObject(PlaceableObjectUnderCursor);
			SetPlaceableObjectSelected(true);
			//todo: Set Object Seletectd State
		}
		// 2. Placeable Object 아래에 커서가 존재하지 않는 경우
		else
		{
			// Placeable Object가 존재하는지?
			SetPlaceableObjectSelected(false);
			if (IsValid(GetSelectedPlaceableObject()))
			{
				//todo: Set Object Seletectd State
			}
		}
	}
}

TArray<FIntPoint> ABuildManager::GetCellsinRectangularArea(FVector CenterLocation, FIntPoint TileCount)
{
	// 설정한 CenterLocation, Tilecount로 Bottom Left Corner 계산
	SetGridCenterLocation(CenterLocation);
	SetGridTileCount(TileCount);

	int CurrentCellX = 0;
	int CurrentCellY = 0;
	TArray<FIntPoint> Cells;
	for (int Index = 0; Index <= round(GridTileCount.X) - 1; Index++)
	{
		CurrentCellX += Index;
		for (int Index2 = 0; Index2 <= round(GridTileCount.Y) - 1; Index2++)
		{
			CurrentCellY += Index2;
			Cells.Add(FIntPoint(CurrentCellX, CurrentCellY));
		}
	}
	SpawnGrid(CenterLocation, GetGridTileSize(), TileCount);
	return Cells;
}

void ABuildManager::SetOccupancyData(FIntPoint Cell, bool IsOccupied)
{	
	if (IsOccupied)
	{
		if (GetOccupancyData().Find(Cell))
		{
			GetOccupancyData().Add(Cell, *GetOccupancyData().Find(Cell));
		}
		else
		{
			GetOccupancyData().Add(Cell, 1);
		}
	}
	else
	{
		if (GetOccupancyData().Find(Cell))
		{
			if (*GetOccupancyData().Find(Cell) < 2)
			{
				GetOccupancyData().Remove(Cell);
			}
			else
			{
				GetOccupancyData().Add(Cell, *GetOccupancyData().Find(Cell) - 1);
			}
		}
	}
}

void ABuildManager::SetObjectData(FIntPoint Cell, APlaceableObjectBase* PlaceableObject)
{
	// If the Data already exists, it will be overwritten.
	GetObjectData().Add(Cell, PlaceableObject);
}

void ABuildManager::UpdateResouresValue(FConstructionCost Resource, bool Add, bool Subtract)
{
	if(Add)
	{
		FFoodData AddFood = FFoodData(GetPlayerResources().Food.Rice+Resource.Food.Rice,
			GetPlayerResources().Food.Fruit+ Resource.Food.Fruit,
			GetPlayerResources().Food.Meat+ Resource.Food.Meat);
		FConstructionCost AddConstruction = FConstructionCost(GetPlayerResources().Gold+Resource.Gold,
			AddFood,
			GetPlayerResources().Wood+Resource.Wood,
			GetPlayerResources().Rock+Resource.Rock,
			GetPlayerResources().Iron+Resource.Iron,
			GetPlayerResources().Coal+Resource.Coal);
		
		SetPlayerResources(AddConstruction);
	}
	else if(Subtract)
	{
		FFoodData SubFood = FFoodData(FMath::Clamp(GetPlayerResources().Food.Rice-Resource.Food.Rice,0,GetPlayerResources().Food.Rice-Resource.Food.Rice),
		FMath::Clamp(GetPlayerResources().Food.Fruit-Resource.Food.Fruit,0,GetPlayerResources().Food.Fruit-Resource.Food.Fruit),
		FMath::Clamp(GetPlayerResources().Food.Meat-Resource.Food.Meat,0,GetPlayerResources().Food.Meat-Resource.Food.Meat));

		FConstructionCost SubConstruction = FConstructionCost(FMath::Clamp(GetPlayerResources().Gold-Resource.Gold, 0 , GetPlayerResources().Gold-Resource.Gold),
			SubFood,
			FMath::Clamp(GetPlayerResources().Wood-Resource.Wood, 0 , GetPlayerResources().Wood-Resource.Wood),
			FMath::Clamp(GetPlayerResources().Rock-Resource.Rock, 0 , GetPlayerResources().Rock-Resource.Rock),
			FMath::Clamp(GetPlayerResources().Iron-Resource.Iron, 0 , GetPlayerResources().Iron-Resource.Iron),
			FMath::Clamp(GetPlayerResources().Coal-Resource.Coal, 0 , GetPlayerResources().Coal-Resource.Coal));

		SetPlayerResources(SubConstruction);
	}
	else
	{
		FFoodData Food = FFoodData(FMath::Clamp(Resource.Food.Rice,0,Resource.Food.Rice),
		FMath::Clamp(Resource.Food.Fruit,0,Resource.Food.Fruit),
		FMath::Clamp(Resource.Food.Meat,0,Resource.Food.Meat));
		
		FConstructionCost Construction = FConstructionCost(FMath::Clamp(Resource.Gold, 0, Resource.Gold),
			Food,
		FMath::Clamp(Resource.Wood, 0, Resource.Wood),
		FMath::Clamp(Resource.Rock, 0, Resource.Rock),
		FMath::Clamp(Resource.Iron, 0, Resource.Iron),
		FMath::Clamp(Resource.Coal, 0, Resource.Coal));

		SetPlayerResources(Construction);
	}
	// Call Event Delegate
	if(UpdateResourceAmountEvent.IsBound())
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Execute")));
		UpdateResourceAmountEvent.Execute(GetPlayerResources());
	}
	
}






