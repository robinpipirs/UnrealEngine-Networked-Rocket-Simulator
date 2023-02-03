// Fill out your copyright notice in the Description page of Project Settings.


#include "SixDofCameraManager.h"

void ASixDofCameraManager::ProcessViewRotation(float DeltaTime, FRotator& OutViewRotation, FRotator& OutDeltaRot)
{
	// Super::ProcessViewRotation(DeltaTime, OutViewRotation, OutDeltaRot);

	const FQuat OutDeltaRotation = FQuat(OutDeltaRot);
	FQuat OutRotation = FQuat(OutViewRotation);

	OutRotation = OutDeltaRotation * OutRotation;
	OutViewRotation = OutRotation.Rotator();
	OutDeltaRot = FRotator::ZeroRotator;
}
