// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPGWeapon.generated.h"

class ARPGNPC;
class UCapsuleComponent;
class UStaticMeshComponent;

UENUM()
enum class ERPGWeaponType : uint8
{
	Ranged,
	Melee
};

UCLASS()
class RPG_API ARPGWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARPGWeapon();
	
	// Called every frame
    virtual void Tick(float DeltaTime) override;

	/**
	 * fires the weapon, used with character
	 * @param Target target to hit
	 * @param BodyPart body part to hit
	 * @param CloseToTarget whether to use the close to target shot
	 * @param Attacker character or npc that attacked target
	 */
	void WeaponShoot(ARPGNPC* Target, uint8 BodyPart, bool CloseToTarget, ACharacter* Attacker);

	/**
	 * fires the weapon, used with npcs
	 * @param Target target to hit
	 * @param BodyPart body part to hit
	 * @param CloseToTarget whether to use the close to target shot
	 * @param Attacker character or npc that attacked target
	 */
	void NPCWeaponShoot(ACharacter* Target, uint8 BodyPart, bool CloseToTarget, ACharacter* Attacker);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	FName GetWeaponName() const { return WeaponName; }

	UFUNCTION(BlueprintCallable, Category = Inventory)
	float GetWeaponDamage() const { return Damage; }

	UFUNCTION(BlueprintCallable, Category = Inventory)
	uint8 GetWeaponShotAmount() const { return ShotAmount; }

	UFUNCTION(BlueprintCallable, Category = Inventory)
	double GetWeaponAccuracy() const { return Accuracy; }

	UFUNCTION(BlueprintCallable, Category = Inventory)
	float GetWeaponRange() const {return Range; }

	UFUNCTION(BlueprintCallable, Category = Inventory)
	float GetWeaponCooldown() const { return Cooldown; }

	UFUNCTION(BlueprintCallable, Category = Inventory)
	float GetWeaponReloadDuration() const { return ReloadDuration; }

	UFUNCTION(BlueprintCallable, Category = Inventory)
	uint8 GetWeaponTotalBulletAmount() const { return TotalBulletAmount; }
	UFUNCTION(BlueprintCallable, Category = Inventory)
	uint8 GetWeaponCurrentBulletAmount() const { return CurrentBulletAmount; }

	void ReduceBulletAmount() { --CurrentBulletAmount; }

	void ResetCurrentBullets() { CurrentBulletAmount = TotalBulletAmount; }
	
	void SetDroppedWeapon(bool Value);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	ERPGWeaponType GetWeaponType() const { return WeaponType; }

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

	UPROPERTY(VisibleAnywhere)
	ERPGWeaponType WeaponType;
	
	UPROPERTY(EditAnywhere)
	FName WeaponName;
	UPROPERTY(EditAnywhere)
	float Damage = 7;
	UPROPERTY(EditAnywhere)
	uint8 ShotAmount = 1;
	UPROPERTY(EditAnywhere)
	double Accuracy = .65;
	UPROPERTY(EditAnywhere)
	float Range = 0;
	UPROPERTY(EditAnywhere)
	uint8 TotalBulletAmount = 5;
	UPROPERTY()
	uint8 CurrentBulletAmount = 0;
	UPROPERTY(EditAnywhere)
	float Cooldown = 1.f;
	UPROPERTY(EditAnywhere)
	float ReloadDuration = 2.f;

	FTimerHandle DroppedWeaponHandle;
	float DroppedWeaponDuration = 120.f;
	UPROPERTY(VisibleAnywhere)
	bool DroppedWeapon = false;

	void DestroyWeapon() { this->Destroy(); }
};
