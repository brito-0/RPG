// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGBag.h"

#include "RPGArmour.h"
#include "RPGWeapon.h"
#include "RPGThrowable.h"
#include "RPGInventory.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ARPGBag::ARPGBag()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CollisionCapsule->SetCollisionProfileName(TEXT("DroppedItem"));
	CollisionCapsule->SetCapsuleHalfHeight(CapsuleHeight);
	CollisionCapsule->SetCapsuleRadius(CapsuleRadius);
	// CollisionCapsule->SetHiddenInGame(false);
	SetRootComponent(CollisionCapsule);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->SetRelativeLocation(FVector(0.f, 0.f, -25.f));
	StaticMesh->SetRelativeScale3D(FVector(.6f, .6f, .6f));
	StaticMesh->SetCollisionProfileName(TEXT("DroppedItem"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>StaticMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (StaticMeshAsset.Succeeded()) { StaticMesh->SetStaticMesh(StaticMeshAsset.Object); }

	this->Tags.Add("Bag");
}

// Called when the game starts or when spawned
void ARPGBag::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARPGBag::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



// ARPGWeapon* ARPGBag::GetBagWeapon(ARPGWeapon* Weapon) const
// {
// 	if (BagWeapon) return BagWeapon;
// 	return nullptr;
// }

void ARPGBag::SetBagWeapon(ARPGWeapon* Weapon)
{
	if (!Weapon)
	{
		BagWeapon = nullptr;
		return;
	}
	
	BagWeapon = Weapon;
	
	const FAttachmentTransformRules AttachRules(EAttachmentRule::KeepRelative, true);
	GetBagWeapon()->SetActorEnableCollision(false);
	GetBagWeapon()->AttachToComponent(GetRootComponent(), AttachRules, "InventorySocket");
}

void ARPGBag::SetBagThrowable(ARPGThrowable* Throwable)
{
	if (!Throwable)
	{
		BagThrowable = nullptr;
		return;
	}
	
	BagThrowable = Throwable;

	const FAttachmentTransformRules AttachRules(EAttachmentRule::KeepRelative, true);
	GetBagThrowable()->SetActorEnableCollision(false);
	GetBagThrowable()->AttachToComponent(GetRootComponent(), AttachRules, "InventorySocket");
}

void ARPGBag::SetBagHeadArmour(ARPGArmour* HeadArmour)
{
	if (!HeadArmour)
	{
		BagHeadArmour = nullptr;
		return;
	}
	
	if (HeadArmour->GetArmourType() == ERPGArmourType::Head)
	{
		BagHeadArmour = HeadArmour;

		const FAttachmentTransformRules AttachRules(EAttachmentRule::KeepRelative, true);
		GetBagHeadArmour()->SetActorEnableCollision(false);
		GetBagHeadArmour()->AttachToComponent(GetRootComponent(), AttachRules, "InventorySocket");
	}
}

void ARPGBag::SetBagChestArmour(ARPGArmour* ChestArmour)
{
	if (!ChestArmour)
	{
		BagChestArmour = nullptr;
		return;
	}
	
	if (ChestArmour->GetArmourType() == ERPGArmourType::Chest)
	{
		BagChestArmour = ChestArmour;
		
		const FAttachmentTransformRules AttachRules(EAttachmentRule::KeepRelative, true);
		GetBagChestArmour()->SetActorEnableCollision(false);
		GetBagChestArmour()->AttachToComponent(GetRootComponent(), AttachRules, "InventorySocket");
	}
}

void ARPGBag::SetBagLegsArmour(ARPGArmour* LegsArmour)
{
	if (!LegsArmour)
	{
		BagLegsArmour = nullptr;
		return;
	}
	
	if (LegsArmour->GetArmourType() == ERPGArmourType::Legs)
	{
		BagLegsArmour = LegsArmour;

		const FAttachmentTransformRules AttachRules(EAttachmentRule::KeepRelative, true);
		GetBagLegsArmour()->SetActorEnableCollision(false);
		GetBagLegsArmour()->AttachToComponent(GetRootComponent(), AttachRules, "InventorySocket");
	}
}

void ARPGBag::SetBagInventory(ARPGInventory* Inventory)
{
	BagInventory = Inventory;

	const FAttachmentTransformRules AttachRules(EAttachmentRule::KeepRelative, true);
	for (ARPGItem* Item : BagInventory->GetItemInventory())
	{
		if (Item)
		{
			Item->SetActorEnableCollision(false);
			Item->AttachToComponent(GetRootComponent(), AttachRules, "InventorySocket");
		}
	}
}


void ARPGBag::SetDroppedBag()
{
	if (CheckEmpty()) DestroyBag();
	TimerActive = true;
	GetWorldTimerManager().ClearTimer(DestructTimeHandle);
	GetWorldTimerManager().SetTimer(DestructTimeHandle, this, &ARPGBag::DestroyBag, DestructTimeDurationFirst, false);
}

void ARPGBag::BagClosedTimer(bool Value)
{
	if (Value)
	{
		if (CheckEmpty()) DestroyBag();
		TimerActive = Value;
		GetWorldTimerManager().ClearTimer(DestructTimeHandle);
		GetWorldTimerManager().SetTimer(DestructTimeHandle, this, &ARPGBag::DestroyBag, DestructTimeDurationNormal, false);
		return;
	}
	
	GetWorldTimerManager().ClearTimer(DestructTimeHandle);
	TimerActive = Value;
}

bool ARPGBag::CheckEmpty()
{
	if (BagWeapon) return false;
	if (BagThrowable) return false;
	if (BagHeadArmour) return false;
	if (BagChestArmour) return false;
	if (BagLegsArmour) return false;
	for (ARPGItem* Item : BagInventory->GetItemInventory())
	{
		if (Item) return false;
	}
	return true;
}


void ARPGBag::DestroyBag()
{
	if (BagWeapon) BagWeapon->Destroy();
	if (BagThrowable) BagThrowable->Destroy();
	if (BagHeadArmour) BagHeadArmour->Destroy();
	if (BagChestArmour) BagChestArmour->Destroy();
	if (BagLegsArmour) BagLegsArmour->Destroy();
	for (ARPGItem* Item : BagInventory->GetItemInventory())
	{
		if (Item) Item->Destroy();
	}
	BagInventory->Destroy();
	this->Destroy();
}
