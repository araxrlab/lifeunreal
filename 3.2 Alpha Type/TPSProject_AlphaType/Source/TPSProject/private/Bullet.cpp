// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;

	// 1. 충돌체 등록하기
	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	// 2. 충돌프로파일 설정
	collisionComp->SetCollisionProfileName(TEXT("BlockAll"));
	// 3. 충돌체 크기 설정
	collisionComp->SetSphereRadius(13);
	// 4. 루트로 등록
	RootComponent = collisionComp;

	// 5. 외관 컴포넌트 등록하기
	bodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshComp"));
	// 6. 부모 컴포넌트 지정
	bodyMeshComp->SetupAttachment(collisionComp);
	// 7. 충돌 비활성화
	bodyMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// 8. 외관 크기 설정
	bodyMeshComp->SetRelativeScale3D(FVector(0.25f));

	// 발사체 컴포넌트
	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	// movement 컴포넌트가 갱신시킬 컴포넌트 지정
	movementComp->SetUpdatedComponent(collisionComp);
	// 초기속도
	movementComp->InitialSpeed = 5000;
	// 최대속도
	movementComp->MaxSpeed = 5000;
	// 반동 여부
	movementComp->bShouldBounce = true;
	// 반동 값
	movementComp->Bounciness = 0.3f;

	// 생명 시간 주기
	//InitialLifeSpan = 2.0f;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle deathTimer;
	//GetWorld()->GetTimerManager().SetTimer(deathTimer, this, &ABullet::Die, 2.0f, false);
	GetWorld()->GetTimerManager().SetTimer(deathTimer, FTimerDelegate::CreateLambda([this]()->void
		{
			Destroy();
		}), 2.0f, false);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::Die()
{
	Destroy();
}

// 액터의 특정 속성을 수정하면 호출되는 이벤트 함수
void ABullet::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	// speed 값이 수정되었는지 체크
	if (PropertyChangedEvent.GetPropertyName() == TEXT("speed"))
	{
		// 프로젝타일 무브먼트 컴포넌트에 speed 값 적용
		movementComp->InitialSpeed = speed;
		movementComp->MaxSpeed = speed;
	}
}