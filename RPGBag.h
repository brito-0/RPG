// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPGBag.generated.h"

class UCapsuleComponent;
class UStaticMeshComponent;
class ARPGWeapon;
class ARPGThrowable;
class ARPGArmour;
class ARPGInventory;
class ARPGItem;
class ACharacter;

UCLASS()
class RPG_API ARPGBag : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARPGBag();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetBagWeapon(ARPGWeapon* Weapon);
	UFUNCTION(BlueprintCallable, Category = Weapon)
	ARPGWeapon* GetBagWeapon() const { return BagWeapon; }
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetBagThrowable(ARPGThrowable* Throwable);
	UFUNCTION(BlueprintCallable, Category = Weapon)
	ARPGThrowable* GetBagThrowable() const { return BagThrowable; }

	UFUNCTION(BlueprintCallable, Category = Armour)
	void SetBagHeadArmour(ARPGArmour* HeadArmour);
	UFUNCTION(BlueprintCallable, Category = Armour)
	ARPGArmour* GetBagHeadArmour() const { return BagHeadArmour; }
	UFUNCTION(BlueprintCallable, Category = Armour)
	void SetBagChestArmour(ARPGArmour* ChestArmour);
	UFUNCTION(BlueprintCallable, Category = Armour)
	ARPGArmour* GetBagChestArmour() const { return BagChestArmour; }
	UFUNCTION(BlueprintCallable, Category = Armour)
	void SetBagLegsArmour(ARPGArmour* LegsArmour);
	UFUNCTION(BlueprintCallable, Category = Armour)
	ARPGArmour* GetBagLegsArmour() const { return BagLegsArmour; }

	UFUNCTION(BlueprintCallable, Category = Inventory)
	void SetBagInventory(ARPGInventory* Inventory);
	UFUNCTION(BlueprintCallable, Category = Inventory)
	ARPGInventory* GetBagInventory() const { return BagInventory; }


	void SetDroppedBag();
	
	void BagClosedTimer(bool Value);
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	UCapsuleComponent* CollisionCapsule;
	UPROPERTY()
	UStaticMeshComponent* StaticMesh;

	UPROPERTY()
	float CapsuleRadius = 22.f; 
	UPROPERTY()
	float CapsuleHeight = 44.f;
	
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	ARPGWeapon* BagWeapon = nullptr;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	ARPGThrowable* BagThrowable = nullptr;

	UPROPERTY(VisibleAnywhere, Category = Armour)
	ARPGArmour* BagHeadArmour = nullptr;
	UPROPERTY(VisibleAnywhere, Category = Armour)
	ARPGArmour* BagChestArmour = nullptr;
	UPROPERTY(VisibleAnywhere, Category = Armour)
	ARPGArmour* BagLegsArmour = nullptr;

	UPROPERTY(VisibleAnywhere, Category = Inventory)
	ARPGInventory* BagInventory = nullptr;

	FTimerHandle DestructTimeHandle;
	float DestructTimeDurationFirst = 300.f;
	float DestructTimeDurationNormal = 180.f;

	UPROPERTY(VisibleAnywhere)
	bool TimerActive = false;

	bool CheckEmpty();
	
	void DestroyBag();
	
};
