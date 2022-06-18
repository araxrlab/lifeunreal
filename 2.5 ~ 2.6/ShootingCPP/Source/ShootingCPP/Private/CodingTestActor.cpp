// Fill out your copyright notice in the Description page of Project Settings.


#include "CodingTestActor.h"

// Sets default values
ACodingTestActor::ACodingTestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACodingTestActor::BeginPlay()
{
	Super::BeginPlay();
	
	// Hello World 출력하기
	//UE_LOG(LogTemp, Warning, TEXT("Hello World!"));
	//UE_LOG(LogTemp, Warning, TEXT("%d"), number1);
	//UE_LOG(LogTemp, Warning, TEXT("%.2f"), number2);
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *name);

	// Add 함수를 이용해서 number1과 number2 변수의 값의 합을 구한다.
	//int32 result = Add(number1, number2);

	// 함수 결과로 반환된 result의 값을 출력한다.
	//UE_LOG(LogTemp, Warning, TEXT("%d"), result);

	// 만일, number1의 값이 100 이상이라면...
	//if (number1 >= 100)
	//{
	//	// number1의 값이 100보다 큽니다라는 문구를 출력한다.
	//	UE_LOG(LogTemp, Warning, TEXT("number1의 값이 100보다 큽니다!"));
	//}
	//// 그렇지 않고 만일, number1의 값이 10보다 크다면...
	//else if (number2 >= 10)
	//{
	//	// number1의 값이 10이상 100 미만입니다라는 문구를 출력한다.
	//	UE_LOG(LogTemp, Warning, TEXT("number1의 값이 10이상 100 미만입니다!"));
	//}
	//// 둘 다 아니라면...
	//else
	//{
	//	// number1의 값이 10 미만입니다 문구를 출력한다.
	//	UE_LOG(LogTemp, Warning, TEXT("number1의 값이 10 미만입니다!"));
	//}

	//// 만일, isReady가 true라면...
	//if (isReady)
	//{
	//	// Add 함수를 실행한다.
	//	int32 result = Add(number1, number2);
	//	UE_LOG(LogTemp, Warning, TEXT("%d"), result);
	//}
	//// 그렇지 않다면...
	//else
	//{
	//	// 준비가 안됐다는 문구를 출력한다.
	//	UE_LOG(LogTemp, Warning, TEXT("아직 준비가 안됐습니다."));
	//}
	
	// number1의 값을 1씩 증가시켜가면서 100번 출력하고 싶다.
	//for (int32 i = 0; i < 100; i++)
	//{
	//  // number1의 값을 1 증가시킨다.
	//	number1++;
	//	UE_LOG(LogTemp, Warning, TEXT("%d"), number1);
	//}

	// 0부터 99까지 100회 반복해서 실행한다.
	for (int32 i = 0; i < 100; i++)
	{
		// i를 2로 나눈 나머지 값을 구한다.
		int32 result = i % 2;

		// 만일, result 값이  0이면 짝수이므로 출력한다.
		if (result == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("%d는 짝수입니다."), i);
		}
	}
}

// Called every frame
void ACodingTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int32 ACodingTestActor::Add(int32 num1, int32 num2)
{
	int32 result = num1 + num2;
	return result;
}