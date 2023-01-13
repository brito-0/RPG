// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGTrapIncendiary.h"
#include "RPGCharacter.h"
#include "RPGNPC.h"

ARPGTrapIncendiary::ARPGTrapIncendiary()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SetItemName("Trap Incendiary");
	SetTrapType(ERPGTrapType::Incendiary);
	SetTrapDamage(.3f);
	SetTrapRange(GetTrapRange());
	SetTrapEffectRange(150.f);
	SetTrapEffectDuration(3.f);
	SetTrapDuration(GetTrapDuration());
	SetTrapDelay(GetTrapDelay());
	
	this->Tags.Add("Incendiary");
}

void ARPGTrapIncendiary::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARPGTrapIncendiary::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!IncendiaryTargets.IsEmpty() && Burning)
	{
		DealDamage();
	}
	
}

void ARPGTrapIncendiary::TrapEffect(TArray<AActor*> Targets)
{
	DestroyDecal();
	
	IncendiaryTargets = Targets;

	StartBurning();

	FTimerHandle StopBurningHandle;
	GetWorldTimerManager().SetTimer(StopBurningHandle, this, &ARPGTrapIncendiary::StopBurning, GetTrapEffectDuration(), false);
}

void ARPGTrapIncendiary::DealDamage()
{
	for (AActor* Target : IncendiaryTargets)
	{
		if (Target)
		{
			if (Target->ActorHasTag("NPC"))
			{
				ARPGNPC* NPCTarget = Cast<ARPGNPC>(Target);
				for (uint8 i = 1; i < 6; i++) NPCTarget->NPCTakeDamage(i, GetTrapDamage(), nullptr);
			}
			else if (Target->ActorHasTag("Player"))
			{
				ARPGCharacter* PlayerTarget = Cast<ARPGCharacter>(Target);
				for (uint8 i = 1; i < 6; i++) PlayerTarget->CharacterTakeDamage(i, GetTrapDamage());
			}
		}
	}
}

void ARPGTrapIncendiary::StartBurning()
{
	Burning = true;

	SetActorTickInterval(GetTrapEffectDuration()/Ticks);
}

void ARPGTrapIncendiary::StopBurning()
{
	Burning = false;

	ARPGTrap::DestroyItem();
}
