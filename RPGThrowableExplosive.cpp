// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGThrowableExplosive.h"
#include "RPGCharacter.h"
#include "RPGNPC.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

ARPGThrowableExplosive::ARPGThrowableExplosive()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SetItemName("Throwable Explosive");
	SetThrowableType(ERPGThrowableType::Explosive);
	SetRemainingUses(2);
	SetDamage(15.f);
	
	
	this->Tags.Add("Explosive");
}

void ARPGThrowableExplosive::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARPGThrowableExplosive::ThrowableThrow(ACharacter* Target, ACharacter* Attacker)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType((ECollisionChannel::ECC_WorldDynamic)));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType((ECollisionChannel::ECC_PhysicsBody)));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType((ECollisionChannel::ECC_Pawn)));
	const TArray<AActor*> IgnoreActor = {Target};
	TArray<AActor*> OutActors;

	UKismetSystemLibrary::DrawDebugSphere(GetWorld(), Target->GetActorLocation(), 250.f, 12, FColor::Red, 2.f, 1.f);
	
	if (UKismetSystemLibrary::SphereOverlapActors(GetWorld(), Target->GetActorLocation(), 250.f,
				ObjectTypes, ACharacter::StaticClass(), IgnoreActor, OutActors))
	{
		for (AActor* Actor : OutActors)
		{
			if (Actor)
			{
				DealDamage(Cast<ACharacter>(Actor), Attacker);
			}
		}
	}
	DealDamage(Target, Attacker);
}

void ARPGThrowableExplosive::DealDamage(ACharacter* Target, ACharacter* Attacker)
{
	if (Target->ActorHasTag("NPC"))
	{
		ARPGNPC* NPCTarget = Cast<ARPGNPC>(Target);
		for (uint8 i = 1; i < 6; i++) NPCTarget->NPCTakeDamage(i, GetDamage(), Attacker);
		// NPCTarget->NPCTakeDamage(1, GetDamage(), Attacker);
		// NPCTarget->NPCTakeDamage(2, GetDamage(), Attacker);
		// NPCTarget->NPCTakeDamage(3, GetDamage(), Attacker);
		// NPCTarget->NPCTakeDamage(4, GetDamage(), Attacker);
		// NPCTarget->NPCTakeDamage(5, GetDamage(), Attacker);
		if (Attacker->ActorHasTag("Player")) Cast<ARPGCharacter>(Attacker)->CheckTargetDead(NPCTarget);
	}
	else if (Target->ActorHasTag("Player"))
	{
		ARPGCharacter* PlayerTarget = Cast<ARPGCharacter>(Target);
		for (uint8 i = 1; i < 6; i++) PlayerTarget->CharacterTakeDamage(i, GetDamage());
		// PlayerTarget->CharacterTakeDamage(1, GetDamage());
		// PlayerTarget->CharacterTakeDamage(2, GetDamage());
		// PlayerTarget->CharacterTakeDamage(3, GetDamage());
		// PlayerTarget->CharacterTakeDamage(4, GetDamage());
		// PlayerTarget->CharacterTakeDamage(5, GetDamage());
	}
}
