// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CreateSessionWidget.h"
#include "FindSessionWidget.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class WSADPHYSICS_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OpenFindGame();

	UFUNCTION()
	void OpenHostGame();
	
	UFUNCTION()
	void QuitGame();

	// Reset World Settings
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UButton* FindGameButton;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UButton* HostGameButton;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UButton* OptionsMenuButton;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UButton* QuitGameButton;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCreateSessionWidget> CreateSessionWidgetClass;
	UPROPERTY()
	UCreateSessionWidget* CreateSessionWidgetInstance;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UFindSessionWidget> FindSessionWidgetClass;
	UPROPERTY()
	UFindSessionWidget* FindSessionWidgetInstance;

public:
	UFUNCTION()
	void OnSessionDestroyed(bool bSuccessful);
};
