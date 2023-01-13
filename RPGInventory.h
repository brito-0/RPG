// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPGInventory.generated.h"

class ARPGItem;

UCLASS()
class RPG_API ARPGInventory : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARPGInventory();

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintCallable, Category = Inventory)
	TArray<ARPGItem*> GetItemInventory() { return ItemInventory; }
	
	void SetItemInventory(ARPGItem* Item, uint8 Pos);
	
	
	void SetInventorySize(const uint8 Size);
	void IncreaseInventorySize();

	// total size of the inventory
	UFUNCTION(BlueprintCallable, Category = Inventory)
	uint8 GetInventorySize() const { return ItemInventory.Num(); }
	// number of items inside of the inventory
	UFUNCTION(BlueprintCallable, Category = Inventory)
	uint8 GetInventoryItemNumber() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	TArray<ARPGItem*> ItemInventory;

	uint8 InventoryMaxSize;

};
