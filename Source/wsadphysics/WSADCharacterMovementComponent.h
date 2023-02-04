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

		FVector Saved_vThrust;
		FQuat Saved_qNewRotation;
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

	FVector Safe_vThrust;
	FQuat Safe_qNewRotation;

	float Safe_fInputThrust;

public:
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;

protected:
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	FVector CalculateThrustDelta(float DeltaTime);
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;
	void PhysMove(float deltaTime, int32 Iterations);
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UWSADCharacterMovementComponent();

public:
	UFUNCTION(BlueprintCallable) void SetThruster(float Thrust);
};
