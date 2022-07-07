#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyManager.generated.h"

UCLASS()
class TPSPROJECT_API AEnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemyManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	// 랜덤시간 간격 최솟값
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
	float minTime = 1;
	// 랜덤시간 간격 최대값
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
	float maxTime = 5;
	// 스폰할 위치 정보 배열
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
	TArray<class AActor*> spawnPoints;
	// AEnemy 타입의 블루프린트 할당받을 변수
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
	TSubclassOf<class AEnemy> enemyFactory;

	// 스폰을 위한 알람 타이머
	FTimerHandle spawnTimerHandle;

	// 적 생성 함수
	void CreateEnemy();

	// 스폰할 위치 동적 찾아 할당하기
	void FindSpawnPoints();
};
