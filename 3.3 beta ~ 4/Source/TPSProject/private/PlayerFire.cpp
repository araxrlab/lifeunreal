// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFire.h"
#include "EnemyFSM.h"
#include <Kismet/GameplayStatics.h>
#include "Bullet.h"
#include <Blueprint/UserWidget.h>
#include <Camera/CameraComponent.h>
#include "PlayerAnim.h"
#include <Particles/ParticleSystem.h>

UPlayerFire::UPlayerFire()
{
	// 총알 사운드 가져오기
	ConstructorHelpers::FObjectFinder<USoundBase> tempSound(TEXT("SoundWave'/Game/SniperGun/Rifle.Rifle'"));
	if (tempSound.Succeeded())
	{
		bulletSound = tempSound.Object;
	}
	// 총알 공장 가져오기
	ConstructorHelpers::FClassFinder<ABullet> tempBullet(TEXT("Blueprint'/Game/Blueprints/BP_Bullet.BP_Bullet_C'"));
	if (tempBullet.Succeeded())
	{
		bulletFactory = tempBullet.Class;
	}
	// SniperUI 가져오기
	ConstructorHelpers::FClassFinder<UUserWidget> tempSnierWidget(TEXT("WidgetBlueprint'/Game/Blueprints/WBP_SniperUI.WBP_SniperUI_C'"));
	if (tempSnierWidget.Succeeded())
	{
		sniperUIFactory = tempSnierWidget.Class;
	}
	// Crosshair UI 가져오기
	ConstructorHelpers::FClassFinder<UUserWidget> tempCrosshairWidget(TEXT("WidgetBlueprint'/Game/Blueprints/WBP_Crosshair.WBP_Crosshair_C'"));
	if (tempCrosshairWidget.Succeeded())
	{
		crosshairUIFactory = tempCrosshairWidget.Class;
	}
	// 총알 파편 효과 가져오기
	ConstructorHelpers::FObjectFinder<UParticleSystem> tempEffect(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_BulletEffect.P_BulletEffect'"));
	if (tempEffect.Succeeded())
	{
		bulletEffectFactory = tempEffect.Object;
	}
	// Camera Shake 가져오기
	ConstructorHelpers::FClassFinder<UCameraShakeBase> tempCameraShake(TEXT("Blueprint'/Game/Blueprints/BP_CameraShake.BP_CameraShake_C'"));
	if (tempCameraShake.Succeeded())
	{
		cameraShake = tempCameraShake.Class;
	}
}

void UPlayerFire::BeginPlay()
{
	Super::BeginPlay();

	tpsCamComp = me->tpsCamComp;
	gunMeshComp = me->gunMeshComp;
	sniperGunComp = me->sniperGunComp;

	// 1. 스나이퍼 UI 위젯 인스턴스 생성
	_sniperUI = CreateWidget(GetWorld(), sniperUIFactory);
	// 2. 일반 조준 UI 크로스헤어 인스턴스 생성
	_crosshairUI = CreateWidget(GetWorld(), crosshairUIFactory);
	// 3. 일반 조준 UI 등록
	_crosshairUI->AddToViewport();

	// 기본으로 스나이퍼건을 사용하도록 설정
	ChangeToSniperGun();
}

void UPlayerFire::SetupInputBinding(class UInputComponent* PlayerInputComponent)
{
	// 총알발사 이벤트 처리함수 바인딩
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &UPlayerFire::InputFire);
	// 총 교체 이벤트 처리함수 바인딩
	PlayerInputComponent->BindAction(TEXT("GrenadeGun"), IE_Pressed, this, &UPlayerFire::ChangeToGrenadeGun);
	PlayerInputComponent->BindAction(TEXT("SniperGun"), IE_Pressed, this, &UPlayerFire::ChangeToSniperGun);
	// 스나이퍼 조준 모드 이벤트 처리 함수 바인딩
	PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Pressed, this, &UPlayerFire::SniperAim);
	PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Released, this, &UPlayerFire::SniperAim);
}

void UPlayerFire::InputFire()
{
	UGameplayStatics::PlaySound2D(GetWorld(), bulletSound);

	// 카메라셰이크 재생
	auto controller = GetWorld()->GetFirstPlayerController();
	controller->PlayerCameraManager->StartCameraShake(cameraShake);

	// 공격 애니메이션 재생
	auto anim = Cast<UPlayerAnim>(me->GetMesh()->GetAnimInstance());
	anim->PlayAttackAnim();

	// 유탄총 사용시
	if (bUsingGrenadeGun)
	{
		// 총알발사처리
		FTransform firePosition = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
		GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition);
	}
	// 스나이퍼건 사용시
	else
	{
		// LineTrace 의 시작위치
		FVector startPos = tpsCamComp->GetComponentLocation();
		// LineTrace 의 종료위치
		FVector endPos = tpsCamComp->GetComponentLocation() + tpsCamComp->GetForwardVector() * 5000;
		// LineTrace 의 충돌 정보를 담을 변수
		FHitResult hitInfo;
		// 충돌 옵션 설정 변수
		FCollisionQueryParams params;
		// 자기 자신(플레이어) 는 충돌에서 제외
		params.AddIgnoredActor(me);
		// Channel 필터를 이용한 LineTrace 충돌 검출 (충돌정보, 시작위치, 종료위치, 검출채널, 충돌 옵션)
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);
		// LineTrace 가 부딪혔을 때
		if (bHit)
		{
			// 총알파편효과 트렌스폼
			FTransform bulletTrans;
			// 부딪힌 위치 할당
			bulletTrans.SetLocation(hitInfo.ImpactPoint);
			// 총알파편효과 인스턴스 생성
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectFactory, bulletTrans);

			auto hitComp = hitInfo.GetComponent();
			// 1.	만약 컴포넌트에 물리가 적용되어 있다면
			if (hitComp && hitComp->IsSimulatingPhysics())
			{
				// 2. 날려버릴 힘과 방향이 필요
				FVector force = -hitInfo.ImpactNormal * hitComp->GetMass() * 500000;
				// 3. 그 방향으로 날려버리고 싶다.
				hitComp->AddForce(force);
			}

			// 부딪힌 대상이 적인지 판단하기
			auto enemy = hitInfo.GetActor()->GetDefaultSubobjectByName(TEXT("FSM"));
			if (enemy)
			{
				auto enemyFSM = Cast<UEnemyFSM>(enemy);
				enemyFSM->OnDamageProcess();
			}
		}
	}
}

// 유탄총으로 변경
void UPlayerFire::ChangeToGrenadeGun()
{
	// 유탄총 사용중으로 체크
	bUsingGrenadeGun = true;
	sniperGunComp->SetVisibility(false);
	gunMeshComp->SetVisibility(true);

	// 유탄총 사용할지 여부 전달
	me->OnUsingGrenade(bUsingGrenadeGun);
}

// 스나이퍼건으로 변경
void UPlayerFire::ChangeToSniperGun()
{
	bSniperAim = false;
	bUsingGrenadeGun = false;
	sniperGunComp->SetVisibility(true);
	gunMeshComp->SetVisibility(false);

	// 유탄총 사용할지 여부 전달
	me->OnUsingGrenade(bUsingGrenadeGun);
}

// 스나이퍼 조준
void UPlayerFire::SniperAim()
{
	// 스나이퍼건 모드가 아니라면 처리하지 않는다.
	if (bUsingGrenadeGun)
	{
		return;
	}
	// Pressed 입력처리
	if (bSniperAim == false)
	{
		// 1. 스나이퍼 조준 모드 활성화
		bSniperAim = true;
		// 2. 스나이퍼조준 UI 등록
		_sniperUI->AddToViewport();
		// 3. 카메라의 시야각 Field Of View 설정
		tpsCamComp->SetFieldOfView(45.0f);
		// 4. 일반 조준 UI 제거
		_crosshairUI->RemoveFromParent();
	}
	// Released 입력처리
	else
	{
		// 1. 스나이퍼 조준 모드 비활성화
		bSniperAim = false;
		// 2. 스나이퍼 조준 UI 화면에서 제거
		_sniperUI->RemoveFromParent();
		// 3. 카메라 시야각 원래대로 복원
		tpsCamComp->SetFieldOfView(90.0f);
		// 4. 일반 조준 UI 등록
		_crosshairUI->AddToViewport();
	}
}