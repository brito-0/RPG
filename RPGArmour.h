// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPGArmour.generated.h"

class UCapsuleComponent;
class UStaticMeshComponent;

UENUM()
enum class ERPGArmourType : uint8
{
	Head,
	Chest,
	Legs
};

UCLASS()
class RPG_API ARPGArmour : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARPGArmour();

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	ERPGArmourType GetArmourType() const { return ArmourType; }

	UFUNCTION(BlueprintCallable, Category = Inventory)
	FName GetArmourName() const { return ArmourName; }

	UFUNCTION(BlueprintCallable, Category = Inventory)
	float GetArmourDefense() const { return Defense; }

	UFUNCTION(BlueprintCallable, Category = Inventory)
	float GetArmourMaxDurability() const { return MaxDurability; }

	UFUNCTION(BlueprintCallable, Category = Inventory)
	float GetArmourCurrentDurability() const { return CurrentDurability; }

	/**
	 *  calculates the final damage
	 *  @param DamageIn initial damage
	 */
	float CalcDamage(float DamageIn);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool RepairArmour();

	void SetDroppedArmour(bool Value);

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
	
	UPROPERTY(EditAnywhere)
	ERPGArmourType ArmourType;

	UPROPERTY(EditAnywhere)
	FName ArmourName;

	// amount by which damage taken is reduced (set to number between 0 and 1)
	UPROPERTY(EditAnywhere)
	float Defense;

	UPROPERTY(EditAnywhere)
	float MaxDurability = 25;
	UPROPERTY(VisibleAnywhere)
	float CurrentDurability;

	FTimerHandle DroppedArmourHandle;
	float DroppedArmourDuration = 120.f;
	UPROPERTY(VisibleAnywhere)
	bool DroppedArmour = false;

	void DestroyArmour() { this->Destroy(); }
};
