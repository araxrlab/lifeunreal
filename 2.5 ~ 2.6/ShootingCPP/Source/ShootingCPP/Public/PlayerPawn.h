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

	// 박스 충돌체 컴포넌트
	UPROPERTY(EditAnywhere)
	class UBoxComponent* boxComp;

	// 스태틱 메쉬 컴포넌트
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* meshComp;

	// Input Mapping Context 파일의 포인터 변수
	UPROPERTY(EditAnywhere)
	class UInputMappingContext* imc_playerInput;
	
	// Input Action 파일의 포인터 변수
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_horizontal;

	UPROPERTY(EditAnywhere)
	class UInputAction* ia_vertical;

	UPROPERTY(EditAnywhere)
	class UInputAction* ia_fire;

	// 속력 변수
	UPROPERTY(EditAnywhere)
	float moveSpeed = 500;

	// 총구 위치
	UPROPERTY(EditAnywhere)
	class UArrowComponent* firePosition;

	// 총알 블루프린트
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABullet> bulletFactory;

	// 총알 발사 효과음 파일
	UPROPERTY(EditAnywhere)
	class USoundBase* fireSound;

private:
	// 사용자의 키 입력 값을 받을 변수
	float h;
	float v;

	// 입력 이벤트 발생 시 실행할 함수
	void OnInputHorizontal(const struct FInputActionValue& value);
	void OnInputVertical(const struct FInputActionValue& value);
	
	// 총알 발사 입력 처리 함수
	void Fire();
};
