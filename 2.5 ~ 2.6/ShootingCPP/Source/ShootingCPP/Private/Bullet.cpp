// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "EnemyActor.h"
#include "Kismet/GameplayStatics.h"
#include "ShootingGameModeBase.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));

	// 박스 컴포넌트의 크기를 변경한다.
	boxComp->SetWorldScale3D(FVector(0.75f, 0.25f, 1.0f));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	meshComp->SetupAttachment(boxComp);

	// 박스 컴포넌트의 콜리전 프리셋을 Bullet으로 설정한다.
	boxComp->SetCollisionProfileName(TEXT("Bullet"));
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
	// 박스 컴포넌트의 충돌 오버랩 이벤트에 BulletOverlap 함수를 연결한다.
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnBulletOverlap);


}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 전방으로 이동될 위치를 계산한다.
	FVector newLocation = GetActorLocation() + GetActorForwardVector() * moveSpeed * DeltaTime;

	// 계산된 위치 좌표를 액터의 새 좌표로 넣는다.
	SetActorLocation(newLocation);
}

// 충돌 이벤트가 발생할 때 실행할 함수
void ABullet::OnBulletOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 충돌한 액터를 AEnemyActor 클래스로 변환해본다.
	AEnemyActor* enemy = Cast<AEnemyActor>(OtherActor);

	// 만일, 캐스팅이 정상적으로 되어서 AEnemyActor 포인터 변수에 값이 있다면...
	if (enemy != nullptr)
	{
		// 충돌한 액터를 제거한다.
		OtherActor->Destroy();
		
		// 폭발 이펙트를 생성한다.
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionFX, GetActorLocation(), GetActorRotation());

		// 현재 게임 모드를 가져온다.
		AGameModeBase* currentMode = GetWorld()->GetAuthGameMode();

		// AShootingGameModeBase 클래스로 변환한다.
		AShootingGameModeBase* currentGameModeBase = Cast<AShootingGameModeBase>(currentMode);

		// 만일, 게임 모드 베이스를 가져왔다면...
		if (currentGameModeBase != nullptr)
		{
			// 게임 모드 베이스의 점수를 1점 추가한다.
			currentGameModeBase->AddScore(1);
		}
	}
	// 자기 자신을 제거한다.
	Destroy();
}