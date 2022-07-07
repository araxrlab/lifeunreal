#include "EnemyManager.h"
#include "Enemy.h"
#include <Kismet/GameplayStatics.h>
#include <EngineUtils.h>

AEnemyManager::AEnemyManager()
{
 	// Tick() 함수가 매 프레임 호출되지 않도록 설정
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();
	
	// 1. 랜덤생성시간 구하기
	float createTime = FMath::RandRange(minTime, maxTime);
	// 2. Timer Manager 한테 알람 등록
	GetWorld()->GetTimerManager().SetTimer(spawnTimerHandle, this, &AEnemyManager::CreateEnemy, createTime);

	// 스폰위치 동적 할당
	FindSpawnPoints();
}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyManager::CreateEnemy()
{
	// 랜덤 위치 구하기
	int index = FMath::RandRange(0, spawnPoints.Num()-1);
	// 적 생성 및 배치하기
	GetWorld()->SpawnActor<AEnemy>(enemyFactory, spawnPoints[index]->GetActorLocation(), FRotator(0));
	
	// 다시 랜덤시간에 CreateEnemy 함수가 호출되도록 타이머 설정
	float createTime = FMath::RandRange(minTime, maxTime);
	GetWorld()->GetTimerManager().SetTimer(spawnTimerHandle, this, &AEnemyManager::CreateEnemy, createTime, false);
}

// 스폰위치 동적할당
void AEnemyManager::FindSpawnPoints()
{
	//for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	//{
	//	AActor* spawn = *It;
	//	// 찾은 액터의 이름에 해당 문자열을 포함하고 있다면
	//	if (spawn->GetName().Contains(TEXT("BP_EnemySpawnPoint")))
	//	{
	//		// 스폰목록에 추가
	//		spawnPoints.Add(spawn);
	//	}
	//}


	// 검색으로 찾은 결과 저장할 배열
	TArray<AActor*> allActors;
	// 원하는 타입의 액터 모두 찾아오기
	UGameplayStatics::GetAllActorsOfClass(this, AActor::StaticClass(), allActors);
	// 찾은 결과가 있을 경우 반복적으로
	for (auto spawn : allActors)
	{
		// 찾은 액터의 이름에 해당 문자열을 포함하고 있다면
		if (spawn->GetName().Contains(TEXT("BP_EnemySpawnPoint")))
		{
			// 스폰목록에 추가
			spawnPoints.Add(spawn);
		}
	}
}

