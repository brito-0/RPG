// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGItem.h"
#include "RPGRepair.generated.h"

class UUserWidget;

UCLASS()
class RPG_API ARPGRepair : public ARPGItem
{
	GENERATED_BODY()

public:
	ARPGRepair();

	virtual void UseItem() override;

	virtual void ItemUsed() override;
	
protected:
	void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere, Category = HUD)
	TSubclassOf<UUserWidget> RepairMenuWidget;
	UPROPERTY()
	UUserWidget* RepairMenu;

	virtual void StartScreen() override;
	virtual void StopScreen() override;
	
};
