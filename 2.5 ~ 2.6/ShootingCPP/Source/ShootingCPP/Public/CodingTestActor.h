// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CodingTestActor.generated.h"

UCLASS()
class SHOOTINGCPP_API ACodingTestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACodingTestActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 number1 = 10;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 number2 = 30;

	//FString name = "Park Won Seok";
	UPROPERTY(EditInstanceOnly)
	FString name = TEXT("박원석");

	UPROPERTY(EditAnywhere)
	bool isReady = true;
	bool isFinished = false;

	// 더하기 함수 선언
	UFUNCTION(BlueprintPure)
	int32 Add(int32 num1, int32 num2);
};
