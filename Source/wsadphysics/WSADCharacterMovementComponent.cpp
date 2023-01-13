// Fill out your copyright notice in the Description page of Project Settings.


#include "WSADCharacterMovementComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/PhysicsVolume.h"

bool UWSADCharacterMovementComponent::FSavedMove_WSAD::CanCombineWith(const FSavedMovePtr& NewMove,
	ACharacter* InCharacter, float MaxDelta) const
{
	FSavedMove_WSAD* NewWSADMove = static_cast<FSavedMove_WSAD*>(NewMove.Get());

	const bool NewMoveMatchCurrent =
		Saved_vRotation != NewWSADMove->Saved_vRotation ||
		Saved_fThrust != NewWSADMove->Saved_fThrust;
	if (!NewMoveMatchCurrent)
	{
		return false;
	}
	
	return Super::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void UWSADCharacterMovementComponent::FSavedMove_WSAD::Clear()
{
	Super::Clear();
	Saved_vRotation = FVector2d::Zero();
	Saved_fThrust = 0;
}

uint8 UWSADCharacterMovementComponent::FSavedMove_WSAD::GetCompressedFlags() const
{
	return Super::GetCompressedFlags();
}

void UWSADCharacterMovementComponent::FSavedMove_WSAD::SetMoveFor(ACharacter* C, float InDeltaTime,
	FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	Super::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

	const UWSADCharacterMovementComponent* CharacterMovement = Cast<UWSADCharacterMovementComponent>(C->GetCharacterMovement());
	if(CharacterMovement)
	{
		Saved_vRotation = CharacterMovement->Safe_vRotation;
		Saved_fThrust = CharacterMovement->Safe_fThrust;
	}
}

void UWSADCharacterMovementComponent::FSavedMove_WSAD::PrepMoveFor(ACharacter* C)
{
	Super::PrepMoveFor(C);

	UWSADCharacterMovementComponent* CharacterMovement = Cast<UWSADCharacterMovementComponent>(C->GetCharacterMovement());
	if(CharacterMovement)
	{
		CharacterMovement->Safe_vRotation = Saved_vRotation;
		CharacterMovement->Safe_fThrust = Saved_fThrust;
	}
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
	check(PawnOwner != NULL);
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

void UWSADCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation,
	const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);
	if (!CharacterOwner)
	{
		return;
	}
	
	//Send movement vector to server
	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerSetThrust(Safe_fThrust);
		ServerSetRotation(Safe_vRotation);
	}
}

bool UWSADCharacterMovementComponent::ServerSetThrust_Validate(const float Thrust)
{
	return true;
}

void UWSADCharacterMovementComponent::ServerSetThrust_Implementation(const float Thrust)
{
	Safe_fThrust = Thrust;
}

bool UWSADCharacterMovementComponent::ServerSetRotation_Validate(const FVector2D& Rotation)
{
	return true;
}

void UWSADCharacterMovementComponent::ServerSetRotation_Implementation(const FVector2D& Rotation)
{
	Safe_vRotation = Rotation;
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

float UWSADCharacterMovementComponent::GetThrustForce()
{
	// const float Thrust = Safe_fThrust * FMaxThrust;
	const float Thrust = Safe_fThrust * .6f * FMaxThrust;
	return Thrust;
}

FVector UWSADCharacterMovementComponent::NewVelocityAfterThrust(const FVector& InitialVelocity, const float ThrustForce, float DeltaTime)
{
	FVector Result = InitialVelocity;

	if (DeltaTime > 0.f)
	{
		// Apply thrust.
		Result += UpdatedComponent->GetUpVector() * DeltaTime * ThrustForce;
	}
	return Result;
}

void UWSADCharacterMovementComponent::PhysMove(float deltaTime, int32 Iterations)
{
	if (deltaTime < MIN_TICK_TIME)
	{
		return;
	}

	RestorePreAdditiveRootMotionVelocity();
	
	float timeTick = GetSimulationTimeStep(deltaTime, Iterations);
	float ThrustTime = timeTick;

	// Add Rotation
	UE::Math::TRotator rotator = UpdatedComponent->GetComponentQuat().Rotator();
	double rotationStrength = 100;
	double PitchRotation = FMath::ClampAngle(Safe_vRotation.Y * rotationStrength, -85.0f, 85.0f);
	PitchRotation = PitchRotation * deltaTime;

	double RollRotation = Safe_vRotation.X * rotationStrength * deltaTime;
	rotator.Add(PitchRotation, 0, RollRotation);
	FQuat NewRotation = rotator.Quaternion();

	// Add Thrust Velocity To Component
	Velocity = NewVelocityAfterThrust(Velocity, GetThrustForce(), ThrustTime);

	// Time Acceleration Gravity
	FVector FallAcceleration = GetFallingLateralAcceleration(deltaTime);
	FallAcceleration.Z = 0.f;
	
	// Compute current gravity
	const FVector Gravity(0.f, 0.f, GetGravityZ());
	float GravityTime = timeTick;

	// Apply gravity
	Velocity = NewFallVelocity(Velocity, Gravity, GravityTime);

	
	// Calc Velocity
	if (!HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	{
		const float Friction = 0.5f * UpdatedComponent->GetPhysicsVolume()->FluidFriction;
		CalcVelocity(deltaTime, Friction, true, GetMaxBrakingDeceleration());
		// CalcVelocity(deltaTime, Move_Friction, true, GetMaxBrakingDeceleration());
	}
	ApplyRootMotionToVelocity(deltaTime);

	// Perform Move
	Iterations++;
	bJustTeleported = false;
	FVector OldLocation = UpdatedComponent->GetComponentLocation();
	const FVector Adjusted = Velocity * deltaTime;
	FHitResult Hit(1.f);
	SafeMoveUpdatedComponent(Adjusted, NewRotation, true, Hit);

	if (Hit.Time < 1.f)
	{
		const FVector GravDir = FVector(0.f, 0.f, -1.f);
		const FVector VelDir = Velocity.GetSafeNormal();
		const float UpDown = GravDir | VelDir;

		bool bSteppedUp = false;
		if ((FMath::Abs(Hit.ImpactNormal.Z) < 0.2f) && (UpDown < 0.5f) && (UpDown > -0.2f) && CanStepUp(Hit))
		{
			float stepZ = UpdatedComponent->GetComponentLocation().Z;
			bSteppedUp = StepUp(GravDir, Adjusted * (1.f - Hit.Time), Hit);
			if (bSteppedUp)
			{
				OldLocation.Z = UpdatedComponent->GetComponentLocation().Z + (OldLocation.Z - stepZ);
			}
		}

		if (!bSteppedUp)
		{
			//adjust and try again
			HandleImpact(Hit, deltaTime, Adjusted);
			SlideAlongSurface(Adjusted, (1.f - Hit.Time), Hit.Normal, Hit, true);
		}
	}

	// Update Outgoing Velocity & Acceleration
	if (!bJustTeleported && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	{
		Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / deltaTime; // v = dx / dt
	}
}

void UWSADCharacterMovementComponent::SetThruster(const float Thrust)
{
	Safe_fThrust = Thrust;
}

void UWSADCharacterMovementComponent::SetRotation(const FVector2D& Rotation)
{
	Safe_vRotation = Rotation;
}

UWSADCharacterMovementComponent::UWSADCharacterMovementComponent()
{
	Safe_vRotation = FVector2d::Zero();
	Safe_fThrust = 0.f;
}
