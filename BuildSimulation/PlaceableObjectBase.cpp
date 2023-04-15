// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaceableObjectBase.h"
#include "PlaceableObjectsData.h"
#include "BuildManager.h"


// Sets default values
APlaceableObjectBase::APlaceableObjectBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	FString ObjectDataPath = TEXT("/Game/Blueprints/Data/DT_PlaceableObjectData");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_OBJECTDATA(*ObjectDataPath);
	check(DT_OBJECTDATA.Succeeded());
	PlaceableObjectTable = DT_OBJECTDATA.Object;
	check(PlaceableObjectTable->GetRowMap().Num() > 0);
	
	SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	SphereVisual->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere"));
	if (SphereVisualAsset.Succeeded())
	{
		SphereVisual->SetStaticMesh(SphereVisualAsset.Object);
		SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
		SphereVisual->SetWorldScale3D(FVector(0.8f));
	}
	
	// Set DataTableRowHandle Defualt value
	FDataTableRowHandle InObjectNameInTable;
	InObjectNameInTable.DataTable = PlaceableObjectTable;
	InObjectNameInTable.RowName = FName("House");
	SetObjectNameInTable(InObjectNameInTable);

	SetupPlaceableObject();

}

// Called when the game starts or when spawned
void APlaceableObjectBase::BeginPlay()
{
	Super::BeginPlay();

	if (SphereVisual != nullptr)
	{
		SphereVisual->OnBeginCursorOver.AddDynamic(this, &APlaceableObjectBase::OnBeginCursorOver);
		UE_LOG(LogTemp, Warning, TEXT("AddDynamic"));
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("NULL"));
	
	
}


void APlaceableObjectBase::OnBeginCursorOver(UPrimitiveComponent* TouchedComponent)
{
	UE_LOG(LogTemp, Warning, TEXT("OnBeginCursorOver"));
}

void APlaceableObjectBase::CallUpdateResourceAmountEvent(FConstructionCost InCost)
{
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, FString::Printf(TEXT("CallDeleFunc_Single_OneParam %d"), InCost.Coal));
}

// Called every frame
void APlaceableObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlaceableObjectBase::SetupPlaceableObject()
{
	FName LocalRowName = GetObjectNameInTable().RowName;
	FPlaceableObjectData* OutRow = GetObjectNameInTable().DataTable->FindRow<FPlaceableObjectData>(LocalRowName, "");

	
	if (OutRow != nullptr)
	{
		// Set OutRow -> MaxHP, HP, BorderEnbaled, OutlineEnabled, HPBarEnabled
		SetObjectData(OutRow);
		SetObjectSize(GetObjectData()->ObjectSize);
		SetMaxHP(GetObjectData()->HealthPoints);
		SetHP(GetMaxHP() * GetStartingHealthPercent());
		SetOutlineEnabled(GetObjectData()->EnableOutline);
		SetHPBarEnabled(GetObjectData()->EnableHpBar);
		SetStartingHealthPercent(FMath::Clamp<float>(GetStartingHealthPercent(), 0.0f, 100.0f));
		// If the object was placed on the map in the editor, not during the game, the dynamic data will not be set and the object itself will occupy the necessary cells

		if (GetObjectDynamicData()->HasData)
		{
			// Set Object -> Object Direction, Object Size, Build Manager, Occupied Center Cell, Occupied Cells, Object Height
			SetObjectDirection(GetObjectDynamicData()->Direction);
			FIntPoint ReturnValue = GetObjectDirection() == 0 || GetObjectDirection() == 2 ? FIntPoint(GetObjectSize().X, GetObjectSize().Y) : FIntPoint(GetObjectSize().Y, GetObjectSize().X);
			SetObjectSize(ReturnValue);
			// Set Build Manager
			//SetBuildManager(GetObjectDynamicData()->BuildManager);
			// Set Occupied Center Cell
			SetOccupiedCenterCell(GetObjectDynamicData()->ObjectCenterCell);
			// Set Occupied Cells array
			//SetOccupiedCells(GetBuildManager()->GetCellsinRectangularArea(FVector(GetOccupiedCenterCell(),100.0f) , GetObjectSize()));
			// Set Object Height
			SetObjectHeight(GetObjectDynamicData()->Height);

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

void APlaceableObjectBase::SetupOutline()
{
	/*
	if (GetOutlineEnabled())
	{
		TArray<UActorComponent*> components = GetOwner()->GetComponentsByClass(UStaticMeshComponent::StaticClass());

		for (int32 i = 0; i < components.Num(); i++)
		{
			GetMeshesforoutline().Add(Cast<UStaticMeshComponent>(components[i]));
		}
		for (int32 j = 0; j < GetMeshesforoutline().Num(); j++)
		{
			GetMeshesforoutline()[j]->SetRenderCustomDepth(true);
		}
	}
	*/
}

void APlaceableObjectBase::EnableObjectOutline(bool IsEnable)
{
	for (int32 i= 0; i < GetMeshesforoutline().Num(); i++)
	{
		if (IsEnable)
		{
			if (GetObjectSide() == 0)
			{
				GetMeshesforoutline()[i]->SetCustomDepthStencilValue(1);
				
			}
			else
			{
				GetMeshesforoutline()[i]->SetCustomDepthStencilValue(1);
			}
		}
		else
		{
			GetMeshesforoutline()[i]->SetCustomDepthStencilValue(0);
		}
			
	}
}