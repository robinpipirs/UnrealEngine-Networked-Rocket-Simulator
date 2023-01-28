// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->SetInputMode(FInputModeGameAndUI());
	PlayerController->SetShowMouseCursor(true);

	ResumeGameButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::ResumeGame);
	QuitToMainMenuButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::QuitToMain);
}

void UPauseMenuWidget::ResumeGame()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->SetInputMode(FInputModeGameOnly());
	PlayerController->SetShowMouseCursor(false);
	RemoveFromParent();
}

void UPauseMenuWidget::QuitToMain()
{
	UE_LOG(LogTemp, Warning, TEXT("QuitToMain()"));
	UGameplayStatics::OpenLevel(this, DefaultMainMenuMapName, true);
}
