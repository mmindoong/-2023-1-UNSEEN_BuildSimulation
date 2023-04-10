// Fill out your copyright notice in the Description page of Project Settings.

#include "GridActor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Math/UnrealMath.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AGridActor::AGridActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InstancedStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedStaticMesh"));
	RootComponent = InstancedStaticMeshComponent;

	SetGridCenterLocation(GetActorLocation());
	SpawnGrid(GridCenterLocation, GridTileSize, GridTileCount);

}


// Called when the game starts or when spawned
void AGridActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGridActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AGridActor::SetGridOffsetFromGround(float Offset)
{
	GridOffsetFromGround = Offset;
	InstancedStaticMeshComponent->SetWorldLocation(FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + GridOffsetFromGround));
}


void AGridActor::SpawnGrid(FVector CenterLocation, FVector TileSize, FVector2D TileCount, bool UseEnvironment)
{
	// Save the variables for later. We'll need them as long as the grid is alive.
	SetGridCenterLocation(CenterLocation);
	GridTileSize = TileSize;
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

FVector AGridActor::SnapVectorToVector(FVector CurrentPosition, FVector SnapValue)
{

	FVector Return;
	Return.X = FMath::RoundToInt32(CurrentPosition.X / SnapValue.X)* SnapValue.X;
	Return.Y = FMath::RoundToInt32(CurrentPosition.Y / SnapValue.Y) * SnapValue.Y;
	Return.Z = FMath::RoundToInt32(CurrentPosition.Z / SnapValue.Z) * SnapValue.Z;


	return Return;
}

float AGridActor::SnapFlaotToFloat(float CurrentLocation, float GridSize)
{
	float Return = FMath::RoundToInt32(CurrentLocation / GridSize) * GridSize;

	return Return;

}


void AGridActor::CalculateCenterandBottomLeft()
{
	FVector NewVector = SnapVectorToVector(GridCenterLocation, GridTileSize);
	FIntPoint AlignVector;

	AlignVector.X = static_cast<float>(FMath::Fmod(GridTileCount.X, 2)) == 0.0f ? 0.0f : 1.0f;
	AlignVector.Y = static_cast<float>(FMath::Fmod(GridTileCount.Y, 2)) == 0.0f ? 0.0f : 1.0f;

	FVector2D Divided = (GridTileCount - AlignVector) / 2.0f;
	FVector GridCount = FVector(Divided.X * GridTileSize.X, Divided.Y * GridTileSize.Y, 1.0f);
	
	SetGridCenterLocation(NewVector);
	SetGridBottomLeftCornerLocation(NewVector - GridCount);
	
}

bool AGridActor::TraceforGround(FVector& Location)
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


void AGridActor::PressedLMB()
{
	SetInteractStarted(true);
	SelectPlaceableObject();

}

void AGridActor::SelectPlaceableObject()
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
					//TODO: Set Object Seletectd State
				}
			}
			SetSelectedPlaceableObject(PlaceableObjectUnderCursor);
			SetPlaceableObjectSelected(true);
			//TODO: Set Object Seletectd State
		}
		// 2. Placeable Object 아래에 커서가 존재하지 않는 경우
		else
		{
			// Placeable Object가 존재하는지?
			SetPlaceableObjectSelected(false);
			if (IsValid(GetSelectedPlaceableObject()))
			{
				//TODO: Set Object Seletectd State
			}
		}
	}
}

TArray<FIntPoint> AGridActor::GetCellsinRectangularArea(FIntPoint CenterLocation, FIntPoint TileCount)
{
	// 설정한 CenterLocation, Tilecount로 Bottom Left Corner 계산
	SetGridCenterLocation(FVector(CenterLocation.X, CenterLocation.Y, GetGridCenterLocation().Z));
	SetGridTileCount(TileCount);
	CalculateCenterandBottomLeft();

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
	return Cells;
}

void AGridActor::SetOccupancyData(FIntPoint Cell, bool IsOccupied)
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

void AGridActor::SetObjectData(FIntPoint Cell, APlaceableObejct_Base_Class* PlaceableObject)
{
	// If the Data already exists, it will be overwritten.
	GetObjectData().Add(Cell, PlaceableObject);
}






