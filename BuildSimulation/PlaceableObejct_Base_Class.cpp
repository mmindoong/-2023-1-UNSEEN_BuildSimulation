// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaceableObejct_Base_Class.h"
#include "PlaceableObjectsData.h"
#include "GridActor.h"


// Sets default values
APlaceableObejct_Base_Class::APlaceableObejct_Base_Class()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	FString ObjectDataPath = TEXT("/Game/Blueprints/Data/DT_PlaceableObjectData");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_OBJECTDATA(*ObjectDataPath);
	check(DT_OBJECTDATA.Succeeded());
	PlaceableObjectTable = DT_OBJECTDATA.Object;
	check(PlaceableObjectTable->GetRowMap().Num() > 0);

	// Set DataTableRowHandle Defualt value
	FDataTableRowHandle InObjectNameInTable;
	InObjectNameInTable.DataTable = PlaceableObjectTable;
	InObjectNameInTable.RowName = FName("House");
	SetObjectNameInTable(InObjectNameInTable);

	SetupPlaceableObject();

}

// Called when the game starts or when spawned
void APlaceableObejct_Base_Class::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void APlaceableObejct_Base_Class::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlaceableObejct_Base_Class::SetupPlaceableObject()
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
		SetBorderEnabled(GetObjectData()->EnableBorder);
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
			SetBuildManager(GetObjectDynamicData()->BuildManager);
			// Set Occupied Center Cell
			SetOccupiedCenterCell(GetObjectDynamicData()->ObjectCenterCell);
			// Set Occupied Cells array
			SetOccupiedCells(GetBuildManager()->GetCellsinRectangularArea(GetOccupiedCenterCell(), GetObjectSize()));
			// Set Object Height
			SetObjectHeight(GetObjectDynamicData()->Height);

			// TODO : Update Resources Value
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

			
			// Set Build Manager
			SetBuildManager(Cast<AGridActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridActor::StaticClass())));
			if (IsValid(GetBuildManager()))
			{
				GetBuildManager()->SetGridCenterLocation(GetActorLocation());
				FIntPoint CenterCell = FIntPoint(GetBuildManager()->GetGridCenterLocation().X, GetBuildManager()->GetGridCenterLocation().Y);
				//Set Occupied Cells array
				SetOccupiedCells(GetBuildManager()->GetCellsinRectangularArea(CenterCell, GetObjectSize()));
			}
		}
		if (IsValid(GetBuildManager()))
		{
			// Build Manger에 점유된 셀 정보 세팅해주기
			for (FIntPoint cells : GetOccupiedCells())
			{
				GetBuildManager()->SetOccupancyData(cells, true);
				GetBuildManager()->SetObjectData(cells, this);
			}
		}
	}
	
}
