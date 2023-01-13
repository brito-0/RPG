// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGNPCRush.h"
#include "RPGInventory.h"
#include "RPGThrowable.h"
#include "RPGWeapon.h"

ARPGNPCRush::ARPGNPCRush()
{
	PrimaryActorTick.bCanEverTick = true;

	SetNPCType(ERPGNPCType::Aggressive);
	SetNPCClass(ERPGNPCClass::Rush);
	SetNPCMaxHealth(300.f);
	SetNPCLArmMaxHealth(150.f);
	SetNPCRArmMaxHealth(150.f);
	SetNPCLLegMaxHealth(150.f);
	SetNPCRLegMaxHealth(150.f);
	SetNPCShotDelay(.2f);
	SetNPCWalkSpeed(335.f);
	SetNPCCrouchSpeed(190.f);
	SetNPCStrength(1.5f);
	SetNPCAccuracy(.05);
	SetNPCMinKillExp(30.5);
	SetNPCMaxKillExp(33.5);
	// high damage weapon with low accuracy

	SetActorTickInterval(1.f);
	
	this->Tags.Add("Rush");
}

void ARPGNPCRush::BeginPlay()
{
	Super::BeginPlay();

	NPCController = Cast<AAIController>(GetController());
}

void ARPGNPCRush::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetNPCTargetEnemy())
	{
		if (IsChasing && !GetMoveSpeedDebuff())
		{
			SpeedInc += SpeedIncValue;
			NPCChangeMoveSpeed(SpeedInc);
		}
	}
}

void ARPGNPCRush::NPCCombatAction()
{
	// check distance to target
	// if the target is farther than 1000 throw throwable behind them
	// run towards the target
	// when the distance is around 100 shoot the target for the bonus damage
	
	if (GetNPCState() != ERPGNPCState::Combat) return;

	if (ThrowTime == 0) ThrowTime = FMath::RandRange(2, 16);
	
	++ThrowTimeCount;

	// figure a way to alert the target without the stagger
	
	if (!NPCAlreadyAttacked)
	{
		NPCAlreadyAttacked = true;
		GetNPCWeapon()->NPCWeaponShoot(GetNPCTargetEnemy(), 6.f, false, this);
	}
	
	CurrentDistanceToEnemy = GetDistanceTo(GetNPCTargetEnemy());
	// if npc outside acceptable range change to search state

	if (ThrowTimeCount >= ThrowTime && CurrentDistanceToEnemy >= 450.f)
	{
		ChangeNPCStateCombat();
		NPCThrow();
		ThrowTime = 0;
		ThrowTimeCount = 0;
		return;
	}

	if (CurrentDistanceToEnemy < 100.f) NPCAttack(0);
	else
	{
		ChangeNPCStateCombat();
		IsChasing = true;
		MoveToEnemy();
		FTimerHandle CombatActionHandle;
		GetWorldTimerManager().SetTimer(CombatActionHandle, this, &ARPGNPCRush::NPCCombatAction, .5f, false);
	}
}

bool ARPGNPCRush::WeaponCheckPriority(ARPGWeapon* Weapon)
{
	if (Weapon->GetWeaponType() == ERPGWeaponType::Melee) return false;
	
	if (!GetNPCWeapon())
	{
		// pick up
		NPCWeaponPickUp(Weapon);
		return true;
	}
	if (GetNPCWeapon()->GetWeaponDamage() >= Weapon->GetWeaponDamage()) return false;
	// pick up
	NPCWeaponPickUp(Weapon);
	return true;
}

bool ARPGNPCRush::ThrowableCheckPriority(ARPGThrowable* Throwable)
{
	if (Throwable->GetThrowableType() != ERPGThrowableType::Slow) return false;
	if (!GetNPCThrowable())
	{
		// pick up
		NPCThrowablePickUp(Throwable);
		return true;
	}
	if (NPCCountInventory("Throwable") >= 3 /**|| GetNPCInventory()->GetInventoryItemNumber() == GetNPCInventory()->GetInventorySize()*/) return false;
	// pick up
	NPCThrowablePickUp(Throwable);
	return true;
}

bool ARPGNPCRush::HeadArmourCheckPriority(ARPGArmour* HArmour)
{
	if (!GetNPCHeadArmour())
	{
		// pick up
		NPCArmourPickUp(HArmour);
		return true;
	}
	if (GetNPCHeadArmour()->GetArmourDefense() >= HArmour->GetArmourDefense()) return false;
	// pick up
	NPCArmourPickUp(HArmour);
	return true;
}

bool ARPGNPCRush::ChestArmourCheckPriority(ARPGArmour* CArmour)
{
	if (!GetNPCChestArmour())
	{
		// pick up
		NPCArmourPickUp(CArmour);
		return true;
	}
	if (GetNPCChestArmour()->GetArmourDefense() >= CArmour->GetArmourDefense()) return false;
	// pick up
	NPCArmourPickUp(CArmour);
	return true;
}

bool ARPGNPCRush::LegsArmourCheckPriority(ARPGArmour* LArmour)
{
	if (!GetNPCLegsArmour())
	{
		// pick up
		NPCArmourPickUp(LArmour);
		return true;
	}
	if (GetNPCLegsArmour()->GetArmourCurrentDurability() >= LArmour->GetArmourCurrentDurability()) return false;
	// pick up
	NPCArmourPickUp(LArmour);
	return true;
}
