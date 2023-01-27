// Fill out your copyright notice in the Description page of Project Settings.


#include "FindSessionWidget.h"

#include "NetworkedGameInstance.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"


void UFindSessionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	RefreshButton->OnClicked.AddDynamic(this, &UFindSessionWidget::OnRefreshButtonClicked);
	JoinButton->OnClicked.AddDynamic(this, &UFindSessionWidget::OnJoinButtonClicked);
	CancelButton->OnClicked.AddDynamic(this, &UFindSessionWidget::OnCancelButtonClicked);
	JoinButton->SetIsEnabled(false);

	SessionsListView->OnItemSelectionChanged().AddUObject(this, &UFindSessionWidget::OnSelectionChanged);
	// USessionInfo SessionInfo = USessionInfo("Name Of Session", "Pirate guy", "Treasure Map", 123, 1);
	USessionInfo* SessionInfo = NewObject<USessionInfo>();
	SessionInfo->SessionName = "Walla";
	SessionInfo->SessionIdString = "asd";
	SessionInfo->OwningUserName = "Brur";
	AvailableSessions.Add(SessionInfo);

	SessionInfo = NewObject<USessionInfo>();
	SessionInfo->SessionName = "Brur";
	SessionInfo->SessionIdString = "asd";
	SessionInfo->OwningUserName = "Walla";
	AvailableSessions.Add(SessionInfo);
	UpdateUI();
}

void UFindSessionWidget::OnRefreshButtonClicked()
{
	Refresh();
}

void UFindSessionWidget::OnJoinButtonClicked()
{
	if (USessionInfo* SessionInfo = Cast<USessionInfo>(CurrentSelection))
	{
		UE_LOG(LogTemp, Warning, TEXT("Will Try to Join Selected: %s, %s"), *SessionInfo->SessionName, *SessionInfo->OwningUserName);

		if(UNetworkedGameInstance* GameInstance = Cast<UNetworkedGameInstance>(GetGameInstance()))
		{
			GameInstance->JoinServer(SessionInfo->SessionName);
		}
	}
}

void UFindSessionWidget::OnCancelButtonClicked()
{
	RemoveFromParent();
}

void UFindSessionWidget::OnSelectionChanged(UObject* Object)
{
	if (USessionInfo* SessionInfo = Cast<USessionInfo>(Object))
	{
		UE_LOG(LogTemp, Warning, TEXT("Item Selected: %s, %s"), *SessionInfo->SessionIdString, *SessionInfo->OwningUserName);
	}

	// Deselect Previous
	if (USessionListEntry* Previous = Cast<USessionListEntry>(SessionsListView->GetEntryWidgetFromItem(CurrentSelection)))
	{
		UE_LOG(LogTemp, Warning, TEXT("Update background color for previous"));
		FLinearColor Color = Previous->Background->GetBrushColor();
		Color.A = .2f;
		Previous->Background->SetBrushColor(Color);
	}

	if (USessionListEntry* Current = Cast<USessionListEntry>(SessionsListView->GetEntryWidgetFromItem(Object)))
	{
		UE_LOG(LogTemp, Warning, TEXT("Update background color for current"));
		FLinearColor Color = Current->Background->GetBrushColor();
		Color.A = 1.0f;
		Current->Background->SetBrushColor(Color);
	}
	CurrentSelection = Object;
	UpdateFocusedItem();
}

void UFindSessionWidget::UpdateUI()
{
	UE_LOG(LogTemp, Warning, TEXT("UpdateUI()"));
	SessionsListView->SetListItems(AvailableSessions);
}

void UFindSessionWidget::UpdateFocusedItem()
{
	UE_LOG(LogTemp, Warning, TEXT("UpdateFocusedItem()"));
	if (const USessionInfo* SessionInfo = Cast<USessionInfo>(CurrentSelection))
	{
		UE_LOG(LogTemp, Warning, TEXT("Item Selected: %s, %s, %s"), *SessionInfo->SessionName, *SessionInfo->SessionIdString, *SessionInfo->OwningUserName);
		SelectedServerNameText->SetText(FText::FromString(SessionInfo->SessionName));
		SelectedOwnerText->SetText(FText::FromString(SessionInfo->OwningUserName));
		SelectedPingText->SetText(FText::FromString(FString::Printf(TEXT("%d ms"),SessionInfo->PingInMs)));
		SelectedNumberOfConnectionsText->SetText(FText::FromString(FString::Printf(TEXT("%d/10"),SessionInfo->NumberOfConnectedPlayers)));
		JoinButton->SetIsEnabled(true);
	}
}

void UFindSessionWidget::OnSearchForSessionsComplete(const TArray<FOnlineSessionSearchResult> SearchResults, bool Successful)
{
	// missing removal of event handler
	// GameInstance->OnSearchForSessionsCompleteEvent.Remove();
	UE_LOG(LogTemp, Warning, TEXT("OnSearchForSessionsComplete()"));
	AvailableSessions.Reset();
	if (Successful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Sessions Found: %d"), SearchResults.Num());
		for (FOnlineSessionSearchResult Result : SearchResults)
		{
			USessionInfo* SessionInfo = NewObject<USessionInfo>();
			Result.Session.SessionSettings.Get("CUSTOM_SESSION_NAME", SessionInfo->SessionName);
			SessionInfo->SessionIdString = Result.Session.GetSessionIdStr();
			SessionInfo->OwningUserName = Result.Session.OwningUserName;
			SessionInfo->PingInMs = Result.PingInMs;
			SessionInfo->NumberOfConnectedPlayers = Result.Session.NumOpenPublicConnections;
			AvailableSessions.Add(SessionInfo);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Sessions Found"));
	}

	if(UNetworkedGameInstance* GameInstance = Cast<UNetworkedGameInstance>(GetGameInstance()))
	{
		GameInstance->OnSearchForSessionsCompleteEvent.Clear();
	}
	bRefreshing = false;
	RefreshButton->SetIsEnabled(true);
	UpdateUI();
}

void UFindSessionWidget::Refresh()
{
	UE_LOG(LogTemp, Warning, TEXT("Refresh()"));
	if (!bRefreshing)
	{
		UE_LOG(LogTemp, Warning, TEXT("Refreshing"));
		bRefreshing = true;
		RefreshButton->SetIsEnabled(false);
		JoinButton->SetIsEnabled(false);
		if(UNetworkedGameInstance* GameInstance = Cast<UNetworkedGameInstance>(GetGameInstance()))
		{
			GameInstance->OnSearchForSessionsCompleteEvent.AddUObject(this, &UFindSessionWidget::OnSearchForSessionsComplete);
			GameInstance->FindServers();
		}
	}

}


