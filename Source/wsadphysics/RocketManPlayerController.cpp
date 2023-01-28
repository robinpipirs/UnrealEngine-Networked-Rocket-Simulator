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
	if (UWSADCharacterMovementComponent* CharacterMovement = Cast<UWSADCharacterMovementComponent>(RocketCharacterOwner->GetCharacterMovement()))
	{
		CharacterMovement->SetThruster(ThrustValue);
	}
}

void ARocketManPlayerController::Rotate(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (UWSADCharacterMovementComponent* CharacterMovement = Cast<UWSADCharacterMovementComponent>(RocketCharacterOwner->GetCharacterMovement()))
	{
		CharacterMovement->SetRotation(MovementVector);
	}
}
