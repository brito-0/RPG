// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGThrowable.h"
#include "RPGThrowableIncendiary.generated.h"

class ACharacter;

UCLASS()
class RPG_API ARPGThrowableIncendiary : public ARPGThrowable
{
	GENERATED_BODY()
		
public:
	ARPGThrowableIncendiary();

	virtual void Tick(float DeltaTime) override;
	
	virtual void ThrowableThrow(ACharacter* Target, ACharacter* Attacker) override;

	virtual bool DecreaseRemainingUses() override;
	
protected:
	void BeginPlay() override;

private:
	void DealDamage();

	UPROPERTY()
	ACharacter* IncendiaryTarget;
	UPROPERTY()
	ACharacter* IncendiaryAttacker;

	bool Burning = false;
	bool SetToDestroy = false;
	
	void StartBurning();
	void StopBurning();
	
	float BurnDuration = 3.f;

	uint8 Ticks = 25;
	
};
