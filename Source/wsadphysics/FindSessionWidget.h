// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "SessionListEntry.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "FindSessionWidget.generated.h"

/**
 * 
 */
UCLASS()
class WSADPHYSICS_API UFindSessionWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnRefreshButtonClicked();

	UFUNCTION()
	void OnJoinButtonClicked();

	UFUNCTION()
	void OnCancelButtonClicked();
	
	UFUNCTION()
	void OnSelectionChanged(UObject* Object);
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UButton* RefreshButton;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UButton* JoinButton;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UButton* CancelButton;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UListView* SessionsListView;

	// Focused
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* SelectedServerNameText;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* SelectedOwnerText;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* SelectedPingText;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* SelectedNumberOfConnectionsText;

private:
	void OnSearchForSessionsComplete(TArray<FOnlineSessionSearchResult> SearchResults, bool Successful);

	UFUNCTION()
	void Refresh();

	UFUNCTION()
	void UpdateUI();
	
	UFUNCTION()
	void UpdateFocusedItem();
	
	UPROPERTY()
	bool bRefreshing = false;

	UPROPERTY()
	TArray<USessionInfo*> AvailableSessions;

	UPROPERTY()
	UObject* CurrentSelection;
};