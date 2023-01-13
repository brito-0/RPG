// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGWeapon.h"
#include "RPGNPC.h"
#include "RPGCharacter.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ARPGWeapon::ARPGWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CollisionCapsule->SetCollisionProfileName(TEXT("DroppedItem"));
	CollisionCapsule->SetCapsuleHalfHeight(CapsuleHeight);
	CollisionCapsule->SetCapsuleRadius(CapsuleRadius);
	// CollisionCapsule->SetHiddenInGame(false);
	SetRootComponent(CollisionCapsule);

	this->Tags.Add("Weapon");
}

// Called when the game starts or when spawned
void ARPGWeapon::BeginPlay()
{
	Super::BeginPlay();

	CurrentBulletAmount = TotalBulletAmount;

	if (GetWeaponType() == ERPGWeaponType::Ranged && Range != 0) Range = 0.f;
	else if (GetWeaponType() == ERPGWeaponType::Melee && Range == 0) Range = 150.f;
}

// Called every frame
void ARPGWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARPGWeapon::WeaponShoot(ARPGNPC* Target, uint8 BodyPart, bool CloseToTarget, ACharacter* Attacker)
{
	// npc take damage - use weapons damage value

	// switch (ShotAmount)
	// {
	// default: {}
	// case 0: // 1 shot
	// 	{
	// 		Target->NPCTakeDamage(BodyPart, GetWeaponDamage(), Attacker);
	// 		break;
	// 	}
	// case 1: // 3 burst
	// 	{
	// 		for (uint8 i = 0; i < 3; i++) Target->NPCTakeDamage(BodyPart, GetWeaponDamage(), Attacker);
	// 		break;
	// 	}
	// case 2: // 5 burst
	// 	{
	// 		for (uint8 i = 0; i < 5; i++) Target->NPCTakeDamage(BodyPart, GetWeaponDamage(), Attacker);
	// 		break;
	// 	}
	// }

	float DamageDealt = GetWeaponDamage();
	if (CloseToTarget) DamageDealt *= 2;

	for (uint8 i = 0; i < ShotAmount; i++) Target->NPCTakeDamage(BodyPart, DamageDealt, Attacker);
}

void ARPGWeapon::NPCWeaponShoot(ACharacter* Target, uint8 BodyPart, bool CloseToTarget, ACharacter* Attacker)
{
	if (Target->ActorHasTag("Player"))
	{
		float DamageDealt = GetWeaponDamage();
		if (CloseToTarget) DamageDealt *= 2;
		
		for (uint8 i = 0; i < ShotAmount; i++) Cast<ARPGCharacter>(Target)->CharacterTakeDamage(BodyPart, DamageDealt);
	}
	else if (Target->ActorHasTag("NPC"))
	{
		float DamageDealt = GetWeaponDamage();
		if (CloseToTarget) DamageDealt *= 2;
		
		for (uint8 i = 0; i < ShotAmount; i++) Cast<ARPGNPC>(Target)->NPCTakeDamage(BodyPart, DamageDealt, Attacker);
	}
}



void ARPGWeapon::SetDroppedWeapon(bool Value)
{
	if (Value)
	{
		DroppedWeapon = Value;
		GetWorldTimerManager().ClearTimer(DroppedWeaponHandle);
		GetWorldTimerManager().SetTimer(DroppedWeaponHandle, this, &ARPGWeapon::DestroyWeapon, DroppedWeaponDuration, false);
		return;
	}

	GetWorldTimerManager().ClearTimer(DroppedWeaponHandle);
	DroppedWeapon = Value;
}
