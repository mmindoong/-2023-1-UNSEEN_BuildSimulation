// Fill out your copyright notice in the Description page of Project Settings.


#include "Placer.h"

#include "Build/BuildManager.h"
#include "Game/BSGameSingleton.h"
#include "Kismet/GameplayStatics.h"

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   APlacerObjectBase
  
  @Category: Constructor
  
  @Summary:  Sets default values
  
  @Modifies: [PlaceableObjectTable, ObjectNameInTable]
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
APlacer::APlacer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Create ObjectMesh StaticMesh Component
	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjectMesh"));
	USceneComponent* DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	SetRootComponent(DefaultRoot);
	ObjectMesh->SetupAttachment(DefaultRoot);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Plane"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset(TEXT("/Game/GridAssets/Materials/Building_Indicators/MI_Building_Accepted"));
	if (MeshAsset.Succeeded())
	{
		UE_LOG(LogTemp, Log, TEXT("[PlacerObjectBase] ObjectMesh Asset Load"));
		ObjectMesh->SetStaticMesh(MeshAsset.Object);
		//ObjectMesh->SetCollisionProfileName(FName("NoCollision"));
		if(MaterialAsset.Succeeded())
		{
			ObjectMesh->SetMaterial(0, MaterialAsset.Object);
		}
		
	}
	
	// Material Asset Defualt Value
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> AcceptedMaterial(TEXT("/Game/GridAssets/Materials/Building_Indicators/MI_Building_Accepted"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> RejectMaterial(TEXT("/Game/GridAssets/Materials/Building_Indicators/MI_Building_Rejected"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> PlacingAcceptedMaterial(TEXT("/Game/GridAssets/Materials/Place_Indicators/MI_Place_Accepted"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> PlacingRejectMaterial(TEXT("/Game/GridAssets/Materials/Place_Indicators/MI_Place_Rejected"));
	if (AcceptedMaterial.Succeeded() && RejectMaterial.Succeeded() && PlacingAcceptedMaterial.Succeeded() && PlacingRejectMaterial.Succeeded())
	{
		UE_LOG(LogTemp, Log, TEXT("[PlacerObjectBase] PlacerMaterial Asset Load"));
		BuildingAcceptedMaterial = AcceptedMaterial.Object;
		BuildingRejectedMaterial = RejectMaterial.Object;
		PlaceAcceptedMaterial = PlacingAcceptedMaterial.Object;
		PlaceRejectedMaterial = PlacingRejectMaterial.Object;
	}

	// Placer Indicator Mesh Asset Load
	static ConstructorHelpers::FObjectFinder<UStaticMesh> IndicatorMeshAsset(TEXT("/Game/GridAssets/Meshes/SM_Plane_100x100"));
	if (IndicatorMeshAsset.Succeeded())
	{
		UE_LOG(LogTemp, Log, TEXT("[PlacerObjectBase] IndicatorMesh Asset Load"));
		IndicatorMesh = IndicatorMeshAsset.Object;
	}
	

	
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   BeginPlay
  
  @Summary:  Called when the game starts or when spawned
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void APlacer::BeginPlay()
{
	Super::BeginPlay();

	// Binding UpdateResourceevent, If there are not enough resources to build, the building material will turn red
	
}


/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   SetupObjectPlacer
  
  @Category: Parent Functions
  
  @Summary:  Setup Object Placer's ObjectData
  
  @Modifies: [ObjectData, PlaceableObjectClass, ObjectSize]
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void APlacer::SetupObjectPlacer()
{
	FName LocalRowName = GetRowName();
	FObjectData* OutRow = UBSGameSingleton::Get().GetPlaceableObjectDataTable()->FindRow<FObjectData>(LocalRowName, "");
	
	if (OutRow != nullptr)
	{
		// Set OutRow -> PlaceableObjectClass, ObjectSize
		SetObjectData(OutRow);
		SetPlaceableObjectClass(GetObjectData()->PlacaebleObjectClass);
		SetObjectSize(FIntPoint(FMath::Clamp<int32>(GetObjectData()->ObjectSize.X, 1, GetObjectData()->ObjectSize.X),
			FMath::Clamp<int32>(GetObjectData()->ObjectSize.Y, 1, GetObjectData()->ObjectSize.Y)));
		ObjectMesh->SetStaticMesh(ObjectData->Mesh.Get());
		SetResourceType(GetObjectData()->ResourceType);
		
		for(int i =0; i < ObjectData->Mesh->GetNumSections(0); i++)
		{
			ObjectMesh->SetMaterial( i, BuildingAcceptedMaterial);
		}
		ObjectMesh->SetCollisionProfileName(FName("NoCollision"));
		
	}
	
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   ActivateObjectPlacer
  
  @Category: Parent Functions
  
  @Summary:  Set Visibility Activate Placer's ObjectMesh
  
  @Modifies: [ObjectMesh]
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void APlacer::ActivateObjectPlacer()
{
	SetActorTickEnabled(true);
	ObjectMesh->SetVisibility(true, true);
}

/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   DeactivateObjectPlacer
  
  @Category: Parent Functions
  
  @Summary:  Destroy Actor
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void APlacer::DeactivateObjectPlacer()
{
	K2_DestroyActor();
}


/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   RotateObjectPlacer
  
  @Category: Parent Functions
  
  @Summary:  Rotate Object Placer's ObjectMesh
  
  @Args:     bool bLeft
  
  @Modifies: [ObjectMesh, BuildDirection]
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void APlacer::Rotate(bool bLeft)
{
	SetObjectSize(FIntPoint(GetObjectSize().Y, GetObjectSize().X));
	float DeltaRotationYaw = bLeft ? -90.0f : 90.0f;
	ObjectMesh->AddRelativeRotation(FRotator(0.0f, DeltaRotationYaw, 0.0f));
	int32 Direction = bLeft ? -1 : 1;
	int32 Direction2 = (Direction + GetBuildDirection()) < 0 ? 3 : Direction + GetBuildDirection();
	int32 Direction3 = (Direction + GetBuildDirection()) > 3 ? 0 : Direction2;
	SetBuildDirection(Direction3);
}


/*M+M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M+++M
  @Method:   CreateIndicatorMesh
  
  @Category: Parent Functions
  
  @Summary:  Create, setup and return new mesh 
  
  @Args:     TArray<UStaticMeshComponent*> PlaceIndicators, bool bPlaceEnabled
  
  @Returns:  UStaticMeshComponent*
M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M---M-M*/
void APlacer::CreateIndicatorMesh(bool bPlaceEnabled)
{
	UMaterialInterface* MeshMaterial = bPlaceEnabled ? PlaceAcceptedMaterial : PlaceRejectedMaterial;
	UStaticMeshComponent* NewComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass()); 
	NewComponent->RegisterComponent();
	
	if(NewComponent->IsRegistered())
	{
		NewComponent->SetStaticMesh(IndicatorMesh);
		NewComponent->SetMaterial(0, MeshMaterial);
		NewComponent->SetVisibility(true);
		PlaceIndicators.Add(NewComponent);
	}
	
}

void APlacer::UpdateMeshMaterial(bool bIsEnoughResource)
{
	if(bIsEnoughResource)
	{
		for(int i =0; i < ObjectData->Mesh->GetNumSections(0); i++)
		{
			ObjectMesh->SetMaterial( i, BuildingAcceptedMaterial);
		}
		
	}
	else
	{
		for(int i =0; i < ObjectData->Mesh->GetNumSections(0); i++)
		{
			ObjectMesh->SetMaterial( i, BuildingRejectedMaterial);
		}
	}
}


