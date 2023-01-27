// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "SessionListEntry.generated.h"

UCLASS()
class WSADPHYSICS_API USessionInfo : public UObject
{
	GENERATED_BODY()

public:
	USessionInfo();
	// USessionInfo(const char* Id, const char* Owner, const char* Map, int Ping, int Players);
	UPROPERTY(meta=(BindWidget))
	FString SessionName;
	
	UPROPERTY(meta=(BindWidget))
	FString SessionIdString;

	UPROPERTY(meta=(BindWidget))
	FString OwningUserName;

	UPROPERTY(meta=(BindWidget))
	int PingInMs;

	UPROPERTY(meta=(BindWidget))
	int NumberOfConnectedPlayers;
};


/**
 * 
 */
UCLASS()
class WSADPHYSICS_API USessionListEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* SessionNameText;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* OwnerNameText;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* PingText;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* NumberOfConnectionsText;

	UPROPERTY(meta=(BindWidget))
	class UBorder* Background;
};
