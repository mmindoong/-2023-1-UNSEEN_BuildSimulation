// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildManager.h"

#include "Background/NaturalObject.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Core/BSPlayerController.h"
#include "Game/BSGameSingleton.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "PlaceableObject/ResidentFacility/PlaceableObject_House.h"

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   ABuildManager
  
  @Summary:  Constructor
  
  @Modifies: [InstancedStaticMeshComponent, StaticMeshComponent,
             GridCenterLocation, Collection].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
// Sets default values
ABuildManager::ABuildManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InstancedStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedStaticMesh"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	GridSystemComponent = CreateDefaultSubobject<UGridSystemComponent>(TEXT("GridSystemComponent"));
	ResourceActorComponent = CreateDefaultSubobject<UResourceActorComponent>(TEXT("ResourceActorComponent"));
	RootComponent = InstancedStaticMeshComponent;
	
	// Outline Material Asset Load
	static ConstructorHelpers::FObjectFinder<UMaterialParameterCollection> MaterialCollection(TEXT("/Game/GridAssets/Materials/Outline/MPC_OutlineCollection"));
	if(MaterialCollection.Succeeded())
	{
		Collection = MaterialCollection.Object;
		UE_LOG(LogTemp, Log, TEXT("[BuildManager] MPC_OutlineCollection Asset Load"));
	}

	// Placer Indicator Mesh Asset Load
	static ConstructorHelpers::FObjectFinder<UStaticMesh> IndicatorMeshAsset(TEXT("/Game/GridAssets/Meshes/SM_Plane_100x100"));
	if (IndicatorMeshAsset.Succeeded())
	{
		UE_LOG(LogTemp, Log, TEXT("[BuildManager] IndicatorMesh Asset Load"));
		IndicatorMesh = IndicatorMeshAsset.Object;
	}
	

	
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   BeginPlay
  
  @Summary:  Called when the game starts or when spawned

  @Modifies: [PCI, PlayerController]
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void ABuildManager::BeginPlay()
{
	Super::BeginPlay();
	if(Collection)
	{
		if(GetWorld())
		{
			SetPCI(GetWorld()->GetParameterCollectionInstance(Collection));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString::Printf(TEXT("GetWorld is NULL")));
		}
	}
	SetPlayerController(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	UpdateResourcesValue(FConstructionCost(0, 10,10,10, 10, 20, 10, 10, 10,0), true, false);
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   Tick
  
  @Summary:  Called every frame
  
  @Args:     float DeltaTime
			 Delta Seconds between frames
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void ABuildManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(GridSystemComponent->GetbBuildToolEnabled() || GridSystemComponent->GetbDemolitionToolEnabled())
	{
		UpdateBuildingManagerValues();
		if(GridSystemComponent->GetbPlacerIndicatorEnabled() && GridSystemComponent->GetbPlaceableObjectSelected() == false )
		{
			DrawPlacementIndicators();
		}
		
	}

}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   EndPlay
  
  @Summary:  Called when the game ends
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void ABuildManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   CallUpdatePlaceableObjectUnderCursor

  @Category: Event Dispatcher
  
  @Summary:  Callback function when Object's Mouse Cursor event occured
  
  @Args:     APlaceableObjectBase* InPlaceableObjectBase, bool IsRemove
			 Get Object's pointer, bool whether begin or end
  
  @Modifies: [PlaceableObjectUnderCursor, PCI]
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void ABuildManager::CallUpdatePlaceableObjectUnderCursor(APlaceableObjectBase* InPlaceableObjectBase, bool IsRemove)
{
	if(IsRemove && GetPlaceableObjectUnderCursor() == InPlaceableObjectBase) // Cursor End Overlap ???? ??? ???
	{
		SetPlaceableObjectUnderCursor(nullptr);
	}
	else // Cursor Begin Overlap
	{
		SetPlaceableObjectUnderCursor(InPlaceableObjectBase);
		if(IsValid(GetPlaceableObjectUnderCursor()))
		{
			// Demolition?? ???????? ???
			if(GridSystemComponent->GetbDemolitionToolEnabled())
			{
				if(IsValid(GetPCI()))
					GetPCI()->SetScalarParameterValue(FName("EnableShading"), 1.0f);
			}
			else
			{
				SetOutlineColor();
				if(IsValid(GetPCI()))
					GetPCI()->SetScalarParameterValue(FName("EnableShading"), -0.5f);
			}
		}
	}
}



/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   PressedLMB
  
  @Category: Main
 
  @Summary:  Left ??lick to select objects under Cursor

  @Modifies: [bInteractStarted, bDragWasInterrupted,
             StartLocationUnderCursor]
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void ABuildManager::PressedLMB()
{
	SelectPlaceableObject();
	if(GridSystemComponent->GetbObjectForPlacementIsSelected())
	{
		FHitResult ResultCamera;
		if(GetPlayerController()->GetHitResultUnderCursorByChannel(TraceTypeQuery2, false, ResultCamera))
		{
			if(ResultCamera.bBlockingHit)
				SetStartLocationUnderCursor(ResultCamera.Location);
		}
	}
	
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   ReleasedLMB
  
  @Category: Main
 
  @Summary:  Build Object when released

  @Modifies: [bInteractStarted, bDragWasInterrupted,
			 bDragStarted]
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void ABuildManager::ReleasedLMB()
{
	if(GridSystemComponent->GetbBuildToolEnabled() && GridSystemComponent->GetbObjectForPlacementIsSelected())
	{
		if(IsValid(GridSystemComponent->GetActivePlacer()))
		{
			if(CheckifEnoughResources(GridSystemComponent->GetActivePlacer()->GetObjectData()->ConstructionCost))
			{
				BuildPlaceableObject();
			}
			else
			{
				ABSPlayerController* Player = Cast<ABSPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
				Player->K2_OnBuildEvent();
			}
		}
	}
	
}


/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   BuildPlaceableObject

  @Category: Main
  
  @Summary:  Spawn PlaceableObject Actor, Set new data

  @Modifies: [PlaceableObjectBase, OccupancyData, ObjectData]
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void ABuildManager::BuildPlaceableObject()
{
	FVector2D SpawnCenterLocation = GridSystemComponent->GetCenterOfRectangularArea(GetCellUnderCursor(), GridSystemComponent->GetActivePlacer()->GetObjectSize());
	FVector SpawnLocation = FVector(SpawnCenterLocation.X , SpawnCenterLocation.Y, GetLocationUnderCursorCamera().Z);
	FRotator SpawnRotator = FRotator(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner =this;
	
	if(GetWorld() && GridSystemComponent->GetbBuildObjecEnabled())
	{
		// DT?? ?????? ??? Object ??? ??????? ???
		SetPlaceableObjectBase(Cast<APlaceableObjectBase>(GetWorld()->SpawnActor<AActor>(GridSystemComponent->GetActivePlacer()->GetObjectData()->PlacaebleObjectClass, SpawnLocation, SpawnRotator, SpawnParams)));
		GetPlaceableObjectBase()->SetRowName(GridSystemComponent->GetActivePlacer()->GetRowName());
		GetPlaceableObjectBase()->SetBuildDirection(GridSystemComponent->GetActivePlacer()->GetBuildDirection());
		// Object Cell Location setting
		GetPlaceableObjectBase()->ObjectCellLocation = GridSystemComponent->GetActivePlacer()->ObjectCellLocation;
		// Object RowName, Data ???? ?? Setting ????
		GetPlaceableObjectBase()->SetupPlaceableObject();
		GetPlaceableObjectBase()->SetupOutline();
		UpdateResourcesValue(GetPlaceableObjectBase()->GetObjectData()->ConstructionCost, false, true);
	}
	
	if(IsValid(GetPlaceableObjectBase()))
	{
		// BuildPlaceableObject ????? ????? ??? true, ????????? ????? ??? false
		GetPlaceableObjectBase()->IsDynamicData = true;
		// Build Manger???? ?????? ?? ???? ?????????
		GetPlaceableObjectBase()->SetOccupiedCenterCell(FIntPoint(GetActorLocation().X, GetActorLocation().Y));
		TArray<FIntPoint> GetOccupiedCells = GridSystemComponent->GetCellsinRectangularArea(GridSystemComponent->GetCellfromWorldLocation(GetPlaceableObjectBase()->GetActorLocation()),
			GetPlaceableObjectBase()->GetObjectSize());
		for (FIntPoint cells : GetOccupiedCells)
		{
			ChangeOccupancyData(cells, true);
			SetupObjectData(cells, GetPlaceableObjectBase());
			GetPlaceableObjectBase()->OccupiedCells.Add(cells);
		}
		
		// Binding event when PlaceableObject construct
		GetPlaceableObjectBase()->UpdatePlaceableObjectCursorEvent.BindUFunction(this, FName("CallUpdatePlaceableObjectUnderCursor"));

		// ???? ????? ???? ??? ??? ????? object?? ????? ????? ??ыб ???? HappinessTypeData ???????
		if(GetPlaceableObjectBase()->HappinessFacilityType != 0)
		{
			FIntPoint Range;
			switch (GetPlaceableObjectBase()->HappinessFacilityType)
			{
			case 0 :
				break;
			case 1 : // road
				Range = FIntPoint(GetPlaceableObjectBase()->GetObjectData()->ObjectSize.X * 3, GetPlaceableObjectBase()->GetObjectData()->ObjectSize.Y * 3);
				break;
			case 2 : // well
				Range = FIntPoint(GetPlaceableObjectBase()->GetObjectData()->ObjectSize.X * 3, GetPlaceableObjectBase()->GetObjectData()->ObjectSize.Y * 3);
				break;
			case 3 : // market
				Range = FIntPoint(GetPlaceableObjectBase()->GetObjectData()->ObjectSize.X * 4, GetPlaceableObjectBase()->GetObjectData()->ObjectSize.Y * 4);
				break;
			case 4 : // church
				Range = FIntPoint(GetPlaceableObjectBase()->GetObjectData()->ObjectSize.X * 7, GetPlaceableObjectBase()->GetObjectData()->ObjectSize.Y * 7);
				break;
			case 5 : // bank
				Range = FIntPoint(GetPlaceableObjectBase()->GetObjectData()->ObjectSize.X * 5, GetPlaceableObjectBase()->GetObjectData()->ObjectSize.Y * 5);
				break;
			}
			TArray<FIntPoint> CellsforBuild = GridSystemComponent->GetCellsinRectangularArea(GridSystemComponent->GetCellfromWorldLocation(GetPlaceableObjectBase()->GetActorLocation()),
				Range); // ?? ??? ??ъ¤? cell ???
			
			for(auto cells : CellsforBuild)
			{
				// ???? ??? data ??ъ¤? ????
				HappinessTypeData.Add(cells, GetPlaceableObjectBase()->HappinessFacilityType);
			}
		}
	}
}

void ABuildManager::PlaceEditorObject(APlaceableObjectBase* PlaceableObject, FName RowName)
{
	if(GetWorld())
	{
		// ????? Object?? self?? ??????? ????????. 
		SetPlaceableObjectBase(PlaceableObject);
		GetPlaceableObjectBase()->SetRowName(RowName);
		GetPlaceableObjectBase()->SetBuildDirection(PlaceableObject->GetBuildDirection());
		// Object Cell Location setting
		GetPlaceableObjectBase()->ObjectCellLocation =PlaceableObject->ObjectCellLocation;
		// Object RowName, Data ???? ?? Setting ????
		GetPlaceableObjectBase()->SetupPlaceableObject();
		GetPlaceableObjectBase()->SetupOutline();
	}
	
	if(IsValid(GetPlaceableObjectBase()))
	{
		
		// Build Manger???? ?????? ?? ???? ?????????
		GetPlaceableObjectBase()->SetOccupiedCenterCell(FIntPoint(GetActorLocation().X, GetActorLocation().Y));
		TArray<FIntPoint> GetOccupiedCells = GridSystemComponent->GetCellsinRectangularArea(GridSystemComponent->GetCellfromWorldLocation(PlaceableObject->GetActorLocation()),
			GetPlaceableObjectBase()->GetObjectSize());
		for (FIntPoint cells : GetOccupiedCells)
		{
			ChangeOccupancyData(cells, true);
			SetupObjectData(cells, GetPlaceableObjectBase());
			GetPlaceableObjectBase()->OccupiedCells.Add(cells);
		}
		
		// Binding event when PlaceableObject construct
		GetPlaceableObjectBase()->UpdatePlaceableObjectCursorEvent.BindUFunction(this, FName("CallUpdatePlaceableObjectUnderCursor"));

		// ???? ????? ???? ??? ??? ????? object?? ????? ????? ??ыб ???? HappinessTypeData ???????
		if(PlaceableObject->HappinessFacilityType != 0)
		{
			FIntPoint Range;
			switch (PlaceableObject->HappinessFacilityType)
			{
			case 0 :
				break;
			case 1 : // road
				Range = FIntPoint(GetPlaceableObjectBase()->GetObjectData()->ObjectSize.X * 3, GetPlaceableObjectBase()->GetObjectData()->ObjectSize.Y * 3);
				break;
			case 2 : // well
				Range = FIntPoint(GetPlaceableObjectBase()->GetObjectData()->ObjectSize.X * 3, GetPlaceableObjectBase()->GetObjectData()->ObjectSize.Y * 3);
				break;
			case 3 : // market
				Range = FIntPoint(GetPlaceableObjectBase()->GetObjectData()->ObjectSize.X * 4, GetPlaceableObjectBase()->GetObjectData()->ObjectSize.Y * 4);
				break;
			case 4 : // church
				Range = FIntPoint(GetPlaceableObjectBase()->GetObjectData()->ObjectSize.X * 7, GetPlaceableObjectBase()->GetObjectData()->ObjectSize.Y * 7);
				break;
			case 5 : // bank
				Range = FIntPoint(GetPlaceableObjectBase()->GetObjectData()->ObjectSize.X * 5, GetPlaceableObjectBase()->GetObjectData()->ObjectSize.Y * 5);
				break;
			}
			TArray<FIntPoint> CellsforBuild = GridSystemComponent->GetCellsinRectangularArea(GridSystemComponent->GetCellfromWorldLocation(GetPlaceableObjectBase()->GetActorLocation()),
				Range); // ?? ??? ??ъ¤? cell ???
			
			for(auto cells : CellsforBuild)
			{
				// ???? ??? data ??ъ¤? ????
				HappinessTypeData.Add(cells, PlaceableObject->HappinessFacilityType);
			}
		}
	}
}

void ABuildManager::SearchHappinessFacility_Resident(APlaceableObjectBase* PlaceableObject)
{
	if(IsValid(PlaceableObject))
	{
		// ???? ????? cell?? key?? ???? ???? data?? ????? ??
		TArray<FIntPoint> CellsforBuild = GridSystemComponent->GetCellsinRectangularArea(GridSystemComponent->GetCellfromWorldLocation(PlaceableObject->GetActorLocation()),
				PlaceableObject->GetObjectSize()); // ?? ??? ??ъ¤? cell ???
		
		for(FIntPoint ObjectCell : CellsforBuild)
		{
			TArray<int32> HappinessType;

			if(HappinessTypeData.Num() > 0 && HappinessTypeData.Contains(ObjectCell))
			{
				HappinessTypeData.MultiFind(ObjectCell, HappinessType);
			}
			
			for(int32 types : HappinessType)
			{
				PlaceableObject->OwnerofHappiness.Add(types);
			}
		}
	}
	PlaceableObject->K2_OnUpdateHappinessData();
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   UpdateBuildingManagerValues
  
  @Category: Main
  
  @Summary:  This function updates the values under the cursor every tick

  @Modifies: [LocationUnderCursorVisibility, LocationUnderCursorCamera,
             LastCellUnderCursor, CellUnderCursor, bCellUnderCursorHasChanged]
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void ABuildManager::UpdateBuildingManagerValues()
{
	FHitResult ResultVisiblity;
	if(GetPlayerController()->GetHitResultUnderCursorByChannel(TraceTypeQuery1, false, ResultVisiblity))
	{
		if(ResultVisiblity.bBlockingHit)
		{
			SetLocationUnderCursorVisibility(ResultVisiblity.Location);
			SetActorUnderCursor(ResultVisiblity.GetActor());
		}
	}
	FHitResult ResultCamera;
	if(GetPlayerController()->GetHitResultUnderCursorByChannel(TraceTypeQuery2, false, ResultCamera))
	{
		if(ResultCamera.bBlockingHit)
		{
			SetLocationUnderCursorCamera(ResultCamera.Location);
			GridSystemComponent->SetLocationUnderCursorCamera(GetLocationUnderCursorCamera());
		}
	}
	
	if( GetCellUnderCursor() != GetLastCellUnderCursor())
	{
		SetLastCellUnderCursor(GetCellUnderCursor());
		SetbCellUnderCursorHasChanged(true);
	}
	else
		SetbCellUnderCursorHasChanged(false);

	
	FIntPoint Cell= GridSystemComponent->GetCellfromWorldLocation(GetLocationUnderCursorCamera());
	SetCellUnderCursor(Cell);

}



/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   SelectPlaceableObject

  @Category: Main
  
  @Summary:  Process UnderCursor object to Select object when PressedLMB
             & Object's SelectedMode Activate
  
  @Modifies: [SelectedPlaceableObject, bPlaceableObjectSelected]
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void ABuildManager::SelectPlaceableObject()
{
	if (GridSystemComponent->GetbBuildToolEnabled() == false) 
	{
		// 1. Placeable Object ????? ии???? ?????? ???
		if(IsValid(GetPlaceableObjectUnderCursor()))
		{
			// 1-1. Placeable Object ????? ?????? ?????????? ??->??????? ???им???
			if (IsValid(GetSelectedPlaceableObject()))
			{
				if (GetPlaceableObjectUnderCursor() != GetSelectedPlaceableObject())
				{
					GetSelectedPlaceableObject()->K2_OnHideDetailWidget();
					GetSelectedPlaceableObject()->SetObjectSelectedMode(false); // ?????? ????? Object?? ?????.
				}
			}
			SetSelectedPlaceableObject(PlaceableObjectUnderCursor); // ии?? ?????? ????????? ???? ????? ????????? ????
			GridSystemComponent->SetbPlaceableObjectSelected(true);
			GetSelectedPlaceableObject()->SetObjectSelectedMode(true);
			// ???? ?? UI ????
			GetSelectedPlaceableObject()->K2_OnDisplayDetailWidget();
			// ????? Outline, Material Color ????
			SetOutlineColor();
			if(IsValid(GetPCI()))
				GetPCI()->SetScalarParameterValue(FName("EnableShading"), 0.4f);
		}
		// 2. Placeable Object ????? ии???? ???????? ??? ???
		else
		{
			GridSystemComponent->SetbPlaceableObjectSelected(false); // ????? ???б╞? ??? ?????? ???
			if (IsValid(GetSelectedPlaceableObject())) 
			{
				GetSelectedPlaceableObject()->K2_OnHideDetailWidget();
				GetSelectedPlaceableObject()->SetObjectSelectedMode(false);
				SetSelectedPlaceableObject(nullptr);
			}
		}
	}
}

void ABuildManager::DeselectPlaceableObject()
{
	if(GridSystemComponent->GetbPlaceableObjectSelected() && IsValid(GetSelectedPlaceableObject()))
	{
		GetSelectedPlaceableObject()->SetObjectSelectedMode(false);
		GridSystemComponent->SetbPlaceableObjectSelected(false);
		if(GetPlaceableObjectUnderCursor() == GetSelectedPlaceableObject())
		{
			GetSelectedPlaceableObject()->SwapObjectHighlighting(true);
		}
		/*if(GetSelectedPlaceableObject()->GetIsConstructing())
			GetSelectedPlaceableObject()->K2_OnHideConstructionWidget();
		else*/
		GetSelectedPlaceableObject()->K2_OnHideDetailWidget();
		SetSelectedPlaceableObject(nullptr);
	}
}





/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   RotateObject
  
  @Category: Main
  
  @Summary:  do ActivePlacer's Rotation
  
  @Args:     bool bLeft
             if rotation is left
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void ABuildManager::RotateObject(bool bLeft)
{
	if(GridSystemComponent->GetbBuildToolEnabled())
		GridSystemComponent->GetActivePlacer()->Rotate(true);
}


/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   SpawnTileMap

  @Category: Main
  
  @Summary:  Spawn Tile Map with Instanced static mesh
  
  @Args:     FVector CenterLocation, FVector TileSize, FIntPoint TileCount
              Tile Map's Center Location, Size, Count

  @Modifies: [GridCenterLocation, GridTileSize, GridTileCount,
              InstancedStaticMeshComponent]
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void ABuildManager::SpawnTileMap(FVector CenterLocation, FVector TileSize, FIntPoint TileCount)
{
	// Save the variables for later. We'll need them as long as the grid is alive.
	GridSystemComponent->SetGridCenterLocation(CenterLocation);
	
	GridSystemComponent->SetGridTileSize(TileSize);	
	GridSystemComponent->SetGridTileCount(TileCount);

	// Even/Odd to Round
	GridSystemComponent->SetGridTileCount( FIntPoint(round(TileCount.X), round(TileCount.Y)));
	InstancedStaticMeshComponent->ClearInstances();

	// Find the bottom left corner of our grid and Center grid, to start spawning tiles from there.
	GridSystemComponent->CalculateCenterandBottomLeft();
	
	for (int X = 0; X < GridSystemComponent->GetGridTileCount().X - 1; X += 8)
	{
		for (int  Y = 0;  Y < GridSystemComponent->GetGridTileCount().Y - 1;  Y += 8)
		{
			// 0 : ???, 1: ???, 2: ????
			int32 ColorIncludedRed = FMath::RandRange(0, 2);
			int32 ColorExcludedRed = FMath::RandRange(0, 1);
			bool isRColor = FMath::RandBool();
			int32 BackGroundColor = isRColor ? ColorIncludedRed : ColorExcludedRed;
			int32 SelectedArr[8][8] = { 0 };

			// ?????? ???? ????
			int32 pattern1[8][8] = {
				{BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor, 1, 1, 1, 1},
				{BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor, 1, 1, 1, 1},
				{BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor, 1, 1, 1, 1},
				{BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor, 1, 1, 1, 1},
				{0, 0, 0, 0, BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor},
				{0, 0, 0, 0, BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor},
				{0, 0, 0, 0, BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor},
				{0, 0, 0, 0, BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor}
			};

			// ?????? ???? ????
			int32 pattern2[8][8] = {
				{BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor,},
				{BackGroundColor, 0, 0, 0, 0, 0, 0, BackGroundColor,},
				{BackGroundColor, 0, 0, 0, 0, 0, 0, BackGroundColor,},
				{BackGroundColor, 0, 0, 0, 0, 0, 0, BackGroundColor,},
				{BackGroundColor, 0, 0, 0, 0, 0, 0, BackGroundColor},
				{BackGroundColor, 0, 0, 0, 0, 0, 0, BackGroundColor},
				{BackGroundColor, 0, 0, 0, 0, 0, 0, BackGroundColor},
				{BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor}
			};

			int32 pattern3[8][8] = {
				{BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor,},
				{BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor,},
				{BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor,},
				{BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor,},
				{BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor},
				{BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor},
				{BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor},
				{BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor, BackGroundColor}
			};
			int32 RanArray = FMath::RandRange(1, 3);
			for(int x =0; x <8; x++)
			{
				for(int y = 0; y < 8; y++)
				{
					if(RanArray == 1)
						SelectedArr[x][y] = pattern1[x][y];
					else if(RanArray == 2)
						SelectedArr[x][y] = pattern2[x][y];
					else if(RanArray == 3)
						SelectedArr[x][y] = pattern3[x][y];
				}
			}
			int x = -1;
			for(int i = X; i <= X + 7; i++ )
			{
				x++;
				int y = -1;
				for(int j = Y; j <= Y + 7; j++)
				{
					y++;
					// Find the Tile transform based on it's index (X,Y) and the variables we defined before.
					FVector TileTransformLocation = GridSystemComponent->GetGridBottomLeftCornerLocation() + FVector(0.0f, 0.0f, 2.0f) + GridSystemComponent->GetGridTileSize() * FVector((float)i, (float)j, 0.0f);
					// Mesh Size
					FVector TileTransformScale = GridSystemComponent->GetGridTileSize() / FVector(100.0f, 100.0f, 100.0f);
					FTransform TileTransform;
					TileTransform.SetLocation(TileTransformLocation);
					TileTransform.SetScale3D(TileTransformScale);
					// Ground ??
					FHitResult HitResult;
					FVector StartLocation = TileTransformLocation + FVector(0.0f, 0.0f, 100.0f);
					FVector EndLocation =  TileTransformLocation +  FVector(0.0f, 0.0f, -500.0f);;
					TArray<AActor*> ActorsToIgnore;
					FCollisionQueryParams Params;
					/*bool IsHitResult = UKismetSystemLibrary::LineTraceSingle(
								GetWorld(),
								StartLocation,
								EndLocation,
								UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1),
								false,
								ActorsToIgnore,
								EDrawDebugTrace::None,
								HitResult,
								true,
								FLinearColor::Red,
								FLinearColor::Green,
								10.0f);
								*/

					
					TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; // ??? ?????? ??????? ??????.
					TEnumAsByte<EObjectTypeQuery> Ground_OBJ = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel3);
					ObjectTypes.Add(Ground_OBJ);
					bool IsHitResult = UKismetSystemLibrary::LineTraceSingleForObjects(
							GetWorld(),
						StartLocation,
						EndLocation,
						ObjectTypes,
						false,
						ActorsToIgnore,
						EDrawDebugTrace::None,
						HitResult,
						true,
						FLinearColor::Red,
						FLinearColor::Green,
						0.5f);
					
					if(IsHitResult == false)
					{
						continue;
					}
					
					// Add Instanced Static Mesh
					int32 InstanceIndex = InstancedStaticMeshComponent->AddInstance(TileTransform, true);
					
					if(SelectedArr[x][y] == 0)
					{
						// ???
						// SetCustomDataValue - R Parameter
						InstancedStaticMeshComponent->SetCustomDataValue(InstanceIndex, 0, 0.0f);
						// SetCustomDataValue - G Parameter
						InstancedStaticMeshComponent->SetCustomDataValue(InstanceIndex, 1, 1.0f);
						// SetCustomDataValue - B Parameter
						InstancedStaticMeshComponent->SetCustomDataValue(InstanceIndex, 2, 0.0f);
						FIntPoint GetTileMapCells = GridSystemComponent->GetCellfromWorldLocation(TileTransform.GetLocation());
						SetupTileMapData(GetTileMapCells, 0);
						
					}
					else if(SelectedArr[x][y] == 1)
					{
						
						// ?????
						// SetCustomDataValue - R Parameter
						InstancedStaticMeshComponent->SetCustomDataValue(InstanceIndex, 0, 1.0f);
						// SetCustomDataValue - G Parameter
						InstancedStaticMeshComponent->SetCustomDataValue(InstanceIndex, 1, 1.0f);
						// SetCustomDataValue - B Parameter
						InstancedStaticMeshComponent->SetCustomDataValue(InstanceIndex, 2, 0.0f);
						FIntPoint GetTileMapCells = GridSystemComponent->GetCellfromWorldLocation(TileTransform.GetLocation());
						SetupTileMapData(GetTileMapCells, 1);
						
					}
					else if(SelectedArr[x][y] == 2)
					{
						// ??????
						// SetCustomDataValue - R Parameter
						InstancedStaticMeshComponent->SetCustomDataValue(InstanceIndex, 0, 1.0f);
						// SetCustomDataValue - G Parameter
						InstancedStaticMeshComponent->SetCustomDataValue(InstanceIndex, 1, 0.0f);
						// SetCustomDataValue - B Parameter
						InstancedStaticMeshComponent->SetCustomDataValue(InstanceIndex, 2, 0.0f);
						FIntPoint GetTileMapCells = GridSystemComponent->GetCellfromWorldLocation(TileTransform.GetLocation());
						SetupTileMapData(GetTileMapCells, 2);
					}
					
				}
			}
		}
	}
	
}

void ABuildManager::SetupNatural()
{
	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANaturalObject::StaticClass(), AllActors);
	
	for(auto actor : AllActors)
	{
		switch (Cast<ANaturalObject>(actor)->ResourceType)
		{
			case 0:
				FIntPoint ResourceTreeCell = GridSystemComponent->GetCellfromWorldLocation(actor->GetActorLocation());
				ResourceTypeData.Add(ResourceTreeCell, 0);
				break;
			case 1:
				FIntPoint ResourceCliffCell = GridSystemComponent->GetCellfromWorldLocation(actor->GetActorLocation());
				ResourceTypeData.Add(ResourceCliffCell, 1);
				break;
			case 2:
				FIntPoint ResourceIronMineCell = GridSystemComponent->GetCellfromWorldLocation(actor->GetActorLocation());
				ResourceTypeData.Add(ResourceIronMineCell, 2);
				break;
			case 3:
				FIntPoint ResourceCoalMineCell = GridSystemComponent->GetCellfromWorldLocation(actor->GetActorLocation());
				ResourceTypeData.Add(ResourceCoalMineCell, 3);
				break;
		}
		
	}
}


/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   DrawPlacementIndicators
  
  @Category: GridSystem
  
  @Summary:  Draw PlacementIndicators as the Placer's objectsize
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void ABuildManager::DrawPlacementIndicators()
{
	if(GetbCellUnderCursorHasChanged())
	{
		if(IsValid(GridSystemComponent->GetActivePlacer()))
		{
			bool IsEnoughResource = CheckifEnoughResources(GridSystemComponent->GetActivePlacer()->GetObjectData()->ConstructionCost);
			GridSystemComponent->GetActivePlacer()->UpdateMeshMaterial(IsEnoughResource);	
		}
		GridSystemComponent->SetbBuildObjecEnabled(true);
		// Set Placer Object Mesh's World Location
		FVector2D CenterIndicators = GridSystemComponent->GetCenterOfRectangularArea(GetCellUnderCursor(), GridSystemComponent->GetActivePlacer()->GetObjectSize());
		bool bSuccess = false;
		FVector CellLocation = GridSystemComponent->GetCellLocation(GetCellUnderCursor(), GetLocationUnderCursorCamera(), bSuccess);
		float BaseBuildLocationLOCAL = CellLocation.Z;
		GridSystemComponent->GetActivePlacer()->ObjectMesh->SetWorldLocation(FVector(CenterIndicators.X, CenterIndicators.Y, BaseBuildLocationLOCAL));
		TArray<FIntPoint> CellsforBuild = GridSystemComponent->GetCellsinRectangularArea(GetCellUnderCursor(), GridSystemComponent->GetActivePlacer()->GetObjectSize());
		// Object Cell Location setting
		GridSystemComponent->GetActivePlacer()->ObjectCellLocation = CellsforBuild;
		bool IsPossible = false;
		for (int32 Index = 0; Index != CellsforBuild.Num(); Index++)
		{
			if(CheckResourceMapData(GridSystemComponent->GetActivePlacer()->GetResourceType(), CellsforBuild[Index]))
				IsPossible = true;
		}
		
		for (int32 Index = 0; Index != CellsforBuild.Num(); Index++)
		{
			bool bSuccessforIndicators = false;
			FVector CellLocationLOCAL = GridSystemComponent->GetCellLocation(CellsforBuild[Index], GetLocationUnderCursorCamera(),bSuccessforIndicators);
			GridSystemComponent->GetActivePlacer()->PlaceIndicators[Index]->SetWorldLocation(CellLocationLOCAL);

			// ???? ????? ??? ?????? ????? ??
			if(bSuccessforIndicators && GridSystemComponent->GetActivePlacer()->GetObjectData()->IsProductionFacility)
			{
				if(CheckTileMapData(CellsforBuild[Index]))
				{
					// ??? ?????? ?? Indicator Cell???? ???? ?? ?????
					if(CheckOccupancyData(CellsforBuild[Index])==false)
					{
						GridSystemComponent->GetActivePlacer()->PlaceIndicators[Index]->SetMaterial(0, GridSystemComponent->GetActivePlacer()->PlaceAcceptedMaterial);
					}
					else
					{
						GridSystemComponent->GetActivePlacer()->PlaceIndicators[Index]->SetMaterial(0, GridSystemComponent->GetActivePlacer()->PlaceRejectedMaterial);
						GridSystemComponent->SetbBuildObjecEnabled(false);
					}	
				}
				else
				{
					GridSystemComponent->GetActivePlacer()->PlaceIndicators[Index]->SetMaterial(0, GridSystemComponent->GetActivePlacer()->PlaceRejectedMaterial);
					GridSystemComponent->SetbBuildObjecEnabled(false);
				}
			}
			// Resource?? ????? Object???? ???
			else if(bSuccessforIndicators && IsPossible)
			{
				// ??? ?????? ?? Indicator Cell???? ???? ?? ?????
				if(CheckOccupancyData(CellsforBuild[Index])==false)
				{
					GridSystemComponent->GetActivePlacer()->PlaceIndicators[Index]->SetMaterial(0, GridSystemComponent->GetActivePlacer()->PlaceAcceptedMaterial);
				}
				else
				{
					GridSystemComponent->GetActivePlacer()->PlaceIndicators[Index]->SetMaterial(0, GridSystemComponent->GetActivePlacer()->PlaceRejectedMaterial);
					GridSystemComponent->SetbBuildObjecEnabled(false);
				}	
			}
			else // Ground ?·Ї ???? ???? ???
			{
				GridSystemComponent->GetActivePlacer()->PlaceIndicators[Index]->SetMaterial(0, GridSystemComponent->GetActivePlacer()->PlaceRejectedMaterial);
				GridSystemComponent->SetbBuildObjecEnabled(false);
			}
			
		}
	}
}


/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   CheckOccupancyData

  @Category: Data|Occupancy
  
  @Summary:  Check if Object's occupied cell data in BuildManager's OccupancyData
  
  @Args:     FIntPoint Cell
  
  @Return:   bool
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
bool ABuildManager::CheckOccupancyData(FIntPoint Cell)
{
	return GetOccupancyData().Contains(Cell);
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   ChangeOccupancyData

  @Category: Data|Occupancy
  
  @Summary:  Set constructed Object's occupied cell data into BuildManager's OccupancyData
  
  @Args:     FIntPoint Cell, bool IsOccupied
  
  @Modifies: [OccupancyData]
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void ABuildManager::ChangeOccupancyData(FIntPoint Cell, bool IsOccupied)
{	
	if (IsOccupied)
	{
		OccupancyData.Add(Cell, 1);
	}
	else
	{
		if (OccupancyData.Contains(Cell))
		{
			OccupancyData.Remove(Cell);
		}
	}
}

void ABuildManager::SetupTileMapData(FIntPoint Cell, int32 InSoil)
{
	TileMapData.Add(Cell, InSoil);
}

bool ABuildManager::CheckTileMapData(FIntPoint Cell)
{
	if (TileMapData.Contains(Cell))
	{
		// Retrieve the value associated with the key
		const int32& Value = TileMapData[Cell];
		if(Value == 2)
			return false;
	}
	
	return true;
}

bool ABuildManager::CheckResourceMapData(int32 ActivePlacerResourceType, FIntPoint Cell)
{
	// ??? ???? Resource?? ????? ??
	if (ResourceTypeData.Contains(Cell))
	{
		//???? ???? Resouorce Type?? ???? ????????? ? ???
		const int32& Value = ResourceTypeData[Cell];
		if(Value != ActivePlacerResourceType)
			return false;
		else
			return true;
	}
	else if(ActivePlacerResourceType != -1)
		return false;
	return true;
}


/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   SetupObjectData

  @Category: Data|Occupancy
  
  @Summary:  Set constructed Object's data into BuildManager's ObjectData
  
  @Args:     FIntPoint Cell, APlaceableObjectBase* PlaceableObject
  
  @Modifies: [OccupancyData]
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void ABuildManager::SetupObjectData(FIntPoint Cell, APlaceableObjectBase* PlaceableObject)
{
	// If the Data already exists, it will be overwritten.
	ObjectData.Add(Cell, PlaceableObject);
}




void ABuildManager::DestorySelectedPlaceableObject()
{
	if(GridSystemComponent->GetbPlaceableObjectSelected())
	{
		if(IsValid(GetSelectedPlaceableObject()))
		{
			UpdateResourcesValue(GetSelectedPlaceableObject()->GetObjectData()->ConstructionCost, true, false);
			for(FIntPoint cells : GetSelectedPlaceableObject()->OccupiedCells)
			{
				ChangeOccupancyData(cells, false);
			}
			// ???? ??? / ??? ????? ???? ??? ??? ????? object?? ????? ????? ??ыб ???? HappinessTypeData ??????
			if(GetSelectedPlaceableObject()->HappinessFacilityType != 0) // ???? ????? ???
			{
				FIntPoint Range;
				switch (GetSelectedPlaceableObject()->HappinessFacilityType)
				{
				case 0 :
					break;
				case 1 : // road
					Range = FIntPoint(GetPlaceableObjectBase()->GetObjectData()->ObjectSize.X * 3, GetPlaceableObjectBase()->GetObjectData()->ObjectSize.Y * 3);
					break;
				case 2 : // well
					Range = FIntPoint(GetPlaceableObjectBase()->GetObjectData()->ObjectSize.X * 3, GetPlaceableObjectBase()->GetObjectData()->ObjectSize.Y * 3);
					break;
				case 3 : // market
					Range = FIntPoint(GetPlaceableObjectBase()->GetObjectData()->ObjectSize.X * 4, GetPlaceableObjectBase()->GetObjectData()->ObjectSize.Y * 4);
					break;
				case 4 : // church
					Range = FIntPoint(GetPlaceableObjectBase()->GetObjectData()->ObjectSize.X * 7, GetPlaceableObjectBase()->GetObjectData()->ObjectSize.Y * 7);
					break;
				case 5 : // bank
					Range = FIntPoint(GetPlaceableObjectBase()->GetObjectData()->ObjectSize.X * 5, GetPlaceableObjectBase()->GetObjectData()->ObjectSize.Y * 5);
					break;
				}
				TArray<FIntPoint> CellsforBuild = GridSystemComponent->GetCellsinRectangularArea(GridSystemComponent->GetCellfromWorldLocation(GetSelectedPlaceableObject()->GetActorLocation()),
					Range); // ?? ??? ??ъ¤? cell ???
				
				for(FIntPoint cells : CellsforBuild)
				{
					if(ObjectData.Contains(cells) && ObjectData[cells]->IsResidentFacility == true)
					{
						TArray<int32> HappinessType;
						HappinessTypeData.MultiFind(cells, HappinessType);
						for(int32 types : HappinessType)
							ObjectData[cells]->OwnerofHappiness.Remove(types); 
					}
					HappinessTypeData.Remove(cells);
				}
				for(FIntPoint ObjectCells : CellsforBuild)
				{
					if(ObjectData.Contains(ObjectCells))
						ObjectData[ObjectCells]->K2_OnUpdateHappinessData();
				}
				GetSelectedPlaceableObject()->DemolitionPlaceableObject();
			}
		}
	}
}


/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   UpdateResourcesValue

  @Category: Resource
  
  @Summary:  Updates ResourcesValue when Object is Constructed/Demolitioned
  
  @Args:     FConstructionCost Resource, bool Add, bool Subtract
  
  @Modifies: [PlayerResources]
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void ABuildManager::UpdateResourcesValue(FConstructionCost Resource, bool Add, bool Subtract)
{
	if(Add)
	{

		FConstructionCost AddConstruction = FConstructionCost(GetPlayerResources().Gold+Resource.Gold,
			GetPlayerResources().Rice+Resource.Rice,
			GetPlayerResources().Fruit+ Resource.Fruit,
			GetPlayerResources().Meat+ Resource.Meat,
			GetPlayerResources().Wood+Resource.Wood,
			GetPlayerResources().Rock+Resource.Rock,
			GetPlayerResources().Iron+Resource.Iron,
			GetPlayerResources().Coal+Resource.Coal,
			GetPlayerResources().TotalNum + Resource.TotalNum,
			GetPlayerResources().UsedNum - Resource.UsedNum
			);
		
		SetPlayerResources(AddConstruction);
	}
	else if(Subtract)
	{
		
		
		FConstructionCost SubConstruction = FConstructionCost(FMath::Clamp(GetPlayerResources().Gold-Resource.Gold, 0 , GetPlayerResources().Gold-Resource.Gold),
		FMath::Clamp(GetPlayerResources().Rice-Resource.Rice,0,GetPlayerResources().Rice-Resource.Rice),
	FMath::Clamp(GetPlayerResources().Fruit-Resource.Fruit,0,GetPlayerResources().Fruit-Resource.Fruit),
	FMath::Clamp(GetPlayerResources().Meat-Resource.Meat,0,GetPlayerResources().Meat-Resource.Meat),
			FMath::Clamp(GetPlayerResources().Wood-Resource.Wood, 0 , GetPlayerResources().Wood-Resource.Wood),
			FMath::Clamp(GetPlayerResources().Rock-Resource.Rock, 0 , GetPlayerResources().Rock-Resource.Rock),
			FMath::Clamp(GetPlayerResources().Iron-Resource.Iron, 0 , GetPlayerResources().Iron-Resource.Iron),
			FMath::Clamp(GetPlayerResources().Coal-Resource.Coal, 0 , GetPlayerResources().Coal-Resource.Coal),
			FMath::Clamp(GetPlayerResources().TotalNum - Resource.TotalNum,0,GetPlayerResources().TotalNum - Resource.TotalNum),
			FMath::Clamp(GetPlayerResources().UsedNum + Resource.UsedNum,0,GetPlayerResources().UsedNum + Resource.UsedNum)
			);

		SetPlayerResources(SubConstruction);
	}
	else
	{
		FConstructionCost Construction = FConstructionCost(FMath::Clamp(Resource.Gold, 0, Resource.Gold),
		FMath::Clamp(Resource.Rice,0,Resource.Rice),
	FMath::Clamp(Resource.Fruit,0,Resource.Fruit),
	FMath::Clamp(Resource.Meat,0,Resource.Meat),	
		FMath::Clamp(Resource.Wood, 0, Resource.Wood),
		FMath::Clamp(Resource.Rock, 0, Resource.Rock),
		FMath::Clamp(Resource.Iron, 0, Resource.Iron),
		FMath::Clamp(Resource.Coal, 0, Resource.Coal),
		FMath::Clamp(Resource.TotalNum,0,Resource.TotalNum),
		FMath::Clamp(Resource.UsedNum,0,Resource.UsedNum)
		);

		SetPlayerResources(Construction);
	}

	if(IsValid(GridSystemComponent->GetActivePlacer()))
	{
		bool IsEnoughResource = CheckifEnoughResources(GridSystemComponent->GetActivePlacer()->GetObjectData()->ConstructionCost);
		GridSystemComponent->GetActivePlacer()->UpdateMeshMaterial(IsEnoughResource);	
	}
	
	
}





/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   SetOutlineColor
  
  @Summary:  Local Function for set Material Collection Parameter to set outline color
  
  @Args:     int32 ObjectSide
             PlaceableObject's Outline Color determining demolition
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void ABuildManager::SetOutlineColor()
{
	if(IsValid(GetPCI()))
	{
		GetPCI()->SetVectorParameterValue(FName("OutlineColor"), OutlineColor);
	}
}






/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   LCheckifEnoughResources

  @Summary:  Local Function for return Check Resource
  
  @Args:     FConstructionCost InCost
			 The Cost need for Construction
  
  @Returns:  bool
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
bool ABuildManager::CheckifEnoughResources(FConstructionCost InCost)
{
	
	bool CheckPlayerResource = GetPlayerResources().Gold - InCost.Gold >= 0 && GetPlayerResources().Fruit - InCost.Fruit >=0 && GetPlayerResources().Rice - InCost.Rice >=0
		&& GetPlayerResources().Meat - InCost.Meat >=0 && GetPlayerResources().Coal - InCost.Coal >= 0 && GetPlayerResources().Iron - InCost.Iron >= 0
		&& GetPlayerResources().Rock - InCost.Rock >=0 && GetPlayerResources().Wood - InCost.Wood >=0
		&& (GetPlayerResources().TotalNum- GetPlayerResources().UsedNum) -  InCost.UsedNum >= 0;

	return CheckPlayerResource;
}






