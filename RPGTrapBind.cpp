// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGTrapBind.h"
#include "RPGCharacter.h"
#include "RPGNPC.h"

ARPGTrapBind::ARPGTrapBind()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SetItemName("Trap Bind");
	SetTrapType(ERPGTrapType::Bind);
	SetTrapDamage(0.f);
	SetTrapRange(GetTrapRange());
	SetTrapEffectRange(150.f);
	SetTrapEffectDuration(5.f);
	SetTrapDuration(GetTrapDuration());
	SetTrapDelay(GetTrapDelay());
	
	this->Tags.Add("Bind");
}

void ARPGTrapBind::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARPGTrapBind::TrapEffect(TArray<AActor*> Targets)
{
	for (AActor* Target : Targets)
	{
		if (Target)
		{
			if (Target->ActorHasTag("NPC"))
			{
				ARPGNPC* NPCTarget = Cast<ARPGNPC>(Target);
				NPCTarget->NPCSetBound(GetTrapEffectDuration());
			}
			else if (Target->ActorHasTag("Player"))
			{
				ARPGCharacter* PlayerTarget = Cast<ARPGCharacter>(Target);
				PlayerTarget->CharacterSetBound(GetTrapEffectDuration());
			}
		}
	}
	ARPGTrap::DestroyItem();
}
