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

	// 1. 스켈레탈메쉬 데이터를 불러오고 싶다.
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("SkeletalMesh'/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		// 2. Mesh 컴포넌트의 위치를 설정하고 싶다.
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	// 3. TPS 카메라를 붙이고 싶다.
	// 3-1. SpringArm 컴포넌트 붙이기
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0, 70, 90));
	springArmComp->TargetArmLength = 400;
	springArmComp->bUsePawnControlRotation = true;
	// 3-2. Camera 컴포넌트 붙이기
	tpsCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TpsCamComp"));
	tpsCamComp->SetupAttachment(springArmComp);
	tpsCamComp->bUsePawnControlRotation = false;
	
	bUseControllerRotationYaw = true;
	// 2단 점프
	JumpMaxCount = 2;

	// 4. 총 스켈레탈메쉬 컴포넌트 등록
	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
	// 4-1. 부모 컴포넌트를 Mesh 컴포넌트로 설정
	gunMeshComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	// 4-2. 스켈레탈메쉬 데이터 로드
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	// 4-3. 데이터로드가 성공했다면
	if (TempGunMesh.Succeeded())
	{
		// 4-4. 스켈레탈메쉬 데이터 할당
		gunMeshComp->SetSkeletalMesh(TempGunMesh.Object);
		// 4-5. 위치 조정하기
		gunMeshComp->SetRelativeLocation(FVector(-17, 10, -3)); 
		gunMeshComp->SetRelativeRotation(FRotator(0, 90, 0));
	}

	// 5. 스나이퍼건 컴포넌트 등록
	sniperGunComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperGunComp"));
	// 5-1. 부모 컴포넌트를 Mesh 컴포넌트로 설정
	sniperGunComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	// 5-2. 스테틱메쉬 데이터 로드
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSniperMesh(TEXT("StaticMesh'/Game/SniperGun/sniper1.sniper1'"));
	// 5-3. 데이터로드가 성공했다면
	if (TempSniperMesh.Succeeded())
	{
		// 5-4. 스테틱메쉬 데이터 할당
		sniperGunComp->SetStaticMesh(TempSniperMesh.Object);		

		// 5-5. 위치 조정하기
		sniperGunComp->SetRelativeLocation(FVector(-42, 7, 1));
		sniperGunComp->SetRelativeRotation(FRotator(0, 90, 0));
		// 5-6. 크기 조정하기
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
	// 일반 델리게이트
	myVar.ExecuteIfBound(TEXT("Single Brad"));
	// 멀티캐스트 델리게이트
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
	
	// 컴포넌트에서 입력 바인딩 처리 하도록 호출
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
	// 게임오버시 일시정지
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}





