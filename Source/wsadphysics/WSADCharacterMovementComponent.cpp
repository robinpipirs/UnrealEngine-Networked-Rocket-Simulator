// Fill out your copyright notice in the Description page of Project Settings.


#include "WSADCharacterMovementComponent.h"

#include "GameFramework/Character.h"


UWSADCharacterMovementComponent::FSavedMove_WSAD::FSavedMove_WSAD()
{
	Saved_fForwardComponent = 0.f;
	Saved_fRightComponent = 0.f;
}

bool UWSADCharacterMovementComponent::FSavedMove_WSAD::CanCombineWith(const FSavedMovePtr& NewMove,
	ACharacter* InCharacter, float MaxDelta) const
{
	FSavedMove_WSAD* NewWSADMove = static_cast<FSavedMove_WSAD*>(NewMove.Get());

	const bool NewMoveMatchCurrent = Saved_fRightComponent != NewWSADMove->Saved_fRightComponent || Saved_fForwardComponent != NewWSADMove->Saved_fForwardComponent;
	if (!NewMoveMatchCurrent)
	{
		return false;
	}
	
	return FSavedMove_Character::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void UWSADCharacterMovementComponent::FSavedMove_WSAD::Clear()
{
	FSavedMove_Character::Clear();
	Saved_fForwardComponent = 0;
	Saved_fRightComponent = 0;
}

uint8 UWSADCharacterMovementComponent::FSavedMove_WSAD::GetCompressedFlags() const
{
	return FSavedMove_Character::GetCompressedFlags();
}

void UWSADCharacterMovementComponent::FSavedMove_WSAD::SetMoveFor(ACharacter* C, float InDeltaTime,
	FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

	const UWSADCharacterMovementComponent* CharacterMovement = Cast<UWSADCharacterMovementComponent>(C->GetCharacterMovement());
	Saved_fForwardComponent = CharacterMovement->Safe_fForwardComponent;
	Saved_fRightComponent = CharacterMovement->Safe_fRightComponent;
}

void UWSADCharacterMovementComponent::FSavedMove_WSAD::PrepMoveFor(ACharacter* C)
{
	FSavedMove_Character::PrepMoveFor(C);

	UWSADCharacterMovementComponent* CharacterMovement = Cast<UWSADCharacterMovementComponent>(C->GetCharacterMovement());
	CharacterMovement->Safe_fForwardComponent = Saved_fForwardComponent;
	CharacterMovement->Safe_fRightComponent = Saved_fRightComponent;
}

UWSADCharacterMovementComponent::FNetworkPredictionData_Client_WSAD::FNetworkPredictionData_Client_WSAD(const UCharacterMovementComponent& ClientMovement)
: Super(ClientMovement)
{
}

FSavedMovePtr UWSADCharacterMovementComponent::FNetworkPredictionData_Client_WSAD::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_WSAD());
}

FNetworkPredictionData_Client* UWSADCharacterMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != nullptr)

if (ClientPredictionData == nullptr)
{
	UWSADCharacterMovementComponent* MutableThis = const_cast<UWSADCharacterMovementComponent*>(this);
	MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_WSAD(*this);
	MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
	MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
}
	return ClientPredictionData;
}

void UWSADCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);
}

void UWSADCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	Super::PhysCustom(deltaTime, Iterations);

	PhysMove(deltaTime, Iterations);
	// switch (CustomMovementMode)
	// {
	// case CMOVE_Slide:
	// 	break;
	// default:
	// 	UE_LOG(LogTemp, Fatal, TEXT("Invalid Movement Mode"));
	// }
}

void UWSADCharacterMovementComponent::PhysMove(float deltaTime, int32 Iterations)
{
	if (deltaTime < MIN_TICK_TIME)
	{
		return;
	}

	RestorePreAdditiveRootMotionVelocity();
	
	// Add Forward and Right Velocity to component
	Velocity += Velocity.GetSafeNormal2D() *  UpdatedComponent->GetForwardVector() * Move_TickVelocityImpulse * Safe_fForwardComponent;
	Velocity += Velocity.GetSafeNormal2D() *  UpdatedComponent->GetRightVector() * Move_TickVelocityImpulse * Safe_fRightComponent;

	// Calc Velocity
	if (!HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	{
		CalcVelocity(deltaTime, Move_Friction, true, GetMaxBrakingDeceleration());
	}
	ApplyRootMotionToVelocity(deltaTime);

	// Perform Move
	Iterations++;
	bJustTeleported = false;

	const FVector OldLocation = UpdatedComponent->GetComponentLocation();
	const FQuat OldRotation = UpdatedComponent->GetComponentRotation().Quaternion();
	FHitResult Hit(1.f);
	const FVector Adjusted = Velocity * deltaTime; // x = v * dt
	SafeMoveUpdatedComponent(Adjusted, OldRotation, true, Hit);

	if (Hit.Time < 1.f)
	{
		HandleImpact(Hit, deltaTime, Adjusted);
		SlideAlongSurface(Adjusted, (1.f - Hit.Time), Hit.Normal, Hit, true);
	}

	// Update Outgoing Velocity & Acceleration
	if (!bJustTeleported && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	{
		Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / deltaTime; // v = dx / dt
	}
}

void UWSADCharacterMovementComponent::SetForwardComponent(const float Forward)
{
	Safe_fForwardComponent = Forward;
}

void UWSADCharacterMovementComponent::SetRightComponent(const float Right)
{
	Safe_fRightComponent = Right;
}

UWSADCharacterMovementComponent::UWSADCharacterMovementComponent()
{
	Safe_fForwardComponent = 0.f;
	Safe_fRightComponent = 0.f;
}
