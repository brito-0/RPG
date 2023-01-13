// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGTrap.h"
#include "RPGTrapIncendiary.generated.h"

UCLASS()
class RPG_API ARPGTrapIncendiary : public ARPGTrap
{
	GENERATED_BODY()

public:
	ARPGTrapIncendiary();

	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

	virtual void TrapEffect(TArray<AActor*> Targets) override;

private:
	bool Burning = false;
	
	uint8 Ticks = 25;

	UPROPERTY()
	TArray<AActor*> IncendiaryTargets;
	
	void DealDamage();
	
	void StartBurning();

	void StopBurning();
};
