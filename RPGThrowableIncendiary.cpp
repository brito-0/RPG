// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGThrowableIncendiary.h"

#include "RPGCharacter.h"
#include "RPGNPC.h"

ARPGThrowableIncendiary::ARPGThrowableIncendiary()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SetItemName("Throwable Incendiary");
	SetThrowableType(ERPGThrowableType::Incendiary);
	SetRemainingUses(1);
	SetDamage(.3f);

	SetActorTickInterval(BurnDuration/Ticks);
	
	this->Tags.Add("Incendiary");
}

void ARPGThrowableIncendiary::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);
	
}

void ARPGThrowableIncendiary::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IncendiaryTarget && Burning)
	{
		DealDamage();
		if (IncendiaryAttacker->ActorHasTag("Player"))
		{
			if (Cast<ARPGCharacter>(IncendiaryAttacker)->CheckTargetDead(Cast<ARPGNPC>(IncendiaryTarget))) Burning = false;
		}
	}
}

void ARPGThrowableIncendiary::ThrowableThrow(ACharacter* Target, ACharacter* Attacker)
{
	IncendiaryTarget = Target;
	IncendiaryAttacker = Attacker;

	StartBurning();

	FTimerHandle StopBurningHandle;
	GetWorldTimerManager().SetTimer(StopBurningHandle, this, &ARPGThrowableIncendiary::StopBurning, BurnDuration, false);
}

void ARPGThrowableIncendiary::DealDamage()
{
	if (IncendiaryTarget->ActorHasTag("NPC"))
	{
		ARPGNPC* NPCTarget = Cast<ARPGNPC>(IncendiaryTarget);
		for (uint8 i = 1; i < 6; i++) NPCTarget->NPCTakeDamage(i, GetDamage(), IncendiaryAttacker);
	}
	else if (IncendiaryTarget->ActorHasTag("Player"))
	{
		ARPGCharacter* PlayerTarget = Cast<ARPGCharacter>(IncendiaryTarget);
		for (uint8 i = 1; i < 6; i++) PlayerTarget->CharacterTakeDamage(i, GetDamage());
	}
}

bool ARPGThrowableIncendiary::DecreaseRemainingUses()
{
	--RemainingUses;

	if (RemainingUses == 0)
	{
		SetToDestroy = true;
		return true;
	}
	return false;
}


void ARPGThrowableIncendiary::StartBurning()
{
	Burning = true;
	SetActorTickEnabled(true);
}

void ARPGThrowableIncendiary::StopBurning()
{
	IncendiaryTarget = nullptr;
	IncendiaryAttacker = nullptr;
	Burning = false;
	SetActorTickEnabled(false);

	if (SetToDestroy) DestroyItem();
}
