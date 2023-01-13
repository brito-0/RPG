// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPGItem.generated.h"

class UCapsuleComponent;
class UStaticMeshComponent;

UENUM()
enum class ERPGItemType : uint8
{
	Heal,
	Repair,
	Throwable,
	Trap,
	Bullets
};

UCLASS()
class RPG_API ARPGItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARPGItem();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetItemName(FName Name) { ItemName = Name; }

	void SetDroppedItem(bool Value);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	FName GetItemName() const { return ItemName; }

	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual void UseItem();

	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual void ItemUsed();

	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual void StopScreen();
	
	UFUNCTION(BlueprintCallable, Category = Inventory)
	ERPGItemType GetItemType() const { return ItemType; }

	void SetItemType(const ERPGItemType Type) { ItemType = Type; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void DestroyItem() { this->Destroy(); }

	virtual void StartScreen();

private:
	UPROPERTY()
	UCapsuleComponent* CollisionCapsule;
	UPROPERTY()
	UStaticMeshComponent* StaticMesh;

	UPROPERTY()
	float CapsuleRadius = 22.f; 
	UPROPERTY()
	float CapsuleHeight = 44.f;
	
	UPROPERTY(EditAnywhere)
	FName ItemName;
	
    // item orientation in the inventory
	uint8 Orientation = 0;

	FTimerHandle DroppedItemHandle;
	float DroppedItemDuration = 120.f;
	UPROPERTY(VisibleAnywhere)
	bool DroppedItem = false;
	uint8 DroppedCounter = 0;

	UPROPERTY(VisibleAnywhere)
	ERPGItemType ItemType;
	
};
