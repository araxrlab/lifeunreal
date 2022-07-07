#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerMove.generated.h"

UCLASS()
class TPSPROJECT_API UPlayerMove : public UPlayerBaseComponent
{
	GENERATED_BODY()

public:
	UPlayerMove();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	
public:
	// �¿� ȸ�� �Է� ó��
	void Turn(float value);
	// ���� ȸ�� �Է� ó��
	void LookUp(float value);

	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent) override;

public:
	// �ȱ�ӵ�
	UPROPERTY(EditAnywhere, Category = PlayerSetting)
	float walkSpeed = 200;
	// �޸��� �ӵ�
	UPROPERTY(EditAnywhere, Category = PlayerSetting)
	float runSpeed = 600;
	// �̵�����
	FVector direction;

	// �¿� �̵� �Է� �̺�Ʈ ó���Լ�
	void InputHorizontal(float value);
	// ���� �̵� �Է� �̺�Ʈ ó���Լ�
	void InputVertical(float value);

	// ���� �Է� �̺�Ʈ ó���Լ�
	void InputJump();

	// �÷��̾� �̵�ó��
	void Move();

	// �޸��� �̺�Ʈ ó���Լ�
	void InputRun();
};
