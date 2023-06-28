// Fill out your copyright notice in the Description page of Project Settings.


#include "GridSystemComponent.h"

#include "Kismet/KismetMathLibrary.h"

UGridSystemComponent::UGridSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UGridSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	
	
}



void UGridSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}




/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   GetCellLocation
  
  @Category: GridSystem
  
  @Summary:  Get Changed Cell Location's Location(Z is Zero)
  
  @Args:     FIntPoint InCell, bool& bSuccess
			 Cell Value, and bool that Line Trace Hit was occured
  
  @Returns:  FVector
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
FVector UGridSystemComponent::GetCellLocation(FIntPoint InCell, FVector InCameraLocation, bool& bSuccess)
{
	FVector2D CellCenter = GetCellCenterToLocation(InCell);
	FVector LocationLOCAL = FVector(CellCenter.X, CellCenter.Y, 0.0f);
	
	FHitResult HitResult;
	FVector StartLocation = FVector(LocationLOCAL.X, LocationLOCAL.Y, GetStartTraceHeight() + InCameraLocation.Z);
	FVector EndLocation =  FVector(LocationLOCAL.X, LocationLOCAL.Y, GetEndTraceHeight() + InCameraLocation.Z);
	TArray<AActor*> ActorsToIgnore;
	FCollisionQueryParams Params;

	/*
	bool IsHitResult = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECC_GameTraceChannel1,
		Params);
	*/
	bool IsHitResult = UKismetSystemLibrary::LineTraceSingle(
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
		0.5f);
		
	if(IsHitResult == true)
	{
		float RoundHeight = RoundHeightToGridStep(HitResult.Location.Z);
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
FIntPoint UGridSystemComponent::GetCellfromWorldLocation(FVector Location)
{
	int32 X = FMath::Abs(FMath::RoundToInt32(Location.X / GetGridTileSize().X)) * FMath::Sign(FMath::RoundToInt32(Location.X / GetGridTileSize().X));
	int32 Y = FMath::Abs(FMath::RoundToInt32(Location.Y / GetGridTileSize().Y)) * FMath::Sign(FMath::RoundToInt32(Location.Y / GetGridTileSize().Y));
	
	return FIntPoint(X,Y);
}



/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   CalculateCenterandBottomLeft
  
  @Summary:  Local Funtion for Calculate BottomLeftCorner using Center Location
  
  @Args:     float CurrentPosition, float SnapValue
  
  @Modifies: [GridCenterLocation, GridBottomLeftCornerLocation]
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void UGridSystemComponent::CalculateCenterandBottomLeft()
{
	FVector NewVector = SnapVectorToVector(GridCenterLocation, GridTileSize);
	FIntPoint AlignVector;

	AlignVector.X = GridTileCount.X % 2 == 0 ? 0 : 1;
	AlignVector.Y = GridTileCount.Y, 2 == 0 ? 0 : 1;

	FIntPoint Divided = (GridTileCount - AlignVector) / 2;
	FVector GridCount = FVector(Divided.X * GridTileSize.X, Divided.Y * GridTileSize.Y, 1.0f);
	
	SetGridCenterLocation(NewVector);
	SetGridBottomLeftCornerLocation(NewVector - GridCount);
}


/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   GetCenterOfRectangularArea

  @Summary:  Get Cells'Center by the size of PlaceableObject's Objectsize.
  
  @Args:     FIntPoint CenterLocation, FIntPoint TileCount
			 Object's OccupiedCenterCell, Object's count
  
  @Returns:  TArray<FIntPoint>
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
FVector2D UGridSystemComponent::GetCenterOfRectangularArea(FIntPoint AreaCenterCell, FIntPoint AreaSize)
{
	FVector2D LocationLOCAL = GetCellCenterToLocation(AreaCenterCell);
	
	float LocationCenterX = AreaSize.X % 2 == 0 ? LocationLOCAL.X - (GetGridTileSize().X * 0.5f) : LocationLOCAL.X;
	float LocationCenterY = AreaSize.Y % 2 == 0 ? LocationLOCAL.Y - (GetGridTileSize().Y * 0.5f) : LocationLOCAL.Y;
	
	return FVector2D(LocationCenterX, LocationCenterY);
}


/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   GetCellsinRectangularArea

  @Summary:  Get Cells by the size of PlaceableObject's Objectsize.
  
  @Args:     FIntPoint CenterLocation, FIntPoint TileCount
			 Object's OccupiedCenterCell, Object's count
  
  @Returns:  TArray<FIntPoint>
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
TArray<FIntPoint> UGridSystemComponent::GetCellsinRectangularArea(FIntPoint CenterLocation, FIntPoint TileCount)
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
  @Method:   ActivateBuildingTool

  @Category: Tool
  
  @Summary:  Spawn Placer Actor and change to new activate Placer
  
  @Args:     FDataTableRowHandle ObjectforBuilding
             the object's data row for change
  
  @Modifies: [bBuildToolEnabled, 
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void UGridSystemComponent::ActivateBuildingTool(FDataTableRowHandle ObjectforBuilding)
{
	DeactivateBuildingTool();
	ChangeObjectforPlacement(ObjectforBuilding);
	if(GetbObjectForPlacementIsSelected())
	{
		SetbBuildToolEnabled(true);
		if(IsValid(GetActivePlacer()))
		{
			GetActivePlacer()->DeactivateObjectPlacer();
		}
		FVector SpawnLocation = GetLocationUnderCursorCamera();
		FRotator rotator = FRotator(0.0f, 0.0f, 0.0f);
		FActorSpawnParameters SpawnParams;

		if(GetWorld())
		{
			FDataTableRowHandle NewPlacerDataRow;
			NewPlacerDataRow.DataTable = GetObjectForPlacement().DataTable;
			NewPlacerDataRow.RowName =  GetObjectForPlacement().RowName;
			
			SetActivePlacer(Cast<APlacerObjectBase>(GetWorld()->SpawnActor<AActor>(APlacerObjectBase::StaticClass(), SpawnLocation, rotator, SpawnParams)));
			GetActivePlacer()->SetObjectNameInTable(NewPlacerDataRow);
			GetActivePlacer()->SetupObjectPlacer();
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
void UGridSystemComponent::DeactivateBuildingTool()
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
void UGridSystemComponent::ActivateDemolitionTool()
{
	DeactivateBuildingTool();
	SetbDemolitionToolEnabled(true);
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   DeactivateDemolitionTool
  
  @Category: Tool
  
  @Summary:  Deactiavte Dmeolitoin tool
  
  @Modifies: [bDemolitionToolEnabled]
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void UGridSystemComponent::DeactivateDemolitionTool()
{
	if(GetbDemolitionToolEnabled()) //원래 활성화되어있었는지 확인
		{
		SetbDemolitionToolEnabled(false);
		//todo : 해당 함수 호출 후 CallUpdatePlaceableObjectUnderCursor(GetPlaceableObjectUnderCursor(), false); 
		}
}




/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   ChangeObjectforPlacement
  
  @Summary:  Local Function for Changing Object Data using new DT Row
  
  @Args:     FDataTableRowHandle NewObjectRow
  
  @Modifies: [ObjectForPlacement, PlaceableObjectData,
			 bObjectForPlacementIsSelected]
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void UGridSystemComponent::ChangeObjectforPlacement(FDataTableRowHandle NewObjectRow)
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
  @Method:   CancelDragObjectPlacing
  
  @Category: Main
  
  @Summary:  Detect Cancel Drag and Hide activated Placer Actor

  @Modifies: [bDragWasInterrupted]
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void UGridSystemComponent::CancelDragObjectPlacing()
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
  @Method:   SnapVectorToVector
  
  @Summary:  Local Funtion for Sanp Vector to Vector
  
  @Args:     FVector CurrentPosition, FVector SnapValue
  
  @Returns:  FVector
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
FVector UGridSystemComponent::SnapVectorToVector(FVector CurrentPosition, FVector SnapValue)
{
	FVector Return;
	Return.X = FMath::RoundToInt32(CurrentPosition.X / SnapValue.X)* SnapValue.X;
	Return.Y = FMath::RoundToInt32(CurrentPosition.Y / SnapValue.Y) * SnapValue.Y;
	Return.Z = FMath::RoundToInt32(CurrentPosition.Z / SnapValue.Z) * SnapValue.Z;
	
	return Return;
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   SnapFlaotToFloat
  
  @Summary:  Local Funtion for Sanp Float to Float
  
  @Args:     float CurrentPosition, float SnapValue
  
  @Returns:  float
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
float UGridSystemComponent::SnapFlaotToFloat(float CurrentLocation, float GridSize)
{
	float Return = FMath::RoundToInt32(CurrentLocation / GridSize) * GridSize;

	return Return;

}


/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   GetCellCenterToLocation
  
  @Summary:  Local Function for return grid cell Center to Location Vector
  
  @Args:     FIntPoint InCell
  
  @Returns:  FVector2D
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
FVector2D UGridSystemComponent::GetCellCenterToLocation(FIntPoint InCell)
{
	float LocationX = FMath::Abs(InCell.X) * GetGridTileSize().X * FMath::Sign(InCell.X);
	float LocationY = FMath::Abs(InCell.Y) * GetGridTileSize().Y * FMath::Sign(InCell.Y);
	
	return FVector2D(LocationX, LocationY);
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   RoundHeightToGridStep
  
  @Summary:  Local Function for return grid round height by tracing new height
  
  @Args:     float InNewHeight
  
  @Returns:  float
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
float UGridSystemComponent::RoundHeightToGridStep(float InNewHeight)
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


