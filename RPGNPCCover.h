// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGNPC.h"
#include "RPGNPCCover.generated.h"

class ARPGNPCShield;

UCLASS()
class RPG_API ARPGNPCCover : public ARPGNPC
{
	GENERATED_BODY()
	
public:
	ARPGNPCCover();

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
	UPROPERTY(VisibleAnywhere)
	bool ShieldPlaced = false;

	FVector ShieldPosition;

	UPROPERTY(VisibleAnywhere)
	ARPGNPCShield* ThrownShield;

	bool BarricadeUsed = false;

	void NPCShieldBarricade();

	// void NPCShieldBarricadeDestroy();

	UPROPERTY(EditAnywhere, Category = Shield)
	TSubclassOf<ARPGNPCShield> NPCShieldObject = nullptr;
	// UPROPERTY(VisibleAnywhere, Category = Shield)
	// ARPGNPCShield* NPCShield = nullptr;
	UPROPERTY()
	TArray<ARPGNPCShield*> NPCShields;
	
	UPROPERTY(VisibleAnywhere)
	uint8 ThrowTime = 0;
	UPROPERTY(VisibleAnywhere)
	uint8 ThrowTimeCount = 0;

	void FindShield();

	void CheckThrownShield();

	void NPCAttackArms();

	void NPCCoverAttack();

	UPROPERTY(VisibleAnywhere)
	bool IsRelocating = false;

	void NPCMoveBehindShield();

	void NPCStopRelocating();
};
