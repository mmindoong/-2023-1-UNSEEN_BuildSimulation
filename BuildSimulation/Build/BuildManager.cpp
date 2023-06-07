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
	GridSystemComponent = CreateDefaultSubobject<UGridSystemComponent>(TEXT("GridSystemComponent"));
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
	UpdateResourcesValue(FConstructionCost(2000, FFoodData(1000,1000,1000), 1000, 1000, 1000, 1000), false, false);
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
		if(GridSystemComponent->GetbDemolitionToolEnabled())
		{
			if(GridSystemComponent->GetbInteractStarted())
			{
				FTimerHandle WaitHandle;
				float WaitTime = 0.07f; 
				GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
				{
					//todo : Destroy Object Under Cursor

				}), WaitTime, false); 
			}
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
			if(GridSystemComponent->GetbDemolitionToolEnabled())
			{
				if(IsValid(GetPCI()))
					GetPCI()->SetScalarParameterValue(FName("EnableShading"), 1.0f);
			}
			else
			{
				SetOutlineColor(GetPlaceableObjectUnderCursor()->GetObjectSide());
				if(IsValid(GetPCI()))
					GetPCI()->SetScalarParameterValue(FName("EnableShading"), -0.5f);
			}
		}
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
	GridSystemComponent->SetbInteractStarted(true); // 클릭했는지 Released에서 확인용
	SelectPlaceableObject();
	if(GridSystemComponent->GetbObjectForPlacementIsSelected())
	{
		GridSystemComponent->SetbDragWasInterrupted(false);
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
	if(GridSystemComponent->GetbInteractStarted())
	{
		if(GridSystemComponent->GetbBuildToolEnabled() && GridSystemComponent->GetbObjectForPlacementIsSelected())
		{
			if(IsValid(GridSystemComponent->GetActivePlacer()))
			{
				if(CheckifEnoughResources(GridSystemComponent->GetActivePlacer()->GetObjectData()->ConstructionCost))
				{
					BuildPlaceableObject();
				}
			}
		}
		GridSystemComponent->SetbInteractStarted(false);
		GridSystemComponent->SetbDragStarted(false);
		GridSystemComponent->SetbDragWasInterrupted(false);
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
		SetPlaceableObjectBase(Cast<APlaceableObjectBase>(GetWorld()->SpawnActor<AActor>(APlaceableObjectBase::StaticClass(), SpawnLocation, SpawnRotator, SpawnParams)));
		FDataTableRowHandle NewOjectDataRow;
		NewOjectDataRow.DataTable = GetPlaceableObjectBase()->GetObjectNameInTable().DataTable;
			NewOjectDataRow.RowName =  GridSystemComponent->GetActivePlacer()->GetObjectNameInTable().RowName;
			GetPlaceableObjectBase()->SetObjectNameInTable(NewOjectDataRow);
			//DynamicObject 구조체 PlaceableObject 생성시 설정
			FDynamicPlaceableObjectData ObjectDynamicData = FDynamicPlaceableObjectData(true, GetCellUnderCursor(),GridSystemComponent->GetActivePlacer()->GetBuildDirection(), 0.0f, 0.0f);
			GetPlaceableObjectBase()->SetObjectDynamicData(ObjectDynamicData);

			// Object RowName, DynamicData 설정 후 Setting 진행
			GetPlaceableObjectBase()->SetupPlaceableObject();
		GetPlaceableObjectBase()->SetupOutline();
		UpdateResourcesValue(GetPlaceableObjectBase()->GetObjectData()->ConstructionCost, false, true);
	}

	if(IsValid(GetPlaceableObjectBase()))
	{
		// Build Manger에서 점유된 셀 정보 세팅해주기
		if(GetPlaceableObjectBase()->GetObjectDynamicData().HasData)
		{
			TArray<FIntPoint> GetOccupiedCells = GridSystemComponent->GetCellsinRectangularArea(GetPlaceableObjectBase()->GetOccupiedCenterCell(), GetPlaceableObjectBase()->GetObjectSize());
			for (FIntPoint cells : GetOccupiedCells)
			{
				ChangeOccupancyData(cells, true);
				SetupObjectData(cells, GetPlaceableObjectBase());
				GetPlaceableObjectBase()->OccupiedCells.Add(cells);
			}
		}
		else 
		{
			GetPlaceableObjectBase()->SetOccupiedCenterCell(FIntPoint(GetActorLocation().X, GetActorLocation().Y));
			TArray<FIntPoint> GetOccupiedCells = GridSystemComponent->GetCellsinRectangularArea(GridSystemComponent->GetCellfromWorldLocation(GetPlaceableObjectBase()->GetActorLocation()), GetPlaceableObjectBase()->GetObjectSize());
			for (FIntPoint cells : GetOccupiedCells)
			{
				ChangeOccupancyData(cells, true);
				SetupObjectData(cells, GetPlaceableObjectBase());
				GetPlaceableObjectBase()->OccupiedCells.Add(cells);
			}
		}
		// Binding event when PlaceableObject construct
		GetPlaceableObjectBase()->UpdatePlaceableObjectCursorEvent.BindUFunction(this, FName("CallUpdatePlaceableObjectUnderCursor"));
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
			GridSystemComponent->SetLocationUnderCursorCamera(GetLocationUnderCursorCamera());
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

	
	FIntPoint Cell= GridSystemComponent->GetCellfromWorldLocation(GetLocationUnderCursorCamera());
	SetCellUnderCursor(Cell);

}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   DetectMouseDrag
  
  @Category: Main
  
  @Summary:  Set bool DragStart Variable

  @Modifies: [bDragStarted]
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void ABuildManager::DetectMouseDrag()
{
	if(IsValid(GridSystemComponent->GetActivePlacer()))
	{
		// Placer가 드래그가능한 상태, BuildManager는 Interact 시작이면서 Drag는 안한 상태
		if(GridSystemComponent->GetActivePlacer()->GetbCanbeDraggged() && GridSystemComponent->GetbInteractStarted() && GridSystemComponent->GetbDragStarted()==false)
		{
			float Squared = UKismetMathLibrary::VSize2DSquared(static_cast<FVector2d>(GetStartLocationUnderCursor() - GetLocationUnderCursorCamera()));
			if(Squared > GetStartDragInstance()* GetStartDragInstance())
				GridSystemComponent->SetbDragStarted(true);
		}
	}
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
			GridSystemComponent->SetbPlaceableObjectSelected(true);
			GetSelectedPlaceableObject()->SetObjectSelectedMode(true);
			// 클릭시 Outline, Material Color 변경
			SetOutlineColor(1);
			if(IsValid(GetPCI()))
				GetPCI()->SetScalarParameterValue(FName("EnableShading"), 0.4f);
		}
		// 2. Placeable Object 아래에 커서가 존재하지 않는 경우
		else
		{
			GridSystemComponent->SetbPlaceableObjectSelected(false); // 선택된 상태가 아닌 것으로 판단
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
	if(GridSystemComponent->GetbPlaceableObjectSelected() && IsValid(GetSelectedPlaceableObject()))
	{
		GetSelectedPlaceableObject()->SetObjectSelectedMode(false);
		GridSystemComponent->SetbPlaceableObjectSelected(false);
		if(GetPlaceableObjectUnderCursor() == GetSelectedPlaceableObject())
		{
			GetSelectedPlaceableObject()->SwapObjectHighlighting(true);
		}
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
		GridSystemComponent->GetActivePlacer()->RotateObjectPlacer(true);
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
			// 0 : 녹색, 1: 노랑, 2: 빨강
			int32 ColorIncludedRed = FMath::RandRange(0, 2);
			int32 ColorExcludedRed = FMath::RandRange(0, 1);
			bool isRColor = FMath::RandBool();
			int32 BackGroundColor = isRColor ? ColorIncludedRed : ColorExcludedRed;
			int32 SelectedArr[8][8] = { 0 };

			// 패턴의 색상 조합
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

			// 패턴의 색상 조합
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
					// Add Instanced Static Mesh
					int32 InstanceIndex = InstancedStaticMeshComponent->AddInstance(TileTransform, true);
					
					if(SelectedArr[x][y] == 0)
					{
						// 녹색
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
						
						// 노란색
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
						
						// 빨간색
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
			GridSystemComponent->GetActivePlacer()->UpdateMeshMatDependingAmountOfResources(IsEnoughResource);	
		}
		GridSystemComponent->SetbBuildObjecEnabled(true);
		// Set Placer Object Mesh's World Location
		FVector2D CenterIndicators = GridSystemComponent->GetCenterOfRectangularArea(GetCellUnderCursor(), GridSystemComponent->GetActivePlacer()->GetObjectSize());
		bool bSuccess = false;
		FVector CellLocation = GridSystemComponent->GetCellLocation(GetCellUnderCursor(), GetLocationUnderCursorCamera(), bSuccess);
		float BaseBuildLocationLOCAL = CellLocation.Z;
		GridSystemComponent->GetActivePlacer()->ObjectMesh->SetWorldLocation(FVector(CenterIndicators.X, CenterIndicators.Y, BaseBuildLocationLOCAL));
		TArray<FIntPoint> CellsforBuild = GridSystemComponent->GetCellsinRectangularArea(GetCellUnderCursor(), GridSystemComponent->GetActivePlacer()->GetObjectSize());
		int SuccessCount = 0;
		for (int32 Index = 0; Index != CellsforBuild.Num(); Index++)
		{
			bool bSuccessforIndicators = false;
			FVector CellLocationLOCAL = GridSystemComponent->GetCellLocation(CellsforBuild[Index], GetLocationUnderCursorCamera(),bSuccessforIndicators);
			GridSystemComponent->GetActivePlacer()->PlaceIndicators[Index]->SetWorldLocation(CellLocationLOCAL);
			
			if(bSuccessforIndicators && CheckTileMapData(CellsforBuild[Index]) )
			{
				SuccessCount++;
				// 해당 공간에 각 Indicator Cell들을 놓을 수 있는지
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
			else // Ground 충돌 감지 못한 경우
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
			FConstructionCost SelectedObjectData = GetSelectedPlaceableObject()->GetObjectData()->ConstructionCost;
			int32 Percent =  GetSelectedPlaceableObject()->GetObjectData()->ReturnResourcesPercent;
			FConstructionCost ReturnResources = FConstructionCost(SelectedObjectData.Gold * Percent / 100, FFoodData(SelectedObjectData.Food.Rice * Percent / 100,
				SelectedObjectData.Food.Fruit * Percent / 100,SelectedObjectData.Food.Meat * Percent / 100),
				SelectedObjectData.Wood * Percent / 100, SelectedObjectData.Rock * Percent / 100, SelectedObjectData.Iron * Percent / 100,SelectedObjectData.Coal * Percent / 100);
			UpdateResourcesValue(ReturnResources, true, false);
			for(FIntPoint cells : GetSelectedPlaceableObject()->OccupiedCells)
			{
				ChangeOccupancyData(cells, false);
			}
			GetSelectedPlaceableObject()->DemolitionPlaceableObject();
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

	if(IsValid(GridSystemComponent->GetActivePlacer()))
	{
		bool IsEnoughResource = CheckifEnoughResources(GridSystemComponent->GetActivePlacer()->GetObjectData()->ConstructionCost);
		GridSystemComponent->GetActivePlacer()->UpdateMeshMatDependingAmountOfResources(IsEnoughResource);	
	}
	
	
}





/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   SetOutlineColor
  
  @Summary:  Local Function for set Material Collection Parameter to set outline color
  
  @Args:     int32 ObjectSide
             PlaceableObject's Outline Color determining demolition
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void ABuildManager::SetOutlineColor(int32 ObjectSide)
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
  @Method:   LCheckifEnoughResources

  @Summary:  Local Function for return Check Resource
  
  @Args:     FConstructionCost InCost
			 The Cost need for Construction
  
  @Returns:  bool
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
bool ABuildManager::CheckifEnoughResources(FConstructionCost InCost)
{
	return GetPlayerResources().Gold - InCost.Gold >= 0 && GetPlayerResources().Food.Fruit - InCost.Food.Fruit >=0 && GetPlayerResources().Food.Rice - InCost.Food.Rice >=0
		&& GetPlayerResources().Food.Meat - InCost.Food.Meat >=0 && GetPlayerResources().Coal - InCost.Coal >= 0 && GetPlayerResources().Iron - InCost.Iron >= 0
		&& GetPlayerResources().Rock - InCost.Rock >=0 && GetPlayerResources().Wood - InCost.Wood >=0;
}






