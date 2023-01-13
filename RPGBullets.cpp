// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGBullets.h"
#include "Kismet/GameplayStatics.h"
#include "RPGCharacter.h"

ARPGBullets::ARPGBullets()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SetItemName("Bullets");
	SetItemType(ERPGItemType::Bullets);
	
	this->Tags.Add("Bullets");
}

void ARPGBullets::BeginPlay()
{
	Super::BeginPlay();
}

void ARPGBullets::UseItem()
{
	// send them to weapon
}
