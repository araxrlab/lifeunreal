// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EngineUtils.h"
#include "PlayerPawn.h"
#include "ShootingGameModeBase.h"


AEnemyActor::AEnemyActor()
{
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static mesh"));
	meshComp->SetupAttachment(boxComp);

	// Collision presets�� Enemy ���������� �����Ѵ�. 
	boxComp->SetCollisionProfileName(TEXT("Enemy"));
}

void AEnemyActor::BeginPlay()
{
	Super::BeginPlay();
	
	// 1 ~ 100 ������ ������ ���� ���� ��÷�Ѵ�.
	int32 drawResult = FMath::RandRange(1, 100);

	// ����, ��÷�� ���� ���� Ȯ�� �������� �۰ų� ���ٸ�...
	if (drawResult <= traceRate)
	{
		// ���� ������ APlayerPawn Ŭ������ �� ���͸� ��� �˻��Ѵ�.
		for (TActorIterator<APlayerPawn> player(GetWorld()); player; ++player)
		{
			// ���� �˻��� ������ �̸��� "BP_PlayerPawn"�̶� ������ ���ԵǾ� �ִٸ�...
			if (player->GetName().Contains(TEXT("BP_PlayerPawn")))
			{
				// �÷��̾� ������ ��ġ - �ڽ��� ��ġ
				dir = player->GetActorLocation() - GetActorLocation();
				dir.Normalize();
			}
		}
	}
	// �׷��� �ʴٸ� ���� ���� ���͸� �����Ѵ�.
	else
	{
		dir = GetActorForwardVector();
	}

	// �ڽ� ������Ʈ�� BeginOverlap ��������Ʈ�� OnEnemyOverlap �Լ��� �����Ѵ�.
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemyActor::OnEnemyOverlap);
}

void AEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// BeginPlay()���� ������ �������� �̵��Ѵ�.
	FVector newLocation = GetActorLocation() + dir * moveSpeed * DeltaTime;
	SetActorLocation(newLocation);
}

void AEnemyActor::OnEnemyOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// �浹�� ��� ���͸� APlayerPawn Ŭ������ ��ȯ�� �õ��Ѵ�.
	APlayerPawn* player = Cast<APlayerPawn>(OtherActor);

	// ����, ĳ������ �����ߴٸ�...
	if (player != nullptr)
	{
		// �ε��� ��� ���͸� �����Ѵ�.
		OtherActor->Destroy();

		// ���� ���� ��带 �����´�.
		AShootingGameModeBase* currentGameMode = Cast<AShootingGameModeBase>(GetWorld()->GetAuthGameMode());

		if (currentGameMode != nullptr)
		{
			// �޴� UI ���� �Լ��� ȣ���Ѵ�.
			currentGameMode->ShowMenu();
		}
	}

	// �ڱ� �ڽ��� �����Ѵ�.
	Destroy();
}