// Fill out your copyright notice in the Description page of Project Settings.


#include "RocketManPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "WSADCharacterMovementComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"

ARocketManPlayerController::ARocketManPlayerController()
{
}

void ARocketManPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// if (PlayerCameraManager)
	// {
	// 	PlayerCameraManager->ViewPitchMax = 359.998993f;
	// 	PlayerCameraManager->ViewYawMin = 0.0f;
	// 	PlayerCameraManager->ViewYawMax = 359.998993f;
	// 	PlayerCameraManager->ViewRollMin = 0.0f;
	// 	PlayerCameraManager->ViewRollMax = 359.998993f;
	// }
	
	bPauseMenuEnabled = !UGameplayStatics::GetCurrentLevelName(this, true).Equals(DefaultMainMenuLevelName, ESearchCase::IgnoreCase);

	if (ACharacter* CurrentCharacter = GetCharacter())
	{
		RocketCharacterOwner = Cast<AwsadphysicsCharacter>(CurrentCharacter);
	}
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void ARocketManPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// if (IsLocalController())
	// {
	// 	const FQuat CurrentRotation = GetControlRotation().Quaternion();
	// 	const FQuat DesiredRotation = CurrentRotation * CalculateRotationDelta(DeltaSeconds);
	// 	SetControlRotation(DesiredRotation.Rotator());
	// }	
}

void ARocketManPlayerController::OpenPauseMenu()
{
	if (bPauseMenuEnabled)
	{
		UE_LOG(LogTemp, Warning, TEXT("OpenPauseMenu()"));
		if (PauseMenuWidgetClass)
		{
			PauseMenuWidget = CreateWidget<UPauseMenuWidget>(this, PauseMenuWidgetClass);
			PauseMenuWidget->AddToViewport();
		}
	}
}

void ARocketManPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	InputComponent->BindKey(EKeys::E, IE_Released, this, &ARocketManPlayerController::OpenPauseMenu);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {
		
		// Thruster
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ARocketManPlayerController::StartThruster);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ARocketManPlayerController::StopThruster);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARocketManPlayerController::Rotate);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ARocketManPlayerController::Rotate);
	}
}

void ARocketManPlayerController::StartThruster()
{
	Thrust(1.0f);
}

void ARocketManPlayerController::StopThruster()
{
	Thrust(.0f);
}

FQuat ARocketManPlayerController::CalculateRotationDelta(float DeltaTime)
{
	// Add Rotation
	constexpr double rotationStrength = 100;
	const double RollRotation = Safe_vInputRotationVector.X * rotationStrength * DeltaTime;
	const double PitchRotation = Safe_vInputRotationVector.Y * rotationStrength * DeltaTime * -1.f;

	FQuat RotationDelta = FRotator(PitchRotation, 0, RollRotation).Quaternion();
	RotationDelta.Normalize();
	return RotationDelta;
}

void ARocketManPlayerController::Thrust(const float ThrustValue)
{
	if (UWSADCharacterMovementComponent* CharacterMovement = Cast<UWSADCharacterMovementComponent>(RocketCharacterOwner->GetCharacterMovement()))
	{
		// CharacterMovement->SetThruster(ThrustValue);
		CharacterMovement->AddInputVector(RocketCharacterOwner->GetActorUpVector() * 1.f, false);
	}
}

void ARocketManPlayerController::Rotate(const FInputActionValue& Value)
{
	// input is a Vector2D
	Safe_vInputRotationVector = Value.Get<FVector2D>();
	// const FQuat CalculatedRotationQuat = CalculateRotationDelta(0.1f) * RootComponent->GetComponentQuat();
	// const FRotator TargetRotation = FRotator(CalculatedRotationQuat);
	// SetControlRotation(TargetRotation);
	AddRollInput(Safe_vInputRotationVector.X);
	AddPitchInput(Safe_vInputRotationVector.Y);
	UE_LOG(LogTemp, Warning, TEXT("Updated Rotation"));
}

void ARocketManPlayerController::AddRollInput(float Val)
{
	// Super::AddRollInput(Val);
	const FQuat RotInput = RotationInput.Quaternion();
	const FQuat RotationDelta = FRotator(0, 0, Val).Quaternion();
	const FQuat RotationResult = RotationDelta * RotInput;
	RotationInput = RotationResult.Rotator();
}

void ARocketManPlayerController::AddPitchInput(float Val)
{
	// Super::AddPitchInput(Val);
	const FQuat RotInput = RotationInput.Quaternion();
	const FQuat RotationDelta = FRotator(Val, 0, 0).Quaternion();
	const FQuat RotationResult = RotationDelta * RotInput;
	RotationInput = RotationResult.Rotator();
}

void ARocketManPlayerController::AddYawInput(float Val)
{
	// Super::AddYawInput(Val);
	const FQuat RotInput = RotationInput.Quaternion();
	const FQuat RotationDelta = FRotator(0, Val, 0).Quaternion();
	const FQuat RotationResult = RotationDelta * RotInput;
	RotationInput = RotationResult.Rotator();
}
