// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGItem.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ARPGItem::ARPGItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CollisionCapsule->SetCollisionProfileName(TEXT("DroppedItem"));
	CollisionCapsule->SetCapsuleHalfHeight(CapsuleHeight);
	CollisionCapsule->SetCapsuleRadius(CapsuleRadius);
	// CollisionCapsule->SetHiddenInGame(false);
	SetRootComponent(CollisionCapsule);

	SetActorTickInterval(1.f);

	this->Tags.Add("Item");
}

// Called when the game starts or when spawned
void ARPGItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARPGItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARPGItem::UseItem()
{
	// 	
}
void ARPGItem::StartScreen()
{
	//	
}
void ARPGItem::StopScreen()
{
	//	
}
void ARPGItem::ItemUsed()
{
	//
}


void ARPGItem::SetDroppedItem(bool Value)
{
	if (Value)
	{
		DroppedItem = Value;
		GetWorldTimerManager().ClearTimer(DroppedItemHandle);
		GetWorldTimerManager().SetTimer(DroppedItemHandle, this, &ARPGItem::DestroyItem, DroppedItemDuration, false);
		return;
	}

	GetWorldTimerManager().ClearTimer(DroppedItemHandle);
	DroppedItem = Value;
}
