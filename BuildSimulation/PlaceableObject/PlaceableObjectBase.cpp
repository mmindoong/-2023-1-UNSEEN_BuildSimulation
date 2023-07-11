// Fill out your copyright notice in the Description page of Project Settings.

#include "PlaceableObjectBase.h"

#include "Data/PlaceableObjectsData.h"
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

	SetupPlaceableObject();
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

  @Modifies: [ObjectData, ObjectSize, HP, MaxHP, bOutlineEnabled,
              bHPBarEnabled, StartingHealthPercent, ObjectDirection,
              OccupiedCenterCell, ObjectHeight].
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void APlaceableObjectBase::SetupPlaceableObject()
{
	FName LocalRowName = GetRowName();
	FPlaceableObjectData* OutRow = UBSGameSingleton::Get().GetPlaceableObjectDataTable()->FindRow<FPlaceableObjectData>(LocalRowName, "");

	if (OutRow != nullptr)
	{
		// Set OutRow -> MaxHP, HP, BorderEnbaled, OutlineEnabled, HPBarEnabled
		ObjectMesh->SetStaticMesh(OutRow->ProceedingMesh);
		SetObjectData(OutRow);
		SetObjectSize(GetObjectData()->ObjectSize);
		SetMaxHP(GetObjectData()->HealthPoints);
		SetHP(GetMaxHP() * GetStartingHealthPercent());
		SetbOutlineEnabled(GetObjectData()->EnableOutline);
		SetbHPBarEnabled(GetObjectData()->EnableHpBar);
		SetStartingHealthPercent(FMath::Clamp<float>(GetStartingHealthPercent(), 0.0f, 100.0f));
		// If the object was placed on the map in the editor, not during the game, the dynamic data will not be set and the object itself will occupy the necessary cells

		if (GetObjectDynamicData().HasData)
		{
			// Set Object -> Object Direction, Object Size, Build Manager, Occupied Center Cell, Occupied Cells, Object Height
			SetObjectDirection(GetObjectDynamicData().Direction);
			FIntPoint ReturnValue = GetObjectDirection() == 0 || GetObjectDirection() == 2 ? FIntPoint(GetObjectSize().X, GetObjectSize().Y) : FIntPoint(GetObjectSize().Y, GetObjectSize().X);
			SetObjectSize(ReturnValue);
			// Set Occupied Center Cell
			SetOccupiedCenterCell(GetObjectDynamicData().ObjectCenterCell);
			// Set Object Height
			SetObjectHeight(GetObjectDynamicData().Height);
			// todo : Update Resources Value
		}
		else
		{
			// Set Object -> Object Direction, Object Size
			int32 b = ((FVector2D)(GetActorForwardVector())).Dot(FVector2D(0.0f, 1.0f)) >= 0.45 ? 1 : 3;
			int32 b2 = ((FVector2D)(GetActorForwardVector())).Dot(FVector2D(1.0f, 0.0f)) <= -0.45 ? 2 : b;
			int32 Direction = ((FVector2D)(GetActorForwardVector())).Dot(FVector2D(1.0f, 0.0f)) >= 0.45 ? 0 : b2;
			SetObjectDirection(Direction);
			FIntPoint ReturnValue = GetObjectDirection() == 0 || GetObjectDirection() == 2 ? FIntPoint(GetObjectSize().X, GetObjectSize().Y) : FIntPoint(GetObjectSize().Y, GetObjectSize().X);
			SetObjectSize(ReturnValue);
		}
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
	if (GetbOutlineEnabled())
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
		//todo : HP Bar UnVisible
	}
	else if(GetbObjectSelected() == true)
	{
		SwapObjectHighlighting(true);
		//todo : HP Bar Visible
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
		// Outline Setting
		if(GetbOutlineEnabled())
			LSwapObjectOutline(true);
		// HP Bar Setting
		if(GetbHPBarEnabled()) {} // todo : HP Bar UI Visible
	}
	else
	{
		// Outline Setting
		if(GetbOutlineEnabled())
			LSwapObjectOutline(false);
		// HP Bar Setting
		if(GetbHPBarEnabled() && GetbObjectSelected() == false) {} // todo : HP Bar UI UnVisible
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
	ObjectMesh->SetStaticMesh(GetObjectData()->Mesh);
	
	return true;
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
			if (GetObjectSide() == 0)
				GetMeshesforoutline()[i]->SetCustomDepthStencilValue(1);
			else
				GetMeshesforoutline()[i]->SetCustomDepthStencilValue(1);
		}
		else
		{
			GetMeshesforoutline()[i]->SetCustomDepthStencilValue(0);
		}
			
	}
}
