// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGCPP_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* button_Restart;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* button_Quit;

	UFUNCTION()
	void Restart();

	UFUNCTION()
	void Quit();

protected:
	virtual void NativeConstruct() override;
};
