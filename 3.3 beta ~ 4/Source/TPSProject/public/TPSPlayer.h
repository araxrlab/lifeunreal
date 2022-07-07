// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"


DECLARE_DELEGATE_OneParam(FMyDelegate, FName);
DECLARE_DYNAMIC_DELEGATE_OneParam(FMyDynamicDelegate, FName, name);
DECLARE_MULTICAST_DELEGATE_OneParam(FMyMultiDelegate, FName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMyDMDelegate, FName, name);

DECLARE_MULTICAST_DELEGATE_OneParam(FInputBindingDelegate, class UInputComponent*);


// 목표 : 사용자의 좌우입력을 받아 이동하고 싶다.
// 필요속성 : 이동속도, 이동방향
UCLASS()
class TPSPROJECT_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()
public:
	// 입력바인딩 델리게이트
	FInputBindingDelegate onInputBindingDelegate;

public:
	// Sets default values for this character's properties
	ATPSPlayer();

	FMyDelegate myVar;
	FMyDynamicDelegate myDynamicVar;
	FMyMultiDelegate myMultiVar;
	FMyDMDelegate myDMVar;

	UFUNCTION()
	void TestFunc(FName name);

	void PlayDelegate();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere, Category=Camera)
	class USpringArmComponent* springArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* tpsCamComp;

	// 총 스켈레탈메쉬
	UPROPERTY(VisibleAnywhere, Category=GunMesh)
	class USkeletalMeshComponent* gunMeshComp;

	// 스나이퍼건 스테틱메쉬 추가
	UPROPERTY(VisibleAnywhere, Category = GunMesh)
	class UStaticMeshComponent* sniperGunComp;
public:
	UPROPERTY(VisibleAnywhere, Category = Component)
	class UPlayerBaseComponent* playerMove;
	UPROPERTY(VisibleAnywhere, Category = Component)
	class UPlayerBaseComponent* playerFire;

	// 현재체력
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Health)
	int32 hp;
	// 초기 hp 값
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Health)
	int32 initialHp = 10;

	// 피격 받았을 때 처리
	UFUNCTION(BlueprintCallable, Category = Health)
	void OnHitEvent();

	// 게임오버 될 때 호출될 함수
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Health)
	void OnGameOver();

	// 총 바꿀 때 호출되는 이벤트 함수
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Health)
	void OnUsingGrenade(bool isGrenade);
};
