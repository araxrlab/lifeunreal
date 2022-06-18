// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFactory.h"
#include "EnemyActor.h"

// Sets default values
AEnemyFactory::AEnemyFactory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyFactory::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyFactory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 만일, 경과된 시간이 생성할 시간을 도과했다면...
	if (currentTime > delayTime)
	{
		// enemy 변수에 할당된 블루프린트를 자신의 위치에 생성한다.
		AEnemyActor* spawnActor = GetWorld()->SpawnActor<AEnemyActor>(enemy, GetActorLocation(), GetActorRotation());

		// 경과된 시간을 0초로 초기화한다.
		currentTime = 0;
	}
	// 그렇지 않다면...
	else
	{
		// 현재 프레임의 경과 시간을 누적시킨다.
		//currentTime = currentTime + DeltaTime;
		currentTime += DeltaTime;
	}
}

