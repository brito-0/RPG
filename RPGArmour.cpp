// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGArmour.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ARPGArmour::ARPGArmour()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CollisionCapsule->SetCollisionProfileName(TEXT("DroppedItem"));
	CollisionCapsule->SetCapsuleHalfHeight(CapsuleHeight);
	CollisionCapsule->SetCapsuleRadius(CapsuleRadius);
	// CollisionCapsule->SetHiddenInGame(false);
	SetRootComponent(CollisionCapsule);

	this->Tags.Add("Armour");
}

// Called when the game starts or when spawned
void ARPGArmour::BeginPlay()
{
	Super::BeginPlay();

	CurrentDurability = GetArmourMaxDurability();
}

// Called every frame
void ARPGArmour::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ARPGArmour::CalcDamage(float DamageIn)
{
	if (CurrentDurability < 1) return DamageIn;
	
	--CurrentDurability;
	if (CurrentDurability < 0) CurrentDurability = 0;

	return DamageIn * (1 - Defense);
}

bool ARPGArmour::RepairArmour()
{
	if (GetArmourCurrentDurability() == GetArmourMaxDurability())
	{
		UE_LOG(LogTemp, Warning, TEXT("ARMOUR ALREADY AT MAX DURABILITY"));
		return false;
	}
	CurrentDurability = GetArmourMaxDurability();
	UE_LOG(LogTemp, Warning, TEXT("ARMOUR REPAIRED"));
	return true;
}

void ARPGArmour::SetDroppedArmour(bool Value)
{
	if (Value)
	{
		DroppedArmour = Value;
		GetWorldTimerManager().ClearTimer(DroppedArmourHandle);
		GetWorldTimerManager().SetTimer(DroppedArmourHandle, this, &ARPGArmour::DestroyArmour, DroppedArmourDuration, false);
		return;
	}

	GetWorldTimerManager().ClearTimer(DroppedArmourHandle);
	DroppedArmour = Value;
}
