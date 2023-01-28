// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"

#include "NetworkedGameInstance.h"
#include "Components/Button.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->SetInputMode(FInputModeGameOnly());
	PlayerController->SetShowMouseCursor(true);
	
	FindGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenFindGame);
	HostGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenHostGame);
	QuitGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::QuitGame);

	if(UNetworkedGameInstance* GameInstance = Cast<UNetworkedGameInstance>(GetGameInstance()))
	{
		GameInstance->DestroyLastSession();
	}
}

void UMainMenuWidget::OpenFindGame()
{
	UE_LOG(LogTemp, Warning, TEXT("OpenFindGame()"));
	if (FindSessionWidgetClass)
	{
		FindSessionWidgetInstance = CreateWidget<UFindSessionWidget>(this, FindSessionWidgetClass);
		FindSessionWidgetInstance->AddToViewport();
	}
}

void UMainMenuWidget::OpenHostGame()
{
	UE_LOG(LogTemp, Warning, TEXT("OpenHostGame()"));
	if (CreateSessionWidgetClass)
	{
		CreateSessionWidgetInstance = CreateWidget<UCreateSessionWidget>(this, CreateSessionWidgetClass);
		CreateSessionWidgetInstance->AddToViewport();
	}
}

void UMainMenuWidget::QuitGame()
{
	UE_LOG(LogTemp, Warning, TEXT("QuitGame()"));
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->ConsoleCommand("quit");
}

void UMainMenuWidget::OnSessionDestroyed(bool bSuccessful)
{
	if (bSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Successfully Destroyed ongoing Session."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to Destroy any ongoing Session."));
	}
}
