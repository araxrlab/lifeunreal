// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/ArrowComponent.h"
#include "Bullet.h"
#include "Kismet/GameplayStatics.h"


APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// �ڽ� �ݶ��̴� ������Ʈ�� �����Ѵ�.
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("My Box Component"));

	// ������ �ڽ� �ݶ��̴� ������Ʈ�� �ֻ�� ������Ʈ�� �����Ѵ�.
	SetRootComponent(boxComp);

	// ����ƽ �޽� ������Ʈ�� �����Ѵ�.
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My Static Mesh"));

	// �ڽ� �ݶ��̴� ������Ʈ�� �ڽ� ������Ʈ�� �����Ѵ�.
	meshComp->SetupAttachment(boxComp);

	// �޽� ũ�⸦ 50 x 50 x 50���� �����Ѵ�.
	FVector boxSize = FVector(50.0f, 50.0f, 50.0f);
	boxComp->SetBoxExtent(boxSize);

	// �ѱ� ǥ�� ������Ʈ�� �����ϰ� �ڽ� ������Ʈ�� �ڽ� ������Ʈ�� �����Ѵ�.
	firePosition = CreateDefaultSubobject<UArrowComponent>(TEXT("Fire Position"));
	firePosition->SetupAttachment(boxComp);
	
	// ������ �̺�Ʈ�� �Ҵ�.
	boxComp->SetGenerateOverlapEvents(true);

	// �浹 ������ Query And Physics�� �����Ѵ�.
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	// Object Type�� 1�� ä��(Player)�� �����Ѵ�.
	boxComp->SetCollisionObjectType(ECC_GameTraceChannel1);

	// ��� ä���� �浹 ���� �������� �����Ѵ�.
	boxComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	// ���ʹ̿ʹ� �浹 �̺�Ʈ üũ(Quary)�� �Ѵ�.
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
	boxComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
}

void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
	// ���� �÷��̾ ������ ��Ʈ�ѷ��� �����´�.
	APlayerController* pc = GetWorld()->GetFirstPlayerController();

	// ����, �÷��̾� ��Ʈ�ѷ� ������ ���� ����ִٸ顦
	if (pc != nullptr)
	{
		// �÷��̾� ��Ʈ�ѷ��κ��� �Է� ���� �ý��� ������ �����´�.
		UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		
		if (subsys != nullptr)
		{
			// �Է� ���� �ý��ۿ� IMC ���� ������ �����Ѵ�.
			subsys->AddMappingContext(imc_playerInput, 0);
		}
	}
}

void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ������� �Է� Ű�� �̿��ؼ� 
	// 1. ���� �Է� ���� �¿� �Է� ���� �̿��ؼ� ���� ���͸� �����.
	FVector dir = FVector(0, h, v);

	// 2. ���� ������ ���̰� 1�� �ǵ��� ���͸� ����ȭ�Ѵ�.
	dir.Normalize();

	// 3. �̵��� ��ġ ��ǥ�� ���Ѵ�(p = p0 + vt).
	FVector newLocation = GetActorLocation() + dir * moveSpeed * DeltaTime;

	// 4. ���� ������ ��ġ ��ǥ�� �տ��� ���� �� ��ǥ�� �����Ѵ�.
	SetActorLocation(newLocation, true);

}

void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (enhancedInputComponent != nullptr)
	{
		enhancedInputComponent->BindAction(ia_horizontal, ETriggerEvent::Triggered,	this, &APlayerPawn::OnInputHorizontal);
		enhancedInputComponent->BindAction(ia_horizontal, ETriggerEvent::Completed,	this, &APlayerPawn::OnInputHorizontal);
		enhancedInputComponent->BindAction(ia_vertical, ETriggerEvent::Triggered, this, &APlayerPawn::OnInputVertical);
		enhancedInputComponent->BindAction(ia_vertical, ETriggerEvent::Completed, this, &APlayerPawn::OnInputVertical);
		enhancedInputComponent->BindAction(ia_fire, ETriggerEvent::Started, this, &APlayerPawn::Fire);
	}
}

// �¿��� �Է� ó�� �Լ�
void APlayerPawn::OnInputHorizontal(const FInputActionValue& value)
{
	//float hor = value.Get<float>();
	//UE_LOG(LogTemp, Warning, TEXT(��Horizontal: % .2f��), hor);
	h = value.Get<float>();
}

void APlayerPawn::OnInputVertical(const FInputActionValue& value)
{
	//float ver = value.Get<float>();
	//UE_LOG(LogTemp, Warning, TEXT(��Vertical: % .2f��), ver);
	v = value.Get<float>();
}

// ���콺 ���� ��ư �Է� ó�� �Լ�
void APlayerPawn::Fire()
{
	// �Ѿ� �������Ʈ ������ firePosition ��ġ�� �����Ѵ�.
	ABullet* bullet = GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition->GetComponentLocation(), firePosition->GetComponentRotation());
	
	// fireSound ������ �Ҵ�� ���� ������ �����Ѵ�.
	UGameplayStatics::PlaySound2D(GetWorld(), fireSound);
}