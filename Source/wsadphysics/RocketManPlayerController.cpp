// Fill out your copyright notice in the Description page of Project Settings.


#include "RocketManPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "WSADCharacterMovementComponent.h"

ARocketManPlayerController::ARocketManPlayerController()
{
}

void ARocketManPlayerController::BeginPlay()
{
	Super::BeginPlay();
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

FQuat ARocketManPlayerController::CalculateRotationDelta(float DeltaTime)
{
	// Add Rotation
	constexpr double rotationStrength = 20;
	const double RollRotation = Safe_vInputRotationVector.X * rotationStrength * DeltaTime;
	const double PitchRotation = Safe_vInputRotationVector.Y * rotationStrength * DeltaTime * -1.f;

	FQuat RotationDelta = FRotator(PitchRotation, 0, RollRotation).Quaternion();
	RotationDelta.Normalize();
	return RotationDelta;
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

void ARocketManPlayerController::Thrust(const float ThrustValue)
{
	// if (UWSADCharacterMovementComponent* CharacterMovement = Cast<UWSADCharacterMovementComponent>(RocketCharacterOwner->GetCharacterMovement()))
	if (UCharacterMovementComponent* CharacterMovement = RocketCharacterOwner->GetCharacterMovement())
	{
		// CharacterMovement->SetThruster(ThrustValue);
		UE_LOG(LogTemp, Warning, TEXT("Add thrust"));
		CharacterMovement->AddInputVector(RocketCharacterOwner->GetActorUpVector() * 2000.f, true);
		// CharacterMovement->AddForce(RocketCharacterOwner->GetActorUpVector() * 2000.f);
	}
}

void ARocketManPlayerController::Rotate(const FInputActionValue& Value)
{
	// input is a Vector2D
	Safe_vInputRotationVector = Value.Get<FVector2D>();
	FQuat DeltaRotation = CalculateRotationDelta(.2f);

	// if (UWSADCharacterMovementComponent* CharacterMovement = Cast<UWSADCharacterMovementComponent>(RocketCharacterOwner->GetCharacterMovement()))
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Updated Rotation"));
	// 	CharacterMovement->SetDeltaRotation(DeltaRotation);
	// }

	const FQuat NewRotationQuat = DeltaRotation * RootComponent->GetComponentQuat();
	// RootComponent->SetWorldRotation(Rotation);
	const FRotator NewRotation = NewRotationQuat.Rotator();
	// SetControlRotation(NewRotation);
	AddRollInput(Safe_vInputRotationVector.X * 2.f);
	AddPitchInput(Safe_vInputRotationVector.Y * 2.f);
}
