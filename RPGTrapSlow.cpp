// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGTrapSlow.h"
#include "RPGCharacter.h"
#include "RPGNPC.h"

ARPGTrapSlow::ARPGTrapSlow()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SetItemName("Trap Slow");
	SetTrapType(ERPGTrapType::Slow);
	SetTrapDamage(0.f);
	SetTrapRange(GetTrapRange());
	SetTrapEffectRange(200.f);
	SetTrapEffectDuration(5.f);
	SetTrapDuration(GetTrapDuration());
	SetTrapDelay(GetTrapDelay());
	
	this->Tags.Add("Slow");
}

void ARPGTrapSlow::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARPGTrapSlow::TrapEffect(TArray<AActor*> Targets)
{
	for (AActor* Target : Targets)
	{
		if (Target)
		{
			if (Target->ActorHasTag("NPC"))
			{
				ARPGNPC* TargetNPC = Cast<ARPGNPC>(Target);
				TargetNPC->NPCTakeDamage(1, GetTrapDamage(), nullptr);
				TargetNPC->NPCStartThrowableSlow(GetTrapEffectDuration());
			}
			else if (Target->ActorHasTag("Player"))
			{
				ARPGCharacter* PlayerTarget = Cast<ARPGCharacter>(Target);
				PlayerTarget->CharacterTakeDamage(1, GetTrapDamage());
				PlayerTarget->StartThrowableSlow(GetTrapEffectDuration());
			}
		}
	}
	ARPGTrap::DestroyItem();
}
