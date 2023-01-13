// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGNPC.h"
#include "RPGNPCTrapper.generated.h"

class ARPGTrap;

/**
 * 
 */
UCLASS()
class RPG_API ARPGNPCTrapper : public ARPGNPC
{
	GENERATED_BODY()

public:
	ARPGNPCTrapper();

	virtual void Tick(float DeltaTime) override;

	void TrapDetonationAlert(AActor* Character);

protected:
	void BeginPlay() override;

	virtual void NPCCombatAction();

	virtual bool WeaponCheckPriority(ARPGWeapon* Weapon) override;
	virtual bool ThrowableCheckPriority(ARPGThrowable* Throwable) override;
	virtual bool HeadArmourCheckPriority(ARPGArmour* HArmour) override;
	virtual bool ChestArmourCheckPriority(ARPGArmour* CArmour) override;
	virtual bool LegsArmourCheckPriority(ARPGArmour* LArmour) override;
	// virtual bool ItemsCheckPriority(ARPGItem* Item) override;

	virtual void DeathAction() override;

private:
	float TrapZ = 44.f;

	UPROPERTY(EditAnywhere)
	float MoveDistance = 350.f;

	bool NPCMoved = false;
	
	UPROPERTY(EditAnywhere, Category = Trap)
	TSubclassOf<ARPGTrap> Trap0Object = nullptr;
	UPROPERTY(VisibleAnywhere, Category = Trap)
	ARPGTrap* Trap0;
	UPROPERTY(EditAnywhere, Category = Trap)
	TSubclassOf<ARPGTrap> Trap1Object = nullptr;
	UPROPERTY(VisibleAnywhere, Category = Trap)
	ARPGTrap* Trap1;

 	FVector NPCInitPos;

	FVector Trap0Pos;
	FVector Trap1Pos;

	UPROPERTY(EditAnywhere)
	float TrapMinDistance = 150.f;
	UPROPERTY(EditAnywhere)
	float TrapMaxDistance = 650.f;
	void GenerateTrapPosition();
	
	void SpawnTraps();
	void DetonateAllTraps();
	
};
