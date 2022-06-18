// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

UCLASS()
class SHOOTINGCPP_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// �ڽ� �浹ü ������Ʈ
	UPROPERTY(EditAnywhere)
	class UBoxComponent* boxComp;

	// ����ƽ �޽� ������Ʈ
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* meshComp;

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

	// ����� �Է� ó�� �Լ�
	void MoveHorizontal(float value);
	void MoveVertical(float value);
	
	// �Ѿ� �߻� �Է� ó�� �Լ�
	void Fire();
};
