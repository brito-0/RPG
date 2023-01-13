// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGTrapExplosive.h"
#include "RPGCharacter.h"
#include "RPGNPC.h"

ARPGTrapExplosive::ARPGTrapExplosive()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SetItemName("Trap Explosive");
	SetTrapType(ERPGTrapType::Explosive);
	SetTrapDamage(15.f);
	SetTrapRange(GetTrapRange());
	SetTrapEffectRange(250.f);
	SetTrapEffectDuration(0.f);
	SetTrapDuration(GetTrapDuration());
	SetTrapDelay(GetTrapDelay());
	
	this->Tags.Add("Explosive");
}

void ARPGTrapExplosive::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARPGTrapExplosive::TrapEffect(TArray<AActor*> Targets)
{
	for (AActor* Target : Targets)
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
	ARPGTrap::DestroyItem();
}
