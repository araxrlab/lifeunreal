#include "TPSPlayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include "PlayerMove.h"
#include "PlayerFire.h"
#include "TPSProject.h"
#include <Kismet/GameplayStatics.h>

ATPSPlayer::ATPSPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	// 1. ���̷�Ż�޽� �����͸� �ҷ����� �ʹ�.
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("SkeletalMesh'/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		// 2. Mesh ������Ʈ�� ��ġ�� �����ϰ� �ʹ�.
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	// 3. TPS ī�޶� ���̰� �ʹ�.
	// 3-1. SpringArm ������Ʈ ���̱�
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0, 70, 90));
	springArmComp->TargetArmLength = 400;
	springArmComp->bUsePawnControlRotation = true;
	// 3-2. Camera ������Ʈ ���̱�
	tpsCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TpsCamComp"));
	tpsCamComp->SetupAttachment(springArmComp);
	tpsCamComp->bUsePawnControlRotation = false;
	
	bUseControllerRotationYaw = true;
	// 2�� ����
	JumpMaxCount = 2;

	// 4. �� ���̷�Ż�޽� ������Ʈ ���
	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
	// 4-1. �θ� ������Ʈ�� Mesh ������Ʈ�� ����
	gunMeshComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	// 4-2. ���̷�Ż�޽� ������ �ε�
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	// 4-3. �����ͷε尡 �����ߴٸ�
	if (TempGunMesh.Succeeded())
	{
		// 4-4. ���̷�Ż�޽� ������ �Ҵ�
		gunMeshComp->SetSkeletalMesh(TempGunMesh.Object);
		// 4-5. ��ġ �����ϱ�
		gunMeshComp->SetRelativeLocation(FVector(-17, 10, -3)); 
		gunMeshComp->SetRelativeRotation(FRotator(0, 90, 0));
	}

	// 5. �������۰� ������Ʈ ���
	sniperGunComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperGunComp"));
	// 5-1. �θ� ������Ʈ�� Mesh ������Ʈ�� ����
	sniperGunComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	// 5-2. ����ƽ�޽� ������ �ε�
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSniperMesh(TEXT("StaticMesh'/Game/SniperGun/sniper1.sniper1'"));
	// 5-3. �����ͷε尡 �����ߴٸ�
	if (TempSniperMesh.Succeeded())
	{
		// 5-4. ����ƽ�޽� ������ �Ҵ�
		sniperGunComp->SetStaticMesh(TempSniperMesh.Object);		

		// 5-5. ��ġ �����ϱ�
		sniperGunComp->SetRelativeLocation(FVector(-42, 7, 1));
		sniperGunComp->SetRelativeRotation(FRotator(0, 90, 0));
		// 5-6. ũ�� �����ϱ�
		sniperGunComp->SetRelativeScale3D(FVector(0.15f));
	}

	playerMove = CreateDefaultSubobject<UPlayerMove>(TEXT("PlayerMove"));
	//playerFire = CreateDefaultSubobject<UPlayerFire>(TEXT("PlayerFire"));
}

void ATPSPlayer::TestFunc(FName name)
{
	UE_LOG(LogTemp, Warning, TEXT("TestFunc Call : %s"), *name.ToString());
}

void ATPSPlayer::PlayDelegate()
{
	// �Ϲ� ��������Ʈ
	myVar.ExecuteIfBound(TEXT("Single Brad"));
	// ��Ƽĳ��Ʈ ��������Ʈ
	myMultiVar.Broadcast(TEXT("Multi Brad"));
}

void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	hp = initialHp;

	/*
	myVar.BindUObject(this, &ATPSPlayer::TestFunc);
	myVar.BindUFunction(this, TEXT("TestFunc"));
	myVar.BindLambda([this](FName name)->void {});
	// Dynamic Delegate
	myDynamicVar.BindDynamic(this, &ATPSPlayer::TestFunc);
	// Multicast Delegate
	myMultiVar.AddUObject(this, &ATPSPlayer::TestFunc);
	// Dynamic Multicast Delegate
	myDMVar.AddDynamic(this, &ATPSPlayer::TestFunc);

	FTimerHandle timer;
	GetWorld()->GetTimerManager().SetTimer(timer, this, &ATPSPlayer::PlayDelegate, 2);
	*/
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// ������Ʈ���� �Է� ���ε� ó�� �ϵ��� ȣ��
	onInputBindingDelegate.Broadcast(PlayerInputComponent);

	/*playerMove->SetupInputBinding(PlayerInputComponent);
	playerFire->SetupInputBinding(PlayerInputComponent);*/
}

void ATPSPlayer::OnHitEvent()
{
	PRINT_LOG(TEXT("Damaged !!!!!"));
	hp--;
	if (hp <= 0)
	{
		PRINT_LOG(TEXT("Player is dead!"));
		OnGameOver();
	}
}

void ATPSPlayer::OnGameOver_Implementation()
{
	// ���ӿ����� �Ͻ�����
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}





