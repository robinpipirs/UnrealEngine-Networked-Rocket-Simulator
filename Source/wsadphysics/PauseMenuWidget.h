// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class WSADPHYSICS_API UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void ResumeGame();
	UFUNCTION()
	void QuitToMain();

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UButton* ResumeGameButton;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UButton* OptionsMenuButton;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UButton* QuitToMainMenuButton;

public:

private:
	UPROPERTY()
	FName DefaultMainMenuMapName = "MainMenuMap";
};
