// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGItem.h"
#include "RPGHeal.generated.h"

class UUserWidget;

UCLASS()
class RPG_API ARPGHeal : public ARPGItem
{
	GENERATED_BODY()

public:
	ARPGHeal();

	virtual void UseItem() override;

	virtual void ItemUsed() override;
	
protected:
	void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = HUD)
	TSubclassOf<UUserWidget> HealMenuWidget;
	UPROPERTY()
	UUserWidget* HealMenu;

	virtual void StartScreen() override;
	virtual void StopScreen() override;
	
};
