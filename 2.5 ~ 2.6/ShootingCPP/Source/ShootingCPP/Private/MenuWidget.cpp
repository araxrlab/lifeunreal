// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// �� ��ư �Է� �̺�Ʈ�� �����ϴ� �Լ��� �����Ѵ�.
	button_Restart->OnClicked.AddDynamic(this, &UMenuWidget::Restart);
	button_Quit->OnClicked.AddDynamic(this, &UMenuWidget::Quit);
}

void UMenuWidget::Restart()
{
	// ������ �ٽ� �ε��Ѵ�.
	UGameplayStatics::OpenLevel(GetWorld(), "ShootingMap");
}

void UMenuWidget::Quit()
{
	// ���� ���� ���� ���� ������Ʈ�� �����´�.
	UWorld* currentWorld = GetWorld();

	// ���� �����Ų��.
	UKismetSystemLibrary::QuitGame(currentWorld, currentWorld->GetFirstPlayerController(), EQuitPreference::Quit, false);
}