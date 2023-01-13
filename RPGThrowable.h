// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGItem.h"
#include "RPGThrowable.generated.h"

class ACharacter;

UENUM()
enum class ERPGThrowableType : uint8
{
	Explosive,
	Incendiary,
	Shield,
	Slow
};

UCLASS()
class RPG_API ARPGThrowable : public ARPGItem
{
	GENERATED_BODY()

public:
	ARPGThrowable();

	virtual void UseItem() override;

	void SetRemainingUses(uint8 Value) { RemainingUses = Value; }
	
	UFUNCTION(BlueprintCallable, Category = Inventory)
	uint8 GetRemainingUses() const { return RemainingUses; }

	virtual  bool DecreaseRemainingUses();

	void SetDamage(float Value) { Damage = Value; }
	
	UFUNCTION(BlueprintCallable, Category = Inventory)
	float GetDamage() const { return Damage; }

	UFUNCTION(BlueprintCallable, Category = Inventory)
	ERPGThrowableType GetThrowableType() const { return ThrowableType; }

	void SetThrowableType(const ERPGThrowableType Type) { ThrowableType = Type; }

	virtual void ThrowableThrow(ACharacter* Target, ACharacter* Attacker);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	uint8 RemainingUses;
	
private:
	UPROPERTY(VisibleAnywhere)
	float Damage;

	UPROPERTY(VisibleAnywhere)
	ERPGThrowableType ThrowableType;
	
};
