// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGNPCTrapper.h"

#include "RPGCharacter.h"
#include "RPGInventory.h"
#include "RPGThrowable.h"
#include "RPGTrap.h"
#include "RPGWeapon.h"

ARPGNPCTrapper::ARPGNPCTrapper()
{
	PrimaryActorTick.bCanEverTick = true;

	SetNPCType(ERPGNPCType::Aggressive);
	SetNPCClass(ERPGNPCClass::Trapper);
	SetNPCMaxHealth(150.f);
	SetNPCLArmMaxHealth(75.f);
	SetNPCRArmMaxHealth(75.f);
	SetNPCLLegMaxHealth(75.f);
	SetNPCRLegMaxHealth(75.f);
	SetNPCShotDelay(.5f);
	SetNPCWalkSpeed(350.f);
	SetNPCCrouchSpeed(192.f);
	SetNPCStrength(1.f);
	SetNPCAccuracy(.50);
	SetNPCMinKillExp(32.5);
	SetNPCMaxKillExp(35.5);
	// melee

	SetActorTickInterval(1.f);
	
	this->Tags.Add("Trapper");
}

void ARPGNPCTrapper::BeginPlay()
{
	Super::BeginPlay();
	
	NPCController = Cast<AAIController>(GetController());

	NPCInitPos = GetActorLocation();

	SpawnTraps();
	
}

void ARPGNPCTrapper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// check distance to player
	// when player is ~1700 units away move to a location (location of one of the traps, add player forward vector times ~700)

	// if player move away from the trapper and the distance is greater than ~2000 return to the initial position

	// move towards the trap furthest from the player

	if (GetNPCState() == ERPGNPCState::Normal)
	{
		if (GetDistanceTo(GetPlayerCharacter()) <= 1550.f)
		{
			if (NPCMoved) return;
			
			FVector MoveLoc;

			if (Trap0->GetDistanceTo(GetPlayerCharacter()) >= Trap1->GetDistanceTo(GetPlayerCharacter()))
				MoveLoc = Trap0Pos + GetPlayerCharacter()->GetActorForwardVector() * MoveDistance;
			else
				MoveLoc = Trap1Pos + GetPlayerCharacter()->GetActorForwardVector() * MoveDistance;

			NPCController->MoveToLocation(MoveLoc);
			NPCMoved = true;
		}
		else if (GetDistanceTo(GetPlayerCharacter()) >= 2250.f)
		{
			if (!NPCMoved) return;
			
			NPCController->MoveToLocation(NPCInitPos);
			NPCMoved = false;
		}
	}
	
}

void ARPGNPCTrapper::NPCCombatAction()
{
	// if a trap explodes, the player tries to interact or attack the trapper run towards the player and attack

	
}

bool ARPGNPCTrapper::WeaponCheckPriority(ARPGWeapon* Weapon)
{
	// only pick up if melee
	if (Weapon->GetWeaponType() == ERPGWeaponType::Ranged) return false;
	
	if (!GetNPCWeapon())
	{
		// pick up
		NPCWeaponPickUp(Weapon);
		return true;
	}
	if (Weapon->GetWeaponDamage() < GetNPCWeapon()->GetWeaponDamage()) return false;
	// pick up
	NPCWeaponPickUp(Weapon);
	return true;
}

bool ARPGNPCTrapper::ThrowableCheckPriority(ARPGThrowable* Throwable)
{
	// do nothing
	return false;
}

bool ARPGNPCTrapper::HeadArmourCheckPriority(ARPGArmour* HArmour)
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

bool ARPGNPCTrapper::ChestArmourCheckPriority(ARPGArmour* CArmour)
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

bool ARPGNPCTrapper::LegsArmourCheckPriority(ARPGArmour* LArmour)
{
	if (!GetNPCLegsArmour())
	{
		// pick up
		NPCArmourPickUp(LArmour);
		return true;
	}
	if (GetNPCLegsArmour()->GetArmourDefense() >= LArmour->GetArmourDefense()) return false;
	// pick up
	NPCArmourPickUp(LArmour);
	return true;
}

void ARPGNPCTrapper::GenerateTrapPosition()
{
	// generate position for each trap and set it to the respective variable

	// TrapMinDistance
	// TrapMaxDistance


	// get a random location around the npc to place the first trap
	// use the location of the first trap to get the location of the second
	

	const float Trap0Angle = FMath::RandRange(0.f, 360.f);

	Trap0Pos = GetActorLocation() + FRotator(0.f, Trap0Angle, 0.f).Vector() * FMath::RandRange(TrapMaxDistance - TrapMinDistance, TrapMaxDistance);

	const float Trap1Value = Trap0Angle + 180.f;
	
	const float Trap1Angle = FMath::RandRange(Trap1Value - 90.f, Trap1Value + 90.f);

	Trap1Pos = GetActorLocation() + FRotator(0.f, Trap1Angle, 0.f).Vector() * FMath::RandRange(TrapMinDistance, TrapMaxDistance - TrapMinDistance);
}


void ARPGNPCTrapper::SpawnTraps()
{
	if (!Trap0Object || !Trap1Object) this->Destroy();

	FRotator DropRot = FRotator(0.f, 0.f, 0.f);

	GenerateTrapPosition();
	
	// spawn traps in a random location around the npc
	Trap0 = GetWorld()->SpawnActor<ARPGTrap>(Trap0Object, Trap0Pos, DropRot);
	Trap0->NPCUseItem(this, Trap0Pos);
	Trap1 = GetWorld()->SpawnActor<ARPGTrap>(Trap1Object, Trap1Pos, DropRot);
	Trap1->NPCUseItem(this, Trap1Pos);
	
}

void ARPGNPCTrapper::DetonateAllTraps()
{
	if (Trap0 && Trap0->GetTrapActivated()) Trap0->DetonateTrap();
	if (Trap1 && Trap1->GetTrapActivated()) Trap1->DetonateTrap();
}

void ARPGNPCTrapper::DeathAction()
{
	DetonateAllTraps();
}

void ARPGNPCTrapper::TrapDetonationAlert(AActor* Character)
{
	// attack character affected by the trap
}
