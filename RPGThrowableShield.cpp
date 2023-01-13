// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGThrowableShield.h"

#include "RPGNPCShield.h"

ARPGThrowableShield::ARPGThrowableShield()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SetItemName("Throwable Shield");
	SetThrowableType(ERPGThrowableType::Shield);
	SetRemainingUses(1);
	
	
	this->Tags.Add("Shield");
}

void ARPGThrowableShield::BeginPlay()
{
	Super::BeginPlay();

	// if (NPCShieldObject)
	// {
	// 	NPCShield = GetWorld()->SpawnActor<ARPGNPCShield>(NPCShieldObject);
	// 	NPCShield->SetActorHiddenInGame(true);
	// 	NPCShield->SetActorEnableCollision(false);
	// 	const FAttachmentTransformRules AttachRules(EAttachmentRule::KeepRelative, true);
	// 	NPCShield->AttachToComponent(GetRootComponent(), AttachRules);
	// }
}

void ARPGThrowableShield::ThrowableThrowShield(FVector Position, FVector Direction)
{
	const FVector DropPos = Position + (Direction * 85);
	const FRotator DropRot = FRotator(0.f, 0.f, 0.f);

	// const FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepRelative, true);
	// NPCShield->DetachFromActor(DetachmentRules);
	// NPCShield->SetActorLocation(DropPos);
	// NPCShield->SetActorRotation(FRotator(0.f, 0.f, 0.f));
	// NPCShield->SetActorHiddenInGame(false);
	// NPCShield->SetActorEnableCollision(true);
	// NPCShield->StartDestroyTimer();

	if (NPCShieldObject)
	{
		NPCShield = GetWorld()->SpawnActor<ARPGNPCShield>(NPCShieldObject, DropPos, DropRot);
		NPCShield->StartDestroyTimer();
	}
}
