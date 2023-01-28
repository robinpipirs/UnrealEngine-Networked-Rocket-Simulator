// Fill out your copyright notice in the Description page of Project Settings.


#include "RocketManPlayerController.h"

#include "Kismet/GameplayStatics.h"

ARocketManPlayerController::ARocketManPlayerController()
{
}

void ARocketManPlayerController::BeginPlay()
{
	Super::BeginPlay();
	bPauseMenuEnabled = !UGameplayStatics::GetCurrentLevelName(this, true).Equals(DefaultMainMenuLevelName, ESearchCase::IgnoreCase);

}

void ARocketManPlayerController::OpenPauseMenu()
{
}

void ARocketManPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindKey(EKeys::E, IE_Released, this, &ARocketManPlayerController::OpenPauseMenu);
}
