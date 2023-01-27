// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "NetworkedGameInstance.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSearchForSessionsComplete, TArray<FOnlineSessionSearchResult>, bool);

/**
 * 
 */
UCLASS()
class WSADPHYSICS_API UNetworkedGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UNetworkedGameInstance();

	FOnSearchForSessionsComplete OnSearchForSessionsCompleteEvent;

	UFUNCTION(BlueprintCallable)
	void CreateServer(FName SessionName, int NumberOfConnections);

	UFUNCTION(BlueprintCallable)
	void FindServers();
	
	UFUNCTION(BlueprintCallable)
	void DestroyLastSession();

	UFUNCTION(BlueprintCallable)
	void JoinServer(FString SessionName);

protected:
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	IOnlineSessionPtr SessionInterface;
	FName LastSessionName;

	void SetupSessionInterface(IOnlineSubsystem* Subsystem);
	virtual void Init() override;
	virtual void OnCreateSessionComplete(FName SessionName, bool Succeeded);
	virtual void OnFindSessionComplete(bool Succeeded);
	virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	virtual void OnSessionDestroyed(FName Name, bool Succeeded);
};
