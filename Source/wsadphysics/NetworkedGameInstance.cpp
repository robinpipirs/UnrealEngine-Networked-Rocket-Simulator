// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkedGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "OnlineSubsystemUtils.h"

UNetworkedGameInstance::UNetworkedGameInstance()
{
}

void UNetworkedGameInstance::Init()
{
	Super::Init();
	if (IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
	{
		SetupSessionInterface(Subsystem);
	}
}

void UNetworkedGameInstance::SetupSessionInterface(IOnlineSubsystem* Subsystem)
{
	SessionInterface = Subsystem->GetSessionInterface();
	if (SessionInterface.IsValid())
	{
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UNetworkedGameInstance::OnCreateSessionComplete);
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UNetworkedGameInstance::OnFindSessionComplete);
		SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UNetworkedGameInstance::OnJoinSessionComplete);
		SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UNetworkedGameInstance::OnSessionDestroyed);
	}
}

void UNetworkedGameInstance::OnCreateSessionComplete(FName SessionName, bool Succeeded)
{
	if (Succeeded)
	{
		GetWorld()->ServerTravel("/Game/ThirdPerson/Maps/ThirdPersonMap?listen");
	}
}

void UNetworkedGameInstance::OnFindSessionComplete(bool Succeeded)
{
	if (Succeeded)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnFindSessionComplete()"));
		const TArray<FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults;
		OnSearchForSessionsCompleteEvent.Broadcast(SearchResults, Succeeded);
	}
	else
	{
		OnSearchForSessionsCompleteEvent.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
	}	
}

void UNetworkedGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FString JoinAddress = "";
		SessionInterface->GetResolvedConnectString(SessionName, JoinAddress);
		if (JoinAddress != "")
		{
			PlayerController->ClientTravel(JoinAddress, TRAVEL_Absolute);
		}
	}
}

void UNetworkedGameInstance::OnSessionDestroyed(FName Name, bool Succeeded)
{
	if (Succeeded)
	{
		UE_LOG(LogTemp, Warning, TEXT("Session Was Destroyed"));
		LastSessionName = "";
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to Destroy Session"));
	}
}

void UNetworkedGameInstance::DestroyLastSession()
{
	if (LastSessionName != "")
	{
		UE_LOG(LogTemp, Warning, TEXT("Destroying Last Session W Name: %s"), *LastSessionName.ToString());
		SessionInterface->DestroySession(LastSessionName);
	}
}

void UNetworkedGameInstance::CreateServer(FName SessionName, int NumberOfConnections)
{
	UE_LOG(LogTemp, Warning, TEXT("CreateServer"));
	LastSessionName = SessionName;
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;
	const bool IsLanMatch = (IOnlineSubsystem::Get()->GetSubsystemName() != "NULL") ? false : true;
	UE_LOG(LogTemp, Warning, TEXT("Is Lan Match: %d"), IsLanMatch);
	SessionSettings.bIsLANMatch = IsLanMatch;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUseLobbiesIfAvailable = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.NumPublicConnections = 5;
	
	SessionSettings.Set("CUSTOM_SESSION_NAME", SessionName.GetPlainNameString(), EOnlineDataAdvertisementType::ViaOnlineService);

	SessionInterface->CreateSession(0, SessionName, SessionSettings);
}

void UNetworkedGameInstance::FindServers()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	const bool IsLanQuery = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL");
	UE_LOG(LogTemp, Warning, TEXT("Is Lan Query: %d"), IsLanQuery);
	SessionSearch->bIsLanQuery = IsLanQuery;
	SessionSearch->MaxSearchResults = 10000;
	SessionSearch->TimeoutInSeconds = 10.f;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UNetworkedGameInstance::JoinServer(FString SessionName)
{
	bool SessionFound = false;
	int SessionIdIndex = 0;
	TArray<FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults;
	for (FOnlineSessionSearchResult SearchResult : SearchResults)
	{
		FString SessionNameString;
		SearchResult.Session.SessionSettings.Get("CUSTOM_SESSION_NAME",SessionNameString);
		UE_LOG(LogTemp, Warning, TEXT("Session in results: %s"), *SessionNameString);
		if (SessionNameString.Equals(SessionName, ESearchCase::IgnoreCase))
		{
			SessionFound = true;
			break;
		}
		SessionIdIndex++;
	}
	
	if (SessionFound)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found Session Joining %s, %d"), *SessionName, SessionIdIndex);
		SessionInterface->JoinSession(0, FName(SessionName), SearchResults[SessionIdIndex]);
	}
}
