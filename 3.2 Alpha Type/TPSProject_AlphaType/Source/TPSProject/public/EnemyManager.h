// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyManager.generated.h"

UCLASS()
class TPSPROJECT_API AEnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// ���� �ð� ���� �ּڰ�
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
	float minTime = 1;
	// ���� �ð� ���� �ִ�
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
