// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AccessTest.generated.h"

UCLASS()
class SHOOTINGCPP_API AAccessTest : public AActor
{
	GENERATED_BODY()
	
public:	
	AAccessTest();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	int32 number;			// �Ϲ� ����
	int32* numPointer;		// ������ ����

	UPROPERTY(EditAnywhere)
	class ACodingTestActor* testPointer;

	void AddPrint(int32 num1, int32 num2, int32* result);
};
