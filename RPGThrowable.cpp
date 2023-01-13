// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGThrowable.h"
#include "Kismet/GameplayStatics.h"
#include "RPGCharacter.h"

ARPGThrowable::ARPGThrowable()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SetItemName("Throwable");
	SetItemType(ERPGItemType::Throwable);
	
	this->Tags.Add("Throwable");
}

void ARPGThrowable::BeginPlay()
{
	Super::BeginPlay();
}

void ARPGThrowable::UseItem()
{
	// set as character throwable item

	Cast<ARPGCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->ThrowableEquip(this);
}

bool ARPGThrowable::DecreaseRemainingUses()
{
	--RemainingUses;

	if (RemainingUses == 0)
	{
		DestroyItem();
		return true;
	}
	return false;
}

void ARPGThrowable::ThrowableThrow(ACharacter* Target, ACharacter* Attacker)
{
	
}
