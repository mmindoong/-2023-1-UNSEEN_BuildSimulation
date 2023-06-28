// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/BSPawn.h"

#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABSPawn::ABSPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_OBJECTDATA(TEXT("/Game/Blueprints/Data/DT_ObjectData"));
	if(DT_OBJECTDATA.Succeeded())
	{
		PlaceableObjectTable = DT_OBJECTDATA.Object;
		check(PlaceableObjectTable->GetRowMap().Num() > 0);
	}
	
	//Create components
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), FRotator(-60.0f, 0.0f, 0.0f));
	SpringArmComponent->TargetArmLength = 400.0f;
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->CameraLagSpeed = 3.0f;
	SpringArmComponent->bDoCollisionTest = false;

	FollowCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	FollowCameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	FollowCameraComponent->SetRelativeLocation(FVector(-3000.0f, 0.0f, 0.0f));
	
	if(UGameplayStatics::GetActorOfClass(GetWorld(), ABuildManager::StaticClass()))
	{
		BuildManager = Cast<ABuildManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ABuildManager::StaticClass()));	
	}

	static ConstructorHelpers::FClassFinder<AAIController> ControllerClassRef(TEXT("/Script/BuildSimulation.AIUnitController"));
	if(ControllerClassRef.Class)
	{
		AIControllerClass = ControllerClassRef.Class;
	}

}

// Called when the game starts or when spawned
void ABSPawn::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<ABSPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	check(PlayerController);
}

// Called every frame
void ABSPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Right Rotation Input
	if(PlayerController->WasInputKeyJustPressed(EKeys::Q))
	{
		GateRightRotation.Open();
	}
	else if(PlayerController->WasInputKeyJustReleased(EKeys::Q))
	{
		GateRightRotation.Close();
	}
	// Left Rotation Input
	if(PlayerController->WasInputKeyJustPressed(EKeys::E))
	{
		GateLeftRotation.Open();
	}
	else if(PlayerController->WasInputKeyJustReleased(EKeys::E))
	{
		GateLeftRotation.Close();
	}
	// Miiddle Mouse Angle Rotation Input
	if(PlayerController->WasInputKeyJustPressed(EKeys::MiddleMouseButton))
	{
		GateAngleRotation.Open();
	}
	else if(PlayerController->WasInputKeyJustReleased(EKeys::MiddleMouseButton))
	{
		GateAngleRotation.Close();
	}

	// ForwardMovement Input
	if(PlayerController->WasInputKeyJustPressed(EKeys::W))
	{
		GateForwardMovement.Open();
	}
	else if(PlayerController->WasInputKeyJustReleased(EKeys::W))
	{
		GateForwardMovement.Close();
	}
	// BackMovement Input
	if(PlayerController->WasInputKeyJustPressed(EKeys::S))
	{
		GateBackMovement.Open();
	}
	else if(PlayerController->WasInputKeyJustReleased(EKeys::S))
	{
		GateBackMovement.Close();
	}
	// LeftMovement Input
	if(PlayerController->WasInputKeyJustPressed(EKeys::A))
	{
		GateLeftMovement.Open();
	}
	else if(PlayerController->WasInputKeyJustReleased(EKeys::A))
	{
		GateLeftMovement.Close();
	}
	// RightMovement Input
	if(PlayerController->WasInputKeyJustPressed(EKeys::D))
	{
		GateRightMovement.Open();
	}
	else if(PlayerController->WasInputKeyJustReleased(EKeys::D))
	{
		GateRightMovement.Close();
	}

	// Gate Execute
	if(GateRightRotation.IsOpen())
	{
		SpringArmComponent->AddRelativeRotation(FRotator(0.0f, GetRotationSpeed(), 0.0f));
	}
	if(GateLeftRotation.IsOpen())
	{
		SpringArmComponent->AddRelativeRotation(FRotator(0.0f, -1.0f * GetRotationSpeed(), 0.0f));
	}
	if(GateAngleRotation.IsOpen())
	{
		float DeltaX = 0;
		float DeltaY = 0;
		PlayerController->GetInputMouseDelta(DeltaX, DeltaY);
		
		float NewPitch = FMath::ClampAngle(DeltaY * 4.0f + SpringArmComponent->GetRelativeRotation().Pitch, -85.0f, 85.0f);
		float NewYaw = SpringArmComponent->GetRelativeRotation().Yaw + DeltaX * 4.0f;
		float NewRoll = SpringArmComponent->GetRelativeRotation().Roll;
		SpringArmComponent->SetRelativeRotation(FRotator(NewPitch, NewYaw, NewRoll));
		
	}
	if(GateForwardMovement.IsOpen())
	{
		FVector DeltaLocation = GetCameraMoveSpeed() * UKismetMathLibrary::Vector_Normal2D(SpringArmComponent->GetForwardVector());
		SpringArmComponent->AddWorldOffset(DeltaLocation);
	}
	if(GateBackMovement.IsOpen())
	{
		FVector DeltaLocation = -1.0f * GetCameraMoveSpeed() * UKismetMathLibrary::Vector_Normal2D(SpringArmComponent->GetForwardVector());
		SpringArmComponent->AddWorldOffset(DeltaLocation);
	}
	if(GateLeftMovement.IsOpen())
	{
		SpringArmComponent->AddWorldOffset(SpringArmComponent->GetRightVector() * GetCameraMoveSpeed() * -1.0f);
	}
	if(GateRightMovement.IsOpen())
	{
		SpringArmComponent->AddWorldOffset(SpringArmComponent->GetRightVector() * GetCameraMoveSpeed());
	}
	
	if(PlayerController->WasInputKeyJustReleased(EKeys::One))
	{
		FDataTableRowHandle NewOjectDataRow;
		NewOjectDataRow.DataTable = PlaceableObjectTable;
		NewOjectDataRow.RowName =  TEXT("House");
		BuildManager->GridSystemComponent->ActivateBuildingTool(NewOjectDataRow);
	}
	if(PlayerController->WasInputKeyJustReleased(EKeys::Zero))
	{
		BuildManager->GridSystemComponent->ActivateDemolitionTool();
	}
	if(PlayerController->WasInputKeyJustReleased(EKeys::Delete))
	{
		BuildManager->DestorySelectedPlaceableObject();
	}
}

// Called to bind functionality to input
void ABSPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MouseMiddle", this, &ABSPawn::MouseWheel);
	PlayerInputComponent->BindAction("LeftMouseButton", IE_Pressed, this, &ABSPawn::PressedLMB);
	PlayerInputComponent->BindAction("LeftMouseButton", IE_Released, this, &ABSPawn::ReleasedLMB);
}

void ABSPawn::MouseWheel(float AxisValue)
{
	if(IsValid(SpringArmComponent))
	{
		float LengthValue = SpringArmComponent->TargetArmLength + GetCameraZoomSpeed() * AxisValue * -1.0f;
		SpringArmComponent->TargetArmLength = FMath::Clamp(LengthValue, GetCameraMinZoom(), GetCameraMaxZoom());
	}
}

void ABSPawn::PressedLMB()
{
	BuildManager->PressedLMB();
}

void ABSPawn::ReleasedLMB()
{
	BuildManager->ReleasedLMB();
}

