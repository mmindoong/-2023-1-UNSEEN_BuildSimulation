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
	FName LocalRowName = ObjectNameInTable.RowName;
	FPlaceableObjectData* OutRow = ObjectNameInTable.DataTable->FindRow<FPlaceableObjectData>(LocalRowName, "");

	
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
		// TODO : If the object was placed on the map in the editor, not during the game, the dynamic data will not be set and the object itself will occupy the necessary cells

		// Set Object -> Object Direction, Object Size
		int32 b = ((FVector2D)(GetActorForwardVector())).Dot(FVector2D(0.0f, 1.0f)) >= 0.45 ? 1 : 3;
		int32 b2 = ((FVector2D)(GetActorForwardVector())).Dot(FVector2D(1.0f, 0.0f)) <= -0.45 ? 2 : b;
		int32 Direction = ((FVector2D)(GetActorForwardVector())).Dot(FVector2D(1.0f, 0.0f)) >= 0.45 ? 0 : b2;
		SetObjectDirection(Direction);

		FIntPoint ReturnValue = GetObjectDirection() == 0 || GetObjectDirection() == 2 ? FIntPoint(GetObjectSize().X, GetObjectSize().Y) : FIntPoint(GetObjectSize().Y, GetObjectSize().X);
		SetObjectSize(ReturnValue);
		
		// Set Build Manager
		SetBuildManager(Cast<AGridActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridActor::StaticClass())));
		// TODO : Get Cells in Rectangular Area



	}
}
