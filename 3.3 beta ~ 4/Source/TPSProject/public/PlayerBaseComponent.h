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
		// Tick �Լ� ȣ����� �ʵ��� ó��
		PrimaryComponentTick.bCanEverTick = false;
		bWantsInitializeComponent = true;
	};

	virtual void InitializeComponent() override;

	virtual void BeginPlay() override;

	// ����� �Է¸��� ó�� �Լ�
	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent){};
public:

	// ������Ʈ ���� ����
	UPROPERTY()
	ATPSPlayer* me;
	
	UPROPERTY()
	UCharacterMovementComponent* moveComp;
};
