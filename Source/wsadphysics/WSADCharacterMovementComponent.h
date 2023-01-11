// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "wsadphysicsCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WSADCharacterMovementComponent.generated.h"

UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_None			UMETA(Hidden),
	CMOVE_Phys			UMETA(DisplayName = "Phys"),
	CMOVE_PhysWRotate	UMETA(DisplayName = "PhysWRotate"),
	CMOVE_MAX			UMETA(Hidden),
};

/**
 * 
 */
UCLASS()
class WSADPHYSICS_API UWSADCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	class FSavedMove_WSAD : public FSavedMove_Character
	{
	public:

		FVector2D Saved_vRotation = FVector2D::Zero();
		float Saved_fThrust = 0.f;
		typedef FSavedMove_Character Super;

		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
		virtual void Clear() override;
		virtual uint8 GetCompressedFlags() const override;
		virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData) override;
		virtual void PrepMoveFor(ACharacter* C) override;
	};

	class FNetworkPredictionData_Client_WSAD : public FNetworkPredictionData_Client_Character
	{
	public:
		FNetworkPredictionData_Client_WSAD(const UCharacterMovementComponent& ClientMovement);
		typedef FNetworkPredictionData_Client_Character Super;
		virtual FSavedMovePtr AllocateNewMove() override;
	};

	UPROPERTY(EditDefaultsOnly) float Move_TickVelocityImpulse = 100;
	UPROPERTY(EditDefaultsOnly) float Move_Friction = 1.3;

	UPROPERTY(Transient) AwsadphysicsCharacter* WsadCharacterOwner;

	FVector2D Safe_vRotation;
	float Safe_fThrust;

	const float FMaxThrust = 2000.f;

	UFUNCTION(Unreliable, Server, WithValidation)
	void ServerSetThrust(const float Thrust);

	UFUNCTION(Unreliable, Server, WithValidation)
	void ServerSetRotation(const FVector2D& Rotation);

public:
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;

protected:
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
	void PhysCustom(float deltaTime, int32 Iterations);
	float GetThrustForce();
	FVector NewVelocityAfterThrust(const FVector& InitialVelocity, const float ThrustForce, float DeltaTime);
	void PhysMove(float deltaTime, int32 Iterations);

public:
	UWSADCharacterMovementComponent();

public:
	UFUNCTION(BlueprintCallable) void SetThruster(float Thrust);
	UFUNCTION(BlueprintCallable) void SetRotation(const FVector2D& Rotation);
};
