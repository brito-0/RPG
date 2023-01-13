// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGTrap.h"
#include "RPGTrapSlow.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ARPGTrapSlow : public ARPGTrap
{
	GENERATED_BODY()

public:
	ARPGTrapSlow();

protected:
	virtual void BeginPlay() override;

	virtual void TrapEffect(TArray<AActor*> Targets) override;

private:

	
};
