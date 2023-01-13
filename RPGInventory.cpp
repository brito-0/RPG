// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGInventory.h"
#include "RPGItem.h"

// Sets default values
ARPGInventory::ARPGInventory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
}

// Called when the game starts or when spawned
void ARPGInventory::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARPGInventory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ARPGInventory::SetItemInventory(ARPGItem* Item, uint8 Pos)
{
	ItemInventory[Pos] = Item;
}


void ARPGInventory::SetInventorySize(const uint8 Size)
{
	InventoryMaxSize = Size;

	if (ItemInventory.IsEmpty())
	{
		ItemInventory.Init(nullptr, InventoryMaxSize);

		// UE_LOG(LogTemp, Warning, TEXT("INVENTORY SIZE: %d"), ItemInventory.Num());
	}
}

void ARPGInventory::IncreaseInventorySize()
{
	UE_LOG(LogTemp, Warning, TEXT("INVENTORY SIZE BEFORE: %d"), ItemInventory.Num());
	ItemInventory.Emplace(nullptr);
	UE_LOG(LogTemp, Warning, TEXT("INVENTORY SIZE AFTER: %d"), ItemInventory.Num());

	UE_LOG(LogTemp, Warning, TEXT("INVENTORY MAX SIZE BEFORE: %d"), InventoryMaxSize);
	++InventoryMaxSize;
	UE_LOG(LogTemp, Warning, TEXT("INVENTORY MAX SIZE AFTER: %d"), InventoryMaxSize);
}

uint8 ARPGInventory::GetInventoryItemNumber() const
{
	uint8 Num = 0;
	if (!ItemInventory.IsEmpty())
	{
		for (ARPGItem* Item : ItemInventory)
		{
			if (Item) ++Num;
		}
	}
	return Num;
}
