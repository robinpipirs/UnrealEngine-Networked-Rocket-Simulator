// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CreateSessionWidget.generated.h"

/**
 * 
 */
UCLASS()
class WSADPHYSICS_API UCreateSessionWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnNumberOfPlayersChanged(float Value);

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UEditableTextBox* ServerNameTextBox;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UCheckBox* LanCheckbox;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class USlider* NumberOfPlayersSlider;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* NumberOfPlayersText;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UButton* CancelButton;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UButton* CreateButton;
public:
	UFUNCTION()
	void OnCreateButtonClicked();
	UFUNCTION()
	void OnCancelButtonClicked();

private:
	UFUNCTION()
	void LogToScreen(float TimeToDisplay, FString StringToDisplay);
	int DefaultNumberOfPlayers = 2;
	bool DefaultIsLan = false;
	FString DefaultServerName = "TheBestServer";
};
