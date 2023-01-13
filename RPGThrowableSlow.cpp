// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGThrowableSlow.h"

#include "RPGCharacter.h"
#include "RPGNPC.h"
#include "GameFramework/Character.h"

ARPGThrowableSlow::ARPGThrowableSlow()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SetItemName("Throwable Slow");
	SetThrowableType(ERPGThrowableType::Slow);
	SetRemainingUses(1);
	SetDamage(0.f);
	
	this->Tags.Add("Slow");
}

void ARPGThrowableSlow::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARPGThrowableSlow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARPGThrowableSlow::ThrowableThrow(ACharacter* Target, ACharacter* Attacker)
{
	if (Target->ActorHasTag("NPC"))
	{
		ARPGNPC* TargetNPC = Cast<ARPGNPC>(Target);
		TargetNPC->NPCTakeDamage(1, GetDamage(), Attacker);
		TargetNPC->NPCStartThrowableSlow(GetSlowDuration());
	}
	else if (Target->ActorHasTag("Player"))
	{
		ARPGCharacter* PlayerTarget = Cast<ARPGCharacter>(Target);
		PlayerTarget->CharacterTakeDamage(1, GetDamage());
		PlayerTarget->StartThrowableSlow(GetSlowDuration());
	}
}
