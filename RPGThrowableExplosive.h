// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGThrowable.h"
#include "RPGThrowableExplosive.generated.h"

UCLASS()
class RPG_API ARPGThrowableExplosive : public ARPGThrowable
{
	GENERATED_BODY()
	
public:
	ARPGThrowableExplosive();
	
	virtual void ThrowableThrow(ACharacter* Target, ACharacter* Attacker) override;
	
protected:
	void BeginPlay() override;

private:
	void DealDamage(ACharacter* Target, ACharacter* Attacker);
	
};
