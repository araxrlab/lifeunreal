// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "PlayerPawn.generated.h"

UCLASS()
class SHOOTINGCPP_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	APlayerPawn();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// �ڽ� �浹ü ������Ʈ
	UPROPERTY(EditAnywhere)
	class UBoxComponent* boxComp;

	// ����ƽ �޽� ������Ʈ
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* meshComp;

	// Input Mapping Context ������ ������ ����
	UPROPERTY(EditAnywhere)
	class UInputMappingContext* imc_playerInput;
	
	// Input Action ������ ������ ����
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_horizontal;

	UPROPERTY(EditAnywhere)
	class UInputAction* ia_vertical;

	UPROPERTY(EditAnywhere)
	class UInputAction* ia_fire;

	// �ӷ� ����
	UPROPERTY(EditAnywhere)
	float moveSpeed = 500;

	// �ѱ� ��ġ
	UPROPERTY(EditAnywhere)
	class UArrowComponent* firePosition;

	// �Ѿ� �������Ʈ
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABullet> bulletFactory;

	// �Ѿ� �߻� ȿ���� ����
	UPROPERTY(EditAnywhere)
	class USoundBase* fireSound;

private:
	// ������� Ű �Է� ���� ���� ����
	float h;
	float v;

	// �Է� �̺�Ʈ �߻� �� ������ �Լ�
	void OnInputHorizontal(const struct FInputActionValue& value);
	void OnInputVertical(const struct FInputActionValue& value);
	
	// �Ѿ� �߻� �Է� ó�� �Լ�
	void Fire();
};
