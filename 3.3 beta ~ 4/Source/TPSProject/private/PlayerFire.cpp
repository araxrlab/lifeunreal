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
	// �Ѿ� ���� ��������
	ConstructorHelpers::FObjectFinder<USoundBase> tempSound(TEXT("SoundWave'/Game/SniperGun/Rifle.Rifle'"));
	if (tempSound.Succeeded())
	{
		bulletSound = tempSound.Object;
	}
	// �Ѿ� ���� ��������
	ConstructorHelpers::FClassFinder<ABullet> tempBullet(TEXT("Blueprint'/Game/Blueprints/BP_Bullet.BP_Bullet_C'"));
	if (tempBullet.Succeeded())
	{
		bulletFactory = tempBullet.Class;
	}
	// SniperUI ��������
	ConstructorHelpers::FClassFinder<UUserWidget> tempSnierWidget(TEXT("WidgetBlueprint'/Game/Blueprints/WBP_SniperUI.WBP_SniperUI_C'"));
	if (tempSnierWidget.Succeeded())
	{
		sniperUIFactory = tempSnierWidget.Class;
	}
	// Crosshair UI ��������
	ConstructorHelpers::FClassFinder<UUserWidget> tempCrosshairWidget(TEXT("WidgetBlueprint'/Game/Blueprints/WBP_Crosshair.WBP_Crosshair_C'"));
	if (tempCrosshairWidget.Succeeded())
	{
		crosshairUIFactory = tempCrosshairWidget.Class;
	}
	// �Ѿ� ���� ȿ�� ��������
	ConstructorHelpers::FObjectFinder<UParticleSystem> tempEffect(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_BulletEffect.P_BulletEffect'"));
	if (tempEffect.Succeeded())
	{
		bulletEffectFactory = tempEffect.Object;
	}
	// Camera Shake ��������
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

	// 1. �������� UI ���� �ν��Ͻ� ����
	_sniperUI = CreateWidget(GetWorld(), sniperUIFactory);
	// 2. �Ϲ� ���� UI ũ�ν���� �ν��Ͻ� ����
	_crosshairUI = CreateWidget(GetWorld(), crosshairUIFactory);
	// 3. �Ϲ� ���� UI ���
	_crosshairUI->AddToViewport();

	// �⺻���� �������۰��� ����ϵ��� ����
	ChangeToSniperGun();
}

void UPlayerFire::SetupInputBinding(class UInputComponent* PlayerInputComponent)
{
	// �Ѿ˹߻� �̺�Ʈ ó���Լ� ���ε�
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &UPlayerFire::InputFire);
	// �� ��ü �̺�Ʈ ó���Լ� ���ε�
	PlayerInputComponent->BindAction(TEXT("GrenadeGun"), IE_Pressed, this, &UPlayerFire::ChangeToGrenadeGun);
	PlayerInputComponent->BindAction(TEXT("SniperGun"), IE_Pressed, this, &UPlayerFire::ChangeToSniperGun);
	// �������� ���� ��� �̺�Ʈ ó�� �Լ� ���ε�
	PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Pressed, this, &UPlayerFire::SniperAim);
	PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Released, this, &UPlayerFire::SniperAim);
}

void UPlayerFire::InputFire()
{
	UGameplayStatics::PlaySound2D(GetWorld(), bulletSound);

	// ī�޶����ũ ���
	auto controller = GetWorld()->GetFirstPlayerController();
	controller->PlayerCameraManager->StartCameraShake(cameraShake);

	// ���� �ִϸ��̼� ���
	auto anim = Cast<UPlayerAnim>(me->GetMesh()->GetAnimInstance());
	anim->PlayAttackAnim();

	// ��ź�� ����
	if (bUsingGrenadeGun)
	{
		// �Ѿ˹߻�ó��
		FTransform firePosition = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
		GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition);
	}
	// �������۰� ����
	else
	{
		// LineTrace �� ������ġ
		FVector startPos = tpsCamComp->GetComponentLocation();
		// LineTrace �� ������ġ
		FVector endPos = tpsCamComp->GetComponentLocation() + tpsCamComp->GetForwardVector() * 5000;
		// LineTrace �� �浹 ������ ���� ����
		FHitResult hitInfo;
		// �浹 �ɼ� ���� ����
		FCollisionQueryParams params;
		// �ڱ� �ڽ�(�÷��̾�) �� �浹���� ����
		params.AddIgnoredActor(me);
		// Channel ���͸� �̿��� LineTrace �浹 ���� (�浹����, ������ġ, ������ġ, ����ä��, �浹 �ɼ�)
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);
		// LineTrace �� �ε����� ��
		if (bHit)
		{
			// �Ѿ�����ȿ�� Ʈ������
			FTransform bulletTrans;
			// �ε��� ��ġ �Ҵ�
			bulletTrans.SetLocation(hitInfo.ImpactPoint);
			// �Ѿ�����ȿ�� �ν��Ͻ� ����
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectFactory, bulletTrans);

			auto hitComp = hitInfo.GetComponent();
			// 1.	���� ������Ʈ�� ������ ����Ǿ� �ִٸ�
			if (hitComp && hitComp->IsSimulatingPhysics())
			{
				// 2. �������� ���� ������ �ʿ�
				FVector force = -hitInfo.ImpactNormal * hitComp->GetMass() * 500000;
				// 3. �� �������� ���������� �ʹ�.
				hitComp->AddForce(force);
			}

			// �ε��� ����� ������ �Ǵ��ϱ�
			auto enemy = hitInfo.GetActor()->GetDefaultSubobjectByName(TEXT("FSM"));
			if (enemy)
			{
				auto enemyFSM = Cast<UEnemyFSM>(enemy);
				enemyFSM->OnDamageProcess();
			}
		}
	}
}

// ��ź������ ����
void UPlayerFire::ChangeToGrenadeGun()
{
	// ��ź�� ��������� üũ
	bUsingGrenadeGun = true;
	sniperGunComp->SetVisibility(false);
	gunMeshComp->SetVisibility(true);

	// ��ź�� ������� ���� ����
	me->OnUsingGrenade(bUsingGrenadeGun);
}

// �������۰����� ����
void UPlayerFire::ChangeToSniperGun()
{
	bSniperAim = false;
	bUsingGrenadeGun = false;
	sniperGunComp->SetVisibility(true);
	gunMeshComp->SetVisibility(false);

	// ��ź�� ������� ���� ����
	me->OnUsingGrenade(bUsingGrenadeGun);
}

// �������� ����
void UPlayerFire::SniperAim()
{
	// �������۰� ��尡 �ƴ϶�� ó������ �ʴ´�.
	if (bUsingGrenadeGun)
	{
		return;
	}
	// Pressed �Է�ó��
	if (bSniperAim == false)
	{
		// 1. �������� ���� ��� Ȱ��ȭ
		bSniperAim = true;
		// 2. ������������ UI ���
		_sniperUI->AddToViewport();
		// 3. ī�޶��� �þ߰� Field Of View ����
		tpsCamComp->SetFieldOfView(45.0f);
		// 4. �Ϲ� ���� UI ����
		_crosshairUI->RemoveFromParent();
	}
	// Released �Է�ó��
	else
	{
		// 1. �������� ���� ��� ��Ȱ��ȭ
		bSniperAim = false;
		// 2. �������� ���� UI ȭ�鿡�� ����
		_sniperUI->RemoveFromParent();
		// 3. ī�޶� �þ߰� ������� ����
		tpsCamComp->SetFieldOfView(90.0f);
		// 4. �Ϲ� ���� UI ���
		_crosshairUI->AddToViewport();
	}
}