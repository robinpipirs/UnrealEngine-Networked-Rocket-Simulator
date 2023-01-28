// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PauseMenuWidget.h"
#include "wsadphysicsCharacter.h"
#include "RocketManPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class WSADPHYSICS_API ARocketManPlayerController : public APlayerController
{
	GENERATED_BODY()

	ARocketManPlayerController();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPauseMenuWidget> PauseMenuWidgetClass;

	UPROPERTY()
	UPauseMenuWidget* PauseMenuWidget;


	UFUNCTION()
	void OpenPauseMenu();
	
public:
	virtual void SetupInputComponent() override;
	
private:
	UPROPERTY() FString DefaultMainMenuLevelName = "MainMenuMap";
	
	UPROPERTY() bool bPauseMenuEnabled;

	UPROPERTY(Transient) AwsadphysicsCharacter* RocketCharacterOwner;
};
