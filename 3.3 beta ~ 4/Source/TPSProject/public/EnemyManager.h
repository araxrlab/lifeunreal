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
	// �����ð� ���� �ּڰ�
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
	float minTime = 1;
	// �����ð� ���� �ִ밪
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
	float maxTime = 5;
	// ������ ��ġ ���� �迭
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
	TArray<class AActor*> spawnPoints;
	// AEnemy Ÿ���� �������Ʈ �Ҵ���� ����
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
	TSubclassOf<class AEnemy> enemyFactory;

	// ������ ���� �˶� Ÿ�̸�
	FTimerHandle spawnTimerHandle;

	// �� ���� �Լ�
	void CreateEnemy();

	// ������ ��ġ ���� ã�� �Ҵ��ϱ�
	void FindSpawnPoints();
};
