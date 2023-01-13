// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGNPCShield.h"

ARPGNPCShield::ARPGNPCShield()
{
	PrimaryActorTick.bCanEverTick = false;

	SetNPCType(ERPGNPCType::Shield);
	SetNPCClass(ERPGNPCClass::Shield);
	SetNPCMaxHealth(50.f);
	SetNPCLArmMaxHealth(50.f);
	SetNPCRArmMaxHealth(50.f);
	SetNPCLLegMaxHealth(50.f);
	SetNPCRLegMaxHealth(50.f);
	SetNPCShotDelay(60.f);
	SetNPCWalkSpeed(0.f);
	SetNPCCrouchSpeed(0.f);
	SetNPCStrength(0.f);
	SetNPCAccuracy(.00);
	SetNPCMinKillExp(9.5);
	SetNPCMaxKillExp(11.5);
	
	SetActorTickInterval(1.f);
	
	this->Tags.Add("Shield");
}

void ARPGNPCShield::BeginPlay()
{
	Super::BeginPlay();

	ChangeNPCStateStationary();
}

void ARPGNPCShield::StartDestroyTimer(float Duration)
{
	FTimerHandle ShieldDestroyHandle;
	GetWorldTimerManager().SetTimer(ShieldDestroyHandle, this, &ARPGNPCShield::ChangeNPCStateDead, Duration, false);
}
