// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGNPC.h"
#include "RPGNPCRush.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ARPGNPCRush : public ARPGNPC
{
	GENERATED_BODY()

public:
	ARPGNPCRush();

	virtual void Tick(float DeltaTime) override;

protected:
	void BeginPlay() override;

	virtual void NPCCombatAction();

	virtual bool WeaponCheckPriority(ARPGWeapon* Weapon) override;
	virtual bool ThrowableCheckPriority(ARPGThrowable* Throwable) override;
	virtual bool HeadArmourCheckPriority(ARPGArmour* HArmour) override;
	virtual bool ChestArmourCheckPriority(ARPGArmour* CArmour) override;
	virtual bool LegsArmourCheckPriority(ARPGArmour* LArmour) override;
	// virtual bool ItemsCheckPriority(ARPGItem* Item) override;

private:
	bool IsChasing = false;

	float SpeedIncValue = .01f;

	UPROPERTY(VisibleAnywhere)
	uint8 ThrowTime = 0;
	UPROPERTY(VisibleAnywhere)
	uint8 ThrowTimeCount = 0;
	
};
