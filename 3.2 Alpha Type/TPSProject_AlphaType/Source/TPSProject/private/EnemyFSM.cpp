// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "TPSPlayer.h"
#include "Enemy.h"
#include <Kismet/GameplayStatics.h>
#include "TPSProject.h"
#include <Components/CapsuleComponent.h>
#include "EnemyAnim.h"
#include <AIController.h>
#include <NavigationSystem.h>

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	// ���忡�� ATPSPlayer Ÿ�� ã�ƿ���
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), ATPSPlayer::StaticClass());
	// ATPSPlayer Ÿ������ ĳ����
	target = Cast<ATPSPlayer>(actor);
	// ���� ��ü ��������
	me = Cast<AEnemy>(GetOwner());
	
	// UEnemyAnim* �Ҵ�
	anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());

	// AAIController �Ҵ��ϱ�
	ai = Cast<AAIController>(me->GetController());
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (mState)
	{
	case EEnemyState::Idle:
		IdleState();
		break;
	case EEnemyState::Move:
		MoveState();
		break;
	case EEnemyState::Attack:
		AttackState();
		break;
	case EEnemyState::Damage:
		DamageState();
		break;
	case EEnemyState::Die:
		DieState();
		break;
	}
}

// ��� ����
void UEnemyFSM::IdleState() 
{
	// 1. �ð��� �귶���ϱ�
	currentTime += GetWorld()->DeltaTimeSeconds;
	// 2. ���� ��� �ð��� ��� �ð��� �ʰ��ߴٸ�
	if (currentTime > idleDelayTime)
	{
		// 3. �̵� ���·� ��ȯ�ϰ� �ʹ�.
		mState = EEnemyState::Move;
		// ��� �ð� �ʱ�ȭ 
		currentTime = 0;

		// �ִϸ��̼� ���� ����ȭ
		anim->animState = mState;
		// ���� ������ ��ġ �����ֱ�
		GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
	}
}

// �̵� ����
void UEnemyFSM::MoveState() 
{
	// 1. Ÿ�� �������� �ʿ��ϴ�.
	FVector destination = target->GetActorLocation();
	// 2. ������ �ʿ��ϴ�.
	FVector dir = destination - me->GetActorLocation();
	// 3. �������� �̵��ϰ� �ʹ�.
	//me->AddMovementInput(dir.GetSafeNormal());
	// NavigationSystem ��ü ������
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	// ������ �� ã�� ��� ������ �˻�
	FPathFindingQuery query;
	FAIMoveRequest req;
	// ���������� ������ �� �ִ� ����
	req.SetAcceptanceRadius(3);
	req.SetGoalLocation(destination);
	// �� ã�⸦ ���� ���� ����
	ai->BuildPathfindingQuery(req, query);
	// �� ã�� ��� ��������
	FPathFindingResult r = ns->FindPathSync(query);
	// ������ ������ ��ã�� ���� ���� Ȯ��
	if (r.Result == ENavigationQueryResult::Success)
	{
		// Ÿ�������� �̵�
		ai->MoveToLocation(destination);
	}
	else
	{
		// ���� ��ġ�� �̵�
		auto result = ai->MoveToLocation(randomPos);
		// �������� �����ϸ�
		if (result == EPathFollowingRequestResult::AlreadyAtGoal)
		{
			// ���ο� ���� ��ġ ��������
			GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
		}
	}

	// Ÿ��� ��������� ���� ���·� ��ȯ�ϰ� �ʹ�.
	// 1. ���� �Ÿ��� ���� ���� �ȿ� ������
	if (dir.Size() < attackRange)
	{
		// �� ã�� ��� ����
		ai->StopMovement();

		// 2. ���� ���·� ��ȯ�ϰ� �ʹ�.
		mState = EEnemyState::Attack;
		// �ִϸ��̼� ���� ����ȭ
		anim->animState = mState;
		// ���� �ִϸ��̼� ��� Ȱ��ȭ
		anim->bAttackPlay = true;
		// ���� ���� ��ȯ �� ��� �ð��� �ٷ� �������� ó�� 
		currentTime = attackDelayTime;
	}
}

// ���� ����
void UEnemyFSM::AttackState() 
{
	// ��ǥ: ���� �ð��� �� ���� �����ϰ� �ʹ�.
	// 1. �ð��� �귯�� �Ѵ�.
	currentTime += GetWorld()->DeltaTimeSeconds;
	// 2. ���� �ð��� �����ϱ�
	if (currentTime > attackDelayTime)
	{
		// 3. �����ϰ� �ʹ�.
		PRINT_LOG(TEXT("Attack!!!!!"));
		// ��� �ð� �ʱ�ȭ
		currentTime = 0;
		anim->bAttackPlay = true;
	}

	// ��ǥ: Ÿ���� ���� ������ ����� ���¸� �̵����� ��ȯ�ϰ� �ʹ�.
	// 1. Ÿ����� �Ÿ��� �ʿ��ϴ�.
	float distance = FVector::Distance(target->GetActorLocation(), me -> GetActorLocation());
	// 2. Ÿ����� �Ÿ��� ���� ������ ������ϱ�
	if (distance > attackRange)
	{
		// 3. ���¸� �̵����� ��ȯ�ϰ� �ʹ�.
		mState = EEnemyState::Move;
		// �ִϸ��̼� ���� ����ȭ
		anim->animState = mState;

		GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
	}
}
// �ǰ� ����
void UEnemyFSM::DamageState() 
{
	// 1. �ð��� �귶���ϱ�
	currentTime += GetWorld()->DeltaTimeSeconds;
	// 2. ���� ��� �ð��� ��� �ð��� �ʰ��ߴٸ�
	if (currentTime > damageDelayTime)
	{
		// 3. ��� ���·� ��ȯ�ϰ� �ʹ�.
		mState = EEnemyState::Idle;
		// ��� �ð� �ʱ�ȭ 
		currentTime = 0;
		// �ִϸ��̼� ���� ����ȭ
		anim->animState = mState;
	}
}
// ���� ����
void UEnemyFSM::DieState() 
{
	// ���� ���� �ִϸ��̼��� ������ �ʾҴٸ�
	// �ٴڳ������� �ʵ��� ó��
	if (anim->bDieDone == false)
	{
		return;
	}

	// ��� �Ʒ��� �������� �ʹ�.
	// ��ӿ ���� P = P0 + vt
	FVector P0 = me->GetActorLocation();
	FVector vt = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
	FVector P = P0 + vt;
	me->SetActorLocation(P);

	// 1. ���� 2���� �̻� �����Դٸ�
	if (P.Z < -200.0f)
	{
		// 2. ���Ž�Ų��.
		me->Destroy();
	}
}

// �ǰ� �˸� �̺�Ʈ �Լ�
void UEnemyFSM::OnDamageProcess()
{
	// ü�� ����
	hp--;

	// ���� ü���� �����ִٸ�
	if (hp > 0)
	{
		// ���¸� �ǰ����� ��ȯ
		mState = EEnemyState::Damage;

		currentTime = 0;
		// �ǰ� �ִϸ��̼� ���
		int32 index = FMath::RandRange(0, 1);
		FString sectionName = FString::Printf(TEXT("Damage%d"), 0);
		anim->PlayDamageAnim(FName(*sectionName));
	}
	// �׷��� �ʴٸ�
	else
	{
		// ���¸� �������� ��ȯ
		mState = EEnemyState::Die;
		// ĸ�� �浹ü ��Ȱ��ȭ
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// ���� �ִϸ��̼� ���
		anim->PlayDamageAnim(TEXT("Die"));
	}

	// �ִϸ��̼� ���� ����ȭ
	anim->animState = mState;

	ai->StopMovement();
}

// ���� ��ġ ��������
bool UEnemyFSM::GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector&
	dest)
{
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;
	bool result = ns->GetRandomReachablePointInRadius(centerLocation, radius, loc);
	dest = loc.Location;
	return result;
}