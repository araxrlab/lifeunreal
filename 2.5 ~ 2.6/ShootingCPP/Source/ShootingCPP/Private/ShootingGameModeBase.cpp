// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingGameModeBase.h"
#include "MainWidget.h"
#include "Components/TextBlock.h"
#include "MenuWidget.h"
#include "Kismet/GameplayStatics.h"

void AShootingGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (mainWidget != nullptr)
	{
		// mainWidget �������Ʈ ������ �޸𸮿� �ε��Ѵ�.
		mainUI = CreateWidget<UMainWidget>(GetWorld(), mainWidget);

		// ������ �޸𸮿� �ε�Ǹ� �� ��Ʈ�� ����Ѵ�.
		if (mainUI != nullptr)
		{
			mainUI->AddToViewport();
		}
	}
}

void AShootingGameModeBase::PrintScore()
{
	if (mainUI != nullptr)
	{
		// scoreData �ؽ�Ʈ ��Ͽ� ���� ���� ���� �Է��Ѵ�.
		mainUI->scoreData->SetText(FText::AsNumber(currentScore));
	}
}

// ���� ������ ����ϴ� �Լ�
void AShootingGameModeBase::AddScore(int32 point)
{
	// �Ű����� point�� ���� �Ѱܹ��� ������ ���� ������ �����Ѵ�.
	currentScore += point;
	
	// ���� ������ ������ �ݿ��Ѵ�.
	PrintScore();
}

// �޴� ������ ����ϴ� �Լ�
void AShootingGameModeBase::ShowMenu()
{
	if (menuWidget != nullptr)
	{
		// �޴� ������ �����Ѵ�.
		menuUI = CreateWidget<UMenuWidget>(GetWorld(), menuWidget);

		if (menuUI != nullptr)
		{
			// ������ �޴� ������ �� ��Ʈ�� ����Ѵ�.
			menuUI->AddToViewport();

			// ������ �Ͻ� ���� ���·� �����.
			UGameplayStatics::SetGamePaused(GetWorld(), true);

			// �÷��̾� ��Ʈ�ѷ����� ���콺 Ŀ���� ȭ�鿡 ���̰� �����Ѵ�.
			GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
		}
	}
}