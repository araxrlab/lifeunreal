#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

// 사용할 상태 정의
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle,
	Move,
	Attack,
	Damage,
	Die,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSPROJECT_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEnemyFSM();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
public:
	// 상태변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=FSM)
	EEnemyState mState = EEnemyState::Idle;

	// 대기상태
	void IdleState();
	// 이동상태
	void MoveState();
	// 공격상태
	void AttackState();
	// 피격상태
	void DamageState();
	// 죽음상태
	void DieState();

	// 대기시간
	UPROPERTY(EditDefaultsOnly, Category=FSM)
	float idleDelayTime = 2;
	// 경과시간
	float currentTime = 0;

	// 타겟
	UPROPERTY(VisibleAnywhere, Category=FSM)
	class ATPSPlayer* target;

	// 소유액터
	UPROPERTY()
	class AEnemy* me;

	// 공격범위
	UPROPERTY(EditAnywhere, Category=FSM)
	float attackRange = 150.0f;

	// 공격대기시간
	UPROPERTY(EditAnywhere, Category=FSM)
	float attackDelayTime = 2.0f;

	// 피격 알림 이벤트 함수
	void OnDamageProcess();

	// 체력
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=FSM)
	int32 hp = 3;
	// 피격대기시간
	UPROPERTY(EditAnywhere, Category=FSM)
	float damageDelayTime = 2.0f;

	// 아래로 사라지는 속도
	UPROPERTY(EditAnywhere, Category=FSM)
	float dieSpeed = 50.0f;

	// 사용중인 애니메이션 블루프린트
	UPROPERTY()
	class UEnemyAnim* anim;

	// Enemy 를 소유하고 있는 AIController
	UPROPERTY()
	class AAIController* ai;

	// 길찾기 수행시 랜덤위치
	FVector randomPos;
	// 랜덤위치 가져오기
	bool GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest);
};
