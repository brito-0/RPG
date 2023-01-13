// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGItem.h"
#include "RPGBullets.generated.h"

UCLASS()
class RPG_API ARPGBullets : public ARPGItem
{
	GENERATED_BODY()

public:
	ARPGBullets();

	virtual void UseItem() override;

	uint8 GetBulletQuantity() const { return BulletQuantity; }

protected:
	void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	uint8 BulletQuantity = 40;

private:
	
};
