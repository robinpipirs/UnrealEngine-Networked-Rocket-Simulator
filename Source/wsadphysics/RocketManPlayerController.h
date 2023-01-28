// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PauseMenuWidget.h"
#include "InputActionValue.h"
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

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;


	UFUNCTION()
	void OpenPauseMenu();

	UFUNCTION()
	void StartThruster();
	
	UFUNCTION()
	void StopThruster();
	
	UFUNCTION()
	void Thrust(const float ThrustValue);
	
	UFUNCTION()
	void Rotate(const FInputActionValue& Value);
	
public:
	virtual void SetupInputComponent() override;

private:
	UPROPERTY() FString DefaultMainMenuLevelName = "MainMenuMap";
	
	UPROPERTY() bool bPauseMenuEnabled;

	UPROPERTY(Transient) AwsadphysicsCharacter* RocketCharacterOwner;
};
