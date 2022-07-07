#pragma once

#include "Components/ActorComponent.h"
#include "TPSPlayer.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "PlayerBaseComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSPROJECT_API UPlayerBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerBaseComponent()
	{
		// Tick 함수 호출되지 않도록 처리
		PrimaryComponentTick.bCanEverTick = false;
		bWantsInitializeComponent = true;
	};

	virtual void InitializeComponent() override;

	virtual void BeginPlay() override;

	// 사용자 입력맵핑 처리 함수
	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent){};
public:

	// 컴포넌트 소유 액터
	UPROPERTY()
	ATPSPlayer* me;
	
	UPROPERTY()
	UCharacterMovementComponent* moveComp;
};
