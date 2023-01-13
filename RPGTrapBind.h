// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGTrap.h"
#include "RPGTrapBind.generated.h"

UCLASS()
class RPG_API ARPGTrapBind : public ARPGTrap
{
	GENERATED_BODY()

public:
	ARPGTrapBind();

protected:
	virtual void BeginPlay() override;

	virtual void TrapEffect(TArray<AActor*> Targets) override;

private:

	
};
