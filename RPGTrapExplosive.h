// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGTrap.h"
#include "RPGTrapExplosive.generated.h"

UCLASS()
class RPG_API ARPGTrapExplosive : public ARPGTrap
{
	GENERATED_BODY()

public:
	ARPGTrapExplosive();

protected:
	virtual void BeginPlay() override;

	virtual void TrapEffect(TArray<AActor*> Targets) override;

private:

	
};
