// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildManager.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Engine/PostProcessVolume.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"

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
	RootComponent = InstancedStaticMeshComponent;

	SetGridCenterLocation(GetActorLocation());
	
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
	UpdateResourcesValue(FConstructionCost(2000, FFoodData(100,100,100), 100, 100, 100, 100), false, false);

	
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
	if(GetbBuildToolEnabled() || GetbDemolitionToolEnabled())
	{
		UpdateBuildingManagerValues();
		if(GetbPlacerIndicatorEnabled())
		{
			DrawPlacementIndicators();
		}
		if(GetbDemolitionToolEnabled())
		{
			//todo : Destroy Object Under Cursor
		}
		DetectMouseDrag();
	}

}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   EndPlay
  
  @Summary:  Called when the game ends
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void ABuildManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	FUpdateResourceAmountEvent.Unbind();
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
	if(IsRemove && GetPlaceableObjectUnderCursor() == InPlaceableObjectBase) // Cursor End Overlap 에서 객체 취소
	{
		SetPlaceableObjectUnderCursor(nullptr);
	}
	else // Cursor Begin Overlap
	{
		SetPlaceableObjectUnderCursor(InPlaceableObjectBase);
		if(IsValid(GetPlaceableObjectUnderCursor()))
		{
			// Demolition이 가능한지 판단
			if(GetbDemolitionToolEnabled())
			{
				if(IsValid(GetPCI()))
					GetPCI()->SetScalarParameterValue(FName("EnableShading"), 1.0f);
			}
			else
			{
				LSetOutlineColor(GetPlaceableObjectUnderCursor()->GetObjectSide());
				if(IsValid(GetPCI()))
					GetPCI()->SetScalarParameterValue(FName("EnableShading"), -0.5f);
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
//todo: 현재는 bp에서 키인풋으로 호출하도록 구현 -> Placer에서 판단 후 호출하도록 수정 
void ABuildManager::BuildPlaceableObject()
{
	FVector2D SpawnCenterLocation = LGetCenterOfRectangularArea(GetCellUnderCursor(), GetActivePlacer()->GetObjectSize());
	FVector SpawnLocation = FVector(SpawnCenterLocation.X, SpawnCenterLocation.Y, GetLocationUnderCursorCamera().Z);
	FRotator rotator;
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	if(GetWorld())
	{
		SetPlaceableObjectBase(Cast<APlaceableObjectBase>(GetWorld()->SpawnActor<AActor>(APlaceableObjectBase::StaticClass(), SpawnLocation, rotator, SpawnParams)));
		FDataTableRowHandle NewOjectDataRow;
		NewOjectDataRow.DataTable = GetPlaceableObjectBase()->GetObjectNameInTable().DataTable;
		NewOjectDataRow.RowName =  GetActivePlacer()->GetObjectNameInTable().RowName;
		GetPlaceableObjectBase()->SetObjectNameInTable(NewOjectDataRow);
		//DynamicObject 구조체 PlaceableObject 생성시 설정
		FDynamicPlaceableObjectData ObjectDynamicData = FDynamicPlaceableObjectData(true, GetCellUnderCursor(), GetActivePlacer()->GetBuildDirection(), 0.0f, 0.0f);
		GetPlaceableObjectBase()->SetObjectDynamicData(ObjectDynamicData);

		// Object RowName, DynamicData 설정 후 Setting 진행
		GetPlaceableObjectBase()->SetupPlaceableObject();
		GetPlaceableObjectBase()->SetupOutline();
	}

	if(IsValid(GetPlaceableObjectBase()))
	{
		// Build Manger에서 점유된 셀 정보 세팅해주기
		if(GetPlaceableObjectBase()->GetObjectDynamicData().HasData)
		{
			TArray<FIntPoint> GetOccupiedCells = LGetCellsinRectangularArea(GetPlaceableObjectBase()->GetOccupiedCenterCell(), GetPlaceableObjectBase()->GetObjectSize());
			for (FIntPoint cells : GetOccupiedCells)
			{
				ChangeOccupancyData(cells, true);
				SetObjectData(cells, GetPlaceableObjectBase());
			}
		}
		else // Editor 상에서 진행한 경우
		{
			GetPlaceableObjectBase()->SetOccupiedCenterCell(FIntPoint(GetActorLocation().X, GetActorLocation().Y));
			TArray<FIntPoint> GetOccupiedCells = LGetCellsinRectangularArea(GetCellfromWorldLocation(GetPlaceableObjectBase()->GetActorLocation()), GetPlaceableObjectBase()->GetObjectSize());
			for (FIntPoint cells : GetOccupiedCells)
			{
				ChangeOccupancyData(cells, true);
				SetObjectData(cells, GetPlaceableObjectBase());
			}
		}
		// Binding event when PlaceableObject construct
		GetPlaceableObjectBase()->UpdatePlaceableObjectCursorEvent.BindUFunction(this, FName("CallUpdatePlaceableObjectUnderCursor"));
	}
}



/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   PressedLMB
  
  @Category: Main
 
  @Summary:  Left Сlick to select objects under Cursor

  @Modifies: [bInteractStarted, bDragWasInterrupted,
             StartLocationUnderCursor]
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void ABuildManager::PressedLMB()
{
	SetbInteractStarted(true); // 클릭했는지 Released에서 확인용
	SelectPlaceableObject();
	if(GetbObjectForPlacementIsSelected())
	{
		SetbDragWasInterrupted(false);
		FHitResult ResultCamera;
		if(GetPlayerController()->GetHitResultUnderCursorByChannel(TraceTypeQuery2, bTraceComplex, ResultCamera))
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
	if(GetbInteractStarted())
	{
		if(GetbBuildToolEnabled() && GetbObjectForPlacementIsSelected())
		{
			if(IsValid(GetActivePlacer()))
			{
				if(LCheckifEnoughResources(GetActivePlacer()->GetObjectData()->ConstructionCost))
				{
					BuildPlaceableObject();
				}
			}
		}
		SetbInteractStarted(false);
		SetbDragStarted(false);
		SetbDragWasInterrupted(false);
	}
	
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
	//By default, two channels are checked, camera and visibility
	FHitResult ResultVisiblity;
	if(GetPlayerController()->GetHitResultUnderCursorByChannel(TraceTypeQuery1, bTraceComplex, ResultVisiblity))
	{
		if(ResultVisiblity.bBlockingHit)
		{
			SetLocationUnderCursorVisibility(ResultVisiblity.Location);
			SetActorUnderCursor(ResultVisiblity.GetActor());
		}
	}
	FHitResult ResultCamera;
	if(GetPlayerController()->GetHitResultUnderCursorByChannel(TraceTypeQuery2, bTraceComplex, ResultCamera))
	{
		if(ResultCamera.bBlockingHit)
		{
			SetLocationUnderCursorCamera(ResultCamera.Location);
		}
	}

	//Checking if the cursor has moved to a new grid cell
	//This is necessary for optimization, checking the area for building a building is done once when the cell under the cursor is changed
	if( GetCellUnderCursor() != GetLastCellUnderCursor())
	{
		SetLastCellUnderCursor(GetCellUnderCursor());
		SetbCellUnderCursorHasChanged(true);
	}
	else
		SetbCellUnderCursorHasChanged(false);

	
	FIntPoint Cell= GetCellfromWorldLocation(GetLocationUnderCursorCamera());
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
	if (GetbBuildToolEnabled() == false) 
	{
		// 1. Placeable Object 아래에 커서가 존재할 경우
		if(IsValid(GetPlaceableObjectUnderCursor()))
		{
			// 1-1. Placeable Object 객체가 기존에 선택된것인지 체크->오브젝트 상태변경
			if (IsValid(GetSelectedPlaceableObject()))
			{
				if (GetPlaceableObjectUnderCursor() != GetSelectedPlaceableObject())
					GetSelectedPlaceableObject()->SetObjectSelectedMode(false); // 기존에 선택된 Object는 비워줌.
			}
			SetSelectedPlaceableObject(PlaceableObjectUnderCursor); // 커서 아래있는 오브젝트가 현재 선택된 오브젝트로 변경
			SetbPlaceableObjectSelected(true);
			GetSelectedPlaceableObject()->SetObjectSelectedMode(true);
			// 클릭시 Outline, Material Color 변경
			LSetOutlineColor(1);
			if(IsValid(GetPCI()))
				GetPCI()->SetScalarParameterValue(FName("EnableShading"), 1.0f);
		}
		// 2. Placeable Object 아래에 커서가 존재하지 않는 경우
		else
		{
			SetbPlaceableObjectSelected(false); // 선택된 상태가 아닌 것으로 판단
			if (IsValid(GetSelectedPlaceableObject())) 
			{
				GetSelectedPlaceableObject()->SetObjectSelectedMode(false);
				SetSelectedPlaceableObject(nullptr);
			}
		}
	}
}

void ABuildManager::DeselectPlaceableObject()
{
	if(GetbPlaceableObjectSelected() && IsValid(GetSelectedPlaceableObject()))
	{
		GetSelectedPlaceableObject()->SetObjectSelectedMode(false);
		SetbPlaceableObjectSelected(false);
		if(GetPlaceableObjectUnderCursor() == GetSelectedPlaceableObject())
		{
			GetSelectedPlaceableObject()->SwapObjectHighlighting(true);
		}
		SetSelectedPlaceableObject(nullptr);
	}
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   CancelDragObjectPlacing
  
  @Category: Main
  
  @Summary:  Detect Cancel Drag and Hide activated Placer Actor

  @Modifies: [bDragWasInterrupted]
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void ABuildManager::CancelDragObjectPlacing()
{
	if(GetbInteractStarted())
	{
		SetbDragWasInterrupted(true); // Drag Cancel Detect
		if(IsValid(GetActivePlacer()))
		{
			GetActivePlacer()->HidePlaceIndicators();
		}
	}
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   DetectMouseDrag
  
  @Category: Main
  
  @Summary:  Set bool DragStart Variable

  @Modifies: [bDragStarted]
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void ABuildManager::DetectMouseDrag()
{
	if(IsValid(GetActivePlacer()))
	{
		// Placer가 드래그가능한 상태, BuildManager는 Interact 시작이면서 Drag는 안한 상태
		if(GetActivePlacer()->GetbCanbeDraggged() && GetbInteractStarted() && GetbDragStarted()==false)
		{
			float Squared = UKismetMathLibrary::VSize2DSquared(static_cast<FVector2d>(GetStartLocationUnderCursor()-GetLocationUnderCursorCamera()));
			if(Squared > GetStartDragInstance()* GetStartDragInstance())
				SetbDragStarted(true);
		}
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
	if(GetbBuildToolEnabled())
		GetActivePlacer()->RotateObjectPlacer(true);
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   SpawnGrid

  @Category: GridSystem
  
  @Summary:  Spawn Tile Map with Instanced static mesh
  
  @Args:     FVector CenterLocation, FVector TileSize, FIntPoint TileCount
              Tile Map's Center Location, Size, Count

  @Modifies: [GridCenterLocation, GridTileSize, GridTileCount,
              InstancedStaticMeshComponent]
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void ABuildManager::SpawnGrid(FVector CenterLocation, FVector TileSize, FIntPoint TileCount)
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
	LCalculateCenterandBottomLeft();

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

			FTransform TileTransform;
			TileTransform.SetLocation(TileTransformLocation);
			TileTransform.SetScale3D(TileTransformScale);
			// Add Instanced Static Mesh
			int32 InstanceIndex = InstancedStaticMeshComponent->AddInstance(TileTransform, true);
			
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


/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   DrawPlacementIndicators
  
  @Category: GridSystem
  
  @Summary:  Draw PlacementIndicators as the Placer's objectsize
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void ABuildManager::DrawPlacementIndicators()
{
	if(GetbCellUnderCursorHasChanged())
	{
		// Set Placer Object Mesh's World Location
		FVector2D CenterIndicators = LGetCenterOfRectangularArea(GetCellUnderCursor(), GetActivePlacer()->GetObjectSize());
		bool bSuccess = false;
		FVector CellLocation = GetCellLocation(GetCellUnderCursor(), bSuccess);
		float BaseBuildLocationLOCAL = CellLocation.Z;
		GetActivePlacer()->ObjectMesh->SetWorldLocation(FVector(CenterIndicators.X, CenterIndicators.Y, BaseBuildLocationLOCAL));
		
		TArray<FIntPoint> CellsforBuild = LGetCellsinRectangularArea(GetCellUnderCursor(), GetActivePlacer()->GetObjectSize());
		for (int32 Index = 0; Index != CellsforBuild.Num(); Index++)
		{
			bool bSuccessforIndicators = false;
			FVector CellLocationLOCAL = GetCellLocation(CellsforBuild[Index], bSuccessforIndicators);
			GetActivePlacer()->PlaceIndicators[Index]->SetWorldLocation(CellLocationLOCAL);

			// 해당 공간에 각 Indicator Cell들을 놓을 수 있는지
			if( bSuccessforIndicators && CheckOccupancyData(CellsforBuild[Index]) == false )
			{
				GetActivePlacer()->PlaceIndicators[Index]->SetMaterial(0, GetActivePlacer()->PlaceAcceptedMaterial);
			}
			else
			{
				GetActivePlacer()->PlaceIndicators[Index]->SetMaterial(0, GetActivePlacer()->PlaceRejectedMaterial);
			}
		}
	}
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   GetCellLocation
  
  @Category: GridSystem
  
  @Summary:  Get Changed Cell Location's Location(Z is Zero)
  
  @Args:     FIntPoint InCell, bool& bSuccess
             Cell Value, and bool that Line Trace Hit was occured
  
  @Returns:  FVector
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
FVector ABuildManager::GetCellLocation(FIntPoint InCell, bool& bSuccess)
{
	FVector2D CellCenter = LGetCellCenterToLocation(InCell);
	FVector LocationLOCAL = FVector(CellCenter.X, CellCenter.Y, 0.0f);

	FHitResult HitResult;
	FVector StartLocation = FVector(LocationLOCAL.X, LocationLOCAL.Y, GetStartTraceHeight()+GetLocationUnderCursorCamera().Z);
	FVector EndLocation =  FVector(LocationLOCAL.X, LocationLOCAL.Y, GetEndTraceHeight()+GetLocationUnderCursorCamera().Z);
	TArray<AActor*> ActorsToIgnore;

	bool IsHitResult = UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		StartLocation,
		EndLocation,
		TraceTypeQuery2,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		0.5f);
	if(IsHitResult == true)
	{
		float RoundHeight = LRoundHeightToGridStep(HitResult.Location.Z);
		LocationLOCAL = FVector(LocationLOCAL.X, LocationLOCAL.Y, RoundHeight);
		bSuccess = true;
		return LocationLOCAL;
	}
	bSuccess = false;
	return LocationLOCAL;	
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   GetCellfromWorldLocation

  @Summary:  Local Function for return grid cell from World Location
  
  @Args:     FVector Location
			 The World location(Mouse Cursor) for getting cell
  
  @Returns:  FIntPoint
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
FIntPoint ABuildManager::GetCellfromWorldLocation(FVector Location)
{
	int32 X = FMath::Abs(FMath::RoundToInt32(Location.X / GetGridTileSize().X)) * FMath::Sign(FMath::RoundToInt32(Location.X / GetGridTileSize().X));
	int32 Y = FMath::Abs(FMath::RoundToInt32(Location.Y / GetGridTileSize().Y)) * FMath::Sign(FMath::RoundToInt32(Location.Y / GetGridTileSize().Y));
	
	return FIntPoint(X,Y);
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

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   SetOccupancyData

  @Category: Data|Occupancy
  
  @Summary:  Set constructed Object's data into BuildManager's ObjectData
  
  @Args:     FIntPoint Cell, APlaceableObjectBase* PlaceableObject
  
  @Modifies: [OccupancyData]
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void ABuildManager::SetObjectData(FIntPoint Cell, APlaceableObjectBase* PlaceableObject)
{
	// If the Data already exists, it will be overwritten.
	GetObjectData().Add(Cell, PlaceableObject);
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   ActivateBuildingTool

  @Category: Tool
  
  @Summary:  Spawn Placer Actor and change to new activate Placer
  
  @Args:     FDataTableRowHandle ObjectforBuilding
             the object's data row for change
  
  @Modifies: [bBuildToolEnabled, 
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void ABuildManager::ActivateBuildingTool(FDataTableRowHandle ObjectforBuilding)
{
	DeactivateBuildingTool();
	LChangeObjectforPlacement(ObjectforBuilding);
	if(GetbObjectForPlacementIsSelected())
	{
		SetbBuildToolEnabled(true);
		if(IsValid(GetActivePlacer()))
		{
			GetActivePlacer()->DeactivateObjectPlacer();
		}
		UpdateBuildingManagerValues();
		FVector SpawnLocation = GetLocationUnderCursorCamera();
		FRotator rotator;
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		if(GetWorld())
		{
			SetActivePlacer(Cast<APlacerObjectBase>(GetWorld()->SpawnActor<AActor>(APlacerObjectBase::StaticClass(), SpawnLocation, rotator, SpawnParams)));
		}
		if(IsValid(GetActivePlacer()))
		{
			GetActivePlacer()->ActivateObjectPlacer(); // Activate Object Placer
			for(int32 index = 0; index <= GetActivePlacer()->GetObjectSize().X * GetActivePlacer()->GetObjectSize().Y -1; index++ )
			{
				GetActivePlacer()->CreateIndicatorMesh(true);
				GetActivePlacer()->PlaceIndicators[index]->SetRelativeScale3D(FVector(GetGridTileSize().X/100 , GetGridTileSize().Y /100, 1.0f));
			}
			SetbPlacerIndicatorEnabled(true);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString::Printf(TEXT("GetActivePlacer is NULL")));
		}
	}
	
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   DeactivateBuildingTool
  
  @Category: Tool
  
  @Summary:  Cancel Placing and Destroy current activated Placer Actor
  
  @Modifies: [bObjectForPlacementIsSelected, bBuildToolEnabled]
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void ABuildManager::DeactivateBuildingTool()
{
	if(GetbBuildToolEnabled())
	{
		if(GetbDragStarted()) // 이미 드래그중이었는지
			CancelDragObjectPlacing();
		else
		{
			SetbPlaceableObjectSelected(false);
			SetbBuildToolEnabled(false);
			if(IsValid(GetActivePlacer()))
			{
				GetActivePlacer()->DeactivateObjectPlacer();
			}
		}
	}
		
	
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   ActivateDemolitionTool
  
  @Category: Tool
  
  @Summary:  Actiavte Dmeolitoin tool by Deactivate Building tool
  
  @Modifies: [bDemolitionToolEnabled]
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void ABuildManager::ActivateDemolitionTool()
{
	DeactivateBuildingTool();
	SetbDemolitionToolEnabled(false);
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   DeactivateDemolitionTool
  
  @Category: Tool
  
  @Summary:  Deactiavte Dmeolitoin tool
  
  @Modifies: [bDemolitionToolEnabled]
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void ABuildManager::DeactivateDemolitionTool()
{
	if(GetbDemolitionToolEnabled()) //원래 활성화되어있었는지 확인
	{
		SetbDemolitionToolEnabled(true);
		//CallUpdatePlaceableObjectUnderCursor(GetPlaceableObjectUnderCursor(), false); 
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
	/*
	// Call Event Delegate
	if(UpdateResourceAmountEvent.IsBound())
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Execute")));
		UpdateResourceAmountEvent.Execute(GetPlayerResources());
	}
	*/
}


/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   LSnapVectorToVector
  
  @Summary:  Local Funtion for Sanp Vector to Vector
  
  @Args:     FVector CurrentPosition, FVector SnapValue
  
  @Returns:  FVector
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
FVector ABuildManager::LSnapVectorToVector(FVector CurrentPosition, FVector SnapValue)
{
	FVector Return;
	Return.X = FMath::RoundToInt32(CurrentPosition.X / SnapValue.X)* SnapValue.X;
	Return.Y = FMath::RoundToInt32(CurrentPosition.Y / SnapValue.Y) * SnapValue.Y;
	Return.Z = FMath::RoundToInt32(CurrentPosition.Z / SnapValue.Z) * SnapValue.Z;
	
	return Return;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   LSnapFlaotToFloat
  
  @Summary:  Local Funtion for Sanp Float to Float
  
  @Args:     float CurrentPosition, float SnapValue
  
  @Returns:  float
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
float ABuildManager::LSnapFlaotToFloat(float CurrentLocation, float GridSize)
{
	float Return = FMath::RoundToInt32(CurrentLocation / GridSize) * GridSize;

	return Return;

}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   LCalculateCenterandBottomLeft
  
  @Summary:  Local Funtion for Calculate BottomLeftCorner using Center Location
  
  @Args:     float CurrentPosition, float SnapValue
  
  @Modifies: [GridCenterLocation, GridBottomLeftCornerLocation]
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void ABuildManager::LCalculateCenterandBottomLeft()
{
	FVector NewVector = LSnapVectorToVector(GridCenterLocation, GridTileSize);
	FIntPoint AlignVector;

	AlignVector.X = GridTileCount.X % 2 == 0 ? 0 : 1;
	AlignVector.Y = GridTileCount.Y, 2 == 0 ? 0 : 1;

	FIntPoint Divided = (GridTileCount - AlignVector) / 2;
	FVector GridCount = FVector(Divided.X * GridTileSize.X, Divided.Y * GridTileSize.Y, 1.0f);
	
	SetGridCenterLocation(NewVector);
	SetGridBottomLeftCornerLocation(NewVector - GridCount);
	
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   LSetOutlineColor
  
  @Summary:  Local Function for set Material Collection Parameter to set outline color
  
  @Args:     int32 ObjectSide
             PlaceableObject's Outline Color determining demolition
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void ABuildManager::LSetOutlineColor(int32 ObjectSide)
{
	if(IsValid(GetPCI()))
	{
		switch (ObjectSide)
		{
		case 0:
			GetPCI()->SetVectorParameterValue(FName("OutlineColor"), PlayerOutlineColor);
			break;
		case 1:
			GetPCI()->SetVectorParameterValue(FName("OutlineColor"), EnemyOutlineColor);
			break;
		case 2:
			GetPCI()->SetVectorParameterValue(FName("OutlineColor"), NeturalOutlineColor);
			break;
		default:
			GetPCI()->SetVectorParameterValue(FName("OutlineColor"), NeturalOutlineColor);
			break;
		}
	}
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   LChangeObjectforPlacement
  
  @Summary:  Local Function for Changing Object Data using new DT Row
  
  @Args:     FDataTableRowHandle NewObjectRow
  
  @Modifies: [ObjectForPlacement, PlaceableObjectData,
             bObjectForPlacementIsSelected]
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void ABuildManager::LChangeObjectforPlacement(FDataTableRowHandle NewObjectRow)
{
	SetObjectForPlacement(NewObjectRow);
	if(GetObjectForPlacement().RowName != FName("None"))
	{
		FName LocalRowName = GetObjectForPlacement().RowName;
		FPlaceableObjectData* OutRow = GetObjectForPlacement().DataTable->FindRow<FPlaceableObjectData>(LocalRowName, "");
		if (OutRow != nullptr)
		{
			SetbObjectForPlacementIsSelected(true);
			SetPlaceableObjectData(OutRow);
		}
		else
			SetbObjectForPlacementIsSelected(false);
	}
	else
		SetbObjectForPlacementIsSelected(false);
	
}



/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   LGetCellCenterToLocation
  
  @Summary:  Local Function for return grid cell Center to Location Vector
  
  @Args:     FIntPoint InCell
  
  @Returns:  FVector2D
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
FVector2D ABuildManager::LGetCellCenterToLocation(FIntPoint InCell)
{
	float LocationX = FMath::Abs(InCell.X) * GetGridTileSize().X * FMath::Sign(InCell.X);
	float LocationY = FMath::Abs(InCell.Y) * GetGridTileSize().Y * FMath::Sign(InCell.Y);
	
	return FVector2D(LocationX, LocationY);
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   LRoundHeightToGridStep
  
  @Summary:  Local Function for return grid round height by tracing new height
  
  @Args:     float InNewHeight
  
  @Returns:  float
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
float ABuildManager::LRoundHeightToGridStep(float InNewHeight)
{
	if(UKismetMathLibrary::NearlyEqual_FloatFloat(GetVerticalStep(), 0.0f, 0.01f))
	{
		return InNewHeight;
	}
	else
	{
		return FMath::RoundToInt32(InNewHeight / GetVerticalStep()) * GetVerticalStep();
	}
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   LGetCenterOfRectangularArea

  @Summary:  Get Cells'Center by the size of PlaceableObject's Objectsize.
  
  @Args:     FIntPoint CenterLocation, FIntPoint TileCount
			 Object's OccupiedCenterCell, Object's count
  
  @Returns:  TArray<FIntPoint>
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
FVector2D ABuildManager::LGetCenterOfRectangularArea(FIntPoint AreaCenterCell, FIntPoint AreaSize)
{
	FVector2D LocationLOCAL = LGetCellCenterToLocation(AreaCenterCell);
	
	float LocationCenterX = AreaSize.X % 2 == 0 ? LocationLOCAL.X - (GetGridTileSize().X * 0.5f) : LocationLOCAL.X;
	float LocationCenterY = AreaSize.Y % 2 == 0 ? LocationLOCAL.Y - (GetGridTileSize().Y * 0.5f) : LocationLOCAL.Y;
	
	return FVector2D(LocationCenterX, LocationCenterY);
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   LGetCellsinRectangularArea

  @Summary:  Get Cells by the size of PlaceableObject's Objectsize.
  
  @Args:     FIntPoint CenterLocation, FIntPoint TileCount
			 Object's OccupiedCenterCell, Object's count
  
  @Returns:  TArray<FIntPoint>
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
TArray<FIntPoint> ABuildManager::LGetCellsinRectangularArea(FIntPoint CenterLocation, FIntPoint TileCount)
{
	int32 CurrentCellX = 0;
	int32 CurrentCellY = 0;
	FIntPoint StartCellLOCAL = FIntPoint((CenterLocation.X-(TileCount.X/2)),(CenterLocation.Y-(TileCount.Y/2)));
	TArray<FIntPoint> CellsLOCAL;
	int32 count = 0;
	for (int Index = 0; Index <= (TileCount.X) - 1; Index++)
	{
		CurrentCellX = StartCellLOCAL.X + Index;
		for (int Index2 = 0; Index2 <= (TileCount.Y) - 1; Index2++)
		{
			CurrentCellY = StartCellLOCAL.Y + Index2;
			CellsLOCAL.Add(FIntPoint(CurrentCellX, CurrentCellY));
			count+=1;
		}
	}
	return CellsLOCAL;
}




/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   LCheckifEnoughResources

  @Summary:  Local Function for return Check Resource
  
  @Args:     FConstructionCost InCost
			 The Cost need for Construction
  
  @Returns:  bool
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
bool ABuildManager::LCheckifEnoughResources(FConstructionCost InCost)
{
	return GetPlayerResources().Gold - InCost.Gold >= 0 && GetPlayerResources().Food.Fruit - InCost.Food.Fruit >=0 && GetPlayerResources().Food.Rice - InCost.Food.Rice >=0
		&& GetPlayerResources().Food.Meat - InCost.Food.Meat >=0 && GetPlayerResources().Coal - InCost.Coal >= 0 && GetPlayerResources().Iron - InCost.Iron >= 0
		&& GetPlayerResources().Rock - InCost.Rock >=0 && GetPlayerResources().Wood - InCost.Wood >=0;
}






