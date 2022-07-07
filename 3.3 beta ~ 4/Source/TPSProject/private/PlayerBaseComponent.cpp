#include "PlayerBaseComponent.h"

void UPlayerBaseComponent::InitializeComponent()
{
	Super::InitializeComponent();

	me = Cast<ATPSPlayer>(GetOwner());
	moveComp = me->GetCharacterMovement();
	
	// ��������Ʈ�� ó�� �Լ� ���
	me->onInputBindingDelegate.AddUObject(this, &UPlayerBaseComponent::SetupInputBinding);
}

void UPlayerBaseComponent::BeginPlay()
{
	Super::BeginPlay();
}

