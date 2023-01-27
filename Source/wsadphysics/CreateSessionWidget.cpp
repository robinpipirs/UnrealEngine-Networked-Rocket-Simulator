// Fill out your copyright notice in the Description page of Project Settings.


#include "CreateSessionWidget.h"

#include "NetworkedGameInstance.h"
#include "OnlineSubsystemUtils.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/EditableTextBox.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetMathLibrary.h"

void UCreateSessionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	LanCheckbox->SetIsChecked(DefaultIsLan);
	ServerNameTextBox->SetText(FText::FromString(DefaultServerName));
	NumberOfPlayersSlider->SetValue(DefaultNumberOfPlayers);
	NumberOfPlayersSlider->OnValueChanged.AddDynamic(this, &UCreateSessionWidget::OnNumberOfPlayersChanged);
	NumberOfPlayersText->SetText(FText::FromString(FString::Printf(TEXT("%d"),DefaultNumberOfPlayers)));
	
	CreateButton->OnClicked.AddDynamic(this, &UCreateSessionWidget::OnCreateButtonClicked);
	CancelButton->OnClicked.AddDynamic(this, &UCreateSessionWidget::OnCancelButtonClicked);
}

void UCreateSessionWidget::OnNumberOfPlayersChanged(float Value)
{
	const int NumberOfPlayers = UKismetMathLibrary::Round(Value);
	NumberOfPlayersText->SetText(FText::FromString(FString::Printf(TEXT("%d"),NumberOfPlayers)));
}

void UCreateSessionWidget::LogToScreen(float TimeToDisplay, FString StringToDisplay)
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Yellow, StringToDisplay);
	}
}

void UCreateSessionWidget::OnCreateButtonClicked()
{
	const int NumberOfPlayers = UKismetMathLibrary::Round(NumberOfPlayersSlider->GetValue());
	const FName ServerName = FName(ServerNameTextBox->GetText().ToString());
	if(UNetworkedGameInstance* GameInstance = Cast<UNetworkedGameInstance>(GetGameInstance()))
	{
		GameInstance->CreateServer(ServerName, NumberOfPlayers);
	}
}

void UCreateSessionWidget::OnCancelButtonClicked()
{
	RemoveFromParent();
}


