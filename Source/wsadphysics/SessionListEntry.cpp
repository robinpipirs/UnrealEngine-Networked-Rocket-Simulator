// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionListEntry.h"
#include "Components/TextBlock.h"

USessionInfo::USessionInfo()
{
}

// USessionInfo::USessionInfo(const char* Id, const char* Owner, const char* Map, int Ping, int Players)
// {
// 	
// }

void USessionListEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	const USessionInfo* SessionInfo = Cast<USessionInfo>(ListItemObject);
	SessionNameText->SetText(FText::FromString(SessionInfo->SessionName));
	OwnerNameText->SetText(FText::FromString(SessionInfo->OwningUserName));
	PingText->SetText(FText::FromString(FString::Printf(TEXT("%d ms"),SessionInfo->PingInMs)));
	NumberOfConnectionsText->SetText(FText::FromString(FString::Printf(TEXT("%d/10"),SessionInfo->NumberOfConnectedPlayers)));
}
