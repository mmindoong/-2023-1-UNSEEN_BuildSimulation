// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BSPlayerController.h"
#include "Build/BuildManager.h"
#include "GameFramework/Pawn.h"
#include "Interface/BSPawnHUDInterface.h"
#include "BSPawn.generated.h"

UCLASS()
class BUILDSIMULATION_API ABSPawn : public APawn, public IBSPawnHUDInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABSPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	ABSPlayerController* PlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Main, Meta = (AllowPrivateAccess = "true"))
	ABuildManager* BuildManager;

protected:
	//Camera Section
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, Meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, Meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCameraComponent;

	// Input Functions
	void MouseWheel(float AxisValue);
	void PressedLMB();
	void ReleasedLMB();


private:
	FGate GateRightRotation = FGate(true);
	FGate GateLeftRotation = FGate(true);
	FGate GateAngleRotation = FGate(true);
	FGate GateForwardMovement = FGate(true);
	FGate GateBackMovement = FGate(true);
	FGate GateRightMovement = FGate(true);
	FGate GateLeftMovement = FGate(true);

	class UDataTable* PlaceableObjectTable;

	//Setting Section
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setting, Meta = (AllowPrivateAccess = "true"))
	float RotationSpeed = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setting, Meta = (AllowPrivateAccess = "true"))
	float CameraMoveSpeed = 100.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setting, Meta = (AllowPrivateAccess = "true"))
	float CameraZoomSpeed = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setting, Meta = (AllowPrivateAccess = "true"))
	float CameraMinZoom = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Setting, Meta = (AllowPrivateAccess = "true"))
	float CameraMaxZoom = 40000.0f;

	// Getter & Setter
	FORCEINLINE float GetRotationSpeed() const { return RotationSpeed; }
	FORCEINLINE void SetRotationSpeed(const float InRotationSpeed) { RotationSpeed = InRotationSpeed; }

	FORCEINLINE float GetCameraMoveSpeed() const { return CameraMoveSpeed; }
	FORCEINLINE void SetCameraMoveSpeed(const float InCameraMoveSpeed) { CameraMoveSpeed = InCameraMoveSpeed; }

	FORCEINLINE float GetCameraZoomSpeed() const { return CameraZoomSpeed; }
	FORCEINLINE void SetCameraZoomSpeed(const float InCameraZoomSpeed) { CameraZoomSpeed = InCameraZoomSpeed; }

	FORCEINLINE float GetCameraMinZoom() const { return CameraMinZoom; }
	FORCEINLINE void SetCameraMinZoom(const float InCameraMinZoom) { CameraMinZoom = InCameraMinZoom; }

	FORCEINLINE float GetCameraMaxZoom() const { return CameraMaxZoom; }
	FORCEINLINE void SetInCameraMaxZoom(const float InCameraMaxZoom) { CameraMaxZoom = InCameraMaxZoom; }

	// UI Section
protected:
	virtual void SetupHUDWidget(UBSHUD* InHUDWidget) override;
};
