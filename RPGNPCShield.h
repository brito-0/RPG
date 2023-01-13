// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGNPC.h"
#include "RPGNPCShield.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ARPGNPCShield : public ARPGNPC
{
	GENERATED_BODY()

public:
	ARPGNPCShield();

	void StartDestroyTimer(float Duration = 300.f);

protected:
	void BeginPlay() override;

private:

	
};
