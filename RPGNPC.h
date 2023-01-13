// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RPGArmour.h"
#include "GameFramework/Character.h"
#include "RPGNPC.generated.h"

class USkeletalMeshComponent;
class ARPGCharacter;
class ARPGItem;
class ARPGWeapon;
class ARPGThrowable;
class ARPGArmour;
class ARPGBag;
class ARPGInventory;
// class UAnimSequence;

UENUM()
enum class ERPGNPCState : uint8
{
	Normal,
	Crouch,
	Trade,
	Heal,
	Fleeing,
	Combat,
	Searching,
	Throwing,
	Shooting,
	Cooldown,
	Reloading,
	Stagger,
	Rest,
	Stationary,
	Deactivated,
	Activated,
	Dead
};

UENUM()
enum class ERPGNPCType : uint8
{
	Neutral,
	Aggressive,
	Passive,
	Shield
};

UENUM()
enum class ERPGNPCClass : uint8
{
	Rush,
	Cover,
	Shield,
	Trapper
};

UCLASS()
class RPG_API ARPGNPC : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARPGNPC();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = Health)
	float GetNPCMaxHealth() const { return NPCMaxHealth; }
	UFUNCTION(BlueprintCallable, Category = Health)
	float GetNPCCurrentHealth() const { return NPCCurrentHealth; }
	UFUNCTION(BlueprintCallable, Category = Health)
	float GetNPCLArmMaxHealth() const { return NPCLArmMaxHealth; }
	UFUNCTION(BlueprintCallable, Category = Health)
	float GetNPCLArmCurrentHealth() const { return NPCLArmCurrentHealth; }
	UFUNCTION(BlueprintCallable, Category = Health)
	float GetNPCRArmMaxHealth() const { return NPCRArmMaxHealth; }
	UFUNCTION(BlueprintCallable, Category = Health)
	float GetNPCRArmCurrentHealth() const { return NPCRArmCurrentHealth; }
	UFUNCTION(BlueprintCallable, Category = Health)
	float GetNPCLLegMaxHealth() const { return NPCLLegMaxHealth; }
	UFUNCTION(BlueprintCallable, Category = Health)
	float GetNPCLLegCurrentHealth() const { return NPCLLegCurrentHealth; }
	UFUNCTION(BlueprintCallable, Category = Health)
	float GetNPCRLegMaxHealth() const { return NPCRLegMaxHealth; }
	UFUNCTION(BlueprintCallable, Category = Health)
	float GetNPCRLegCurrentHealth() const { return NPCRLegCurrentHealth; }

	UFUNCTION(BlueprintCallable, Category = Inventory)
	ARPGWeapon* GetNPCWeapon() const { return NPCWeapon; }
	UFUNCTION(BlueprintCallable, Category = Inventory)
	ARPGThrowable* GetNPCThrowable() const { return NPCThrowable; }

	UFUNCTION(BlueprintCallable, Category = Inventory)
	ARPGArmour* GetNPCHeadArmour() const { return NPCHeadArmour; }
	UFUNCTION(BlueprintCallable, Category = Inventory)
	ARPGArmour* GetNPCChestArmour() const { return NPCChestArmour; }
	UFUNCTION(BlueprintCallable, Category = Inventory)
	ARPGArmour* GetNPCLegsArmour() const { return NPCLegsArmour; }

	UFUNCTION(BlueprintCallable, Category = Inventory)
	ARPGInventory* GetNPCInventory() const { return NPCInventory; }

	uint8 GetMissSocketNumber() const { return MissSocketNumber; }

	/**
	 * tries to attack the selected body part of the npc or player target
	 * @param BodyPart the body part that the npc is trying to attack
	 */
	virtual void NPCAttack(uint8 BodyPart);

	/**
	 * deals damage to the selected body part
	 * @param BodyPart body part that will take damage
	 * @param Damage amount of damage
	 * @param Attacker npc or player that did the attack
	 */
	void NPCTakeDamage(uint8 BodyPart, float Damage, ACharacter* Attacker);

	float GetNPCMinKillExp() const { return MinKillExp; }
	float GetNPCMaxKillExp() const { return MaxKillExp; }

	UFUNCTION(BlueprintCallable, Category = NPCState)
	ERPGNPCState GetNPCState() const { return CurrentNPCState; }
	UFUNCTION(BlueprintCallable, Category = NPCState)
	ERPGNPCState GetPreviousNPCState() const { return PreviousNPCState; }

	ERPGNPCType GetNPCType() const { return NPCType; }

	ERPGNPCClass GetNPCClass() const { return NPCClass; }

	void NPCStartThrowableSlow(float Duration);

	void NPCStartTrade(ACharacter* Character);

	void NPCEndTrade();

	UFUNCTION(BlueprintCallable, Category = HUD)
	void SetNPCSelectedItem(ARPGItem* Item);
	UFUNCTION(BlueprintCallable, Category = HUD)
	void SetNPCSelectedItemID(uint8 ID) { NPCSelectedItemID = ID; }
	UFUNCTION(BlueprintCallable, Category = HUD)
	ARPGItem* GetNPCSelectedItem() { return NPCSelectedItem; }
	UFUNCTION(BlueprintCallable, Category = HUD)
	uint8 GetNPCSelectedItemID() { return NPCSelectedItemID; }

	void NPCWeaponPickUp(ARPGWeapon* Weapon);
	void NPCSetWeaponNull() { NPCWeapon = nullptr; }

	bool NPCItemPickUp(ARPGItem* Item);

	void NPCArmourPickUp(ARPGArmour* Armour);
	void NPCSetHeadArmourNull() { NPCHeadArmour = nullptr; }
	void NPCSetChestArmourNull() { NPCChestArmour = nullptr; }
	void NPCSetLegsArmourNull() { NPCLegsArmour = nullptr; }

	void NPCThrowablePickUp(ARPGThrowable* Throwable);
	void NPCSetThrowableNull() { NPCThrowable = nullptr; }

	void NPCSetBound(float Duration);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetNPCType(ERPGNPCType Type) { NPCType = Type; }

	void SetNPCClass(ERPGNPCClass Class) { NPCClass = Class; }

	void NPCAttackResponse(ACharacter* Attacker);

	virtual void NPCCombatAction();
	
	void NPCNextAction();

	void SetNPCMaxHealth(float Value) { NPCMaxHealth = Value; }
	void SetNPCLArmMaxHealth(float Value) { NPCLArmMaxHealth = Value; }
	void SetNPCRArmMaxHealth(float Value) { NPCRArmMaxHealth = Value; }
	void SetNPCLLegMaxHealth(float Value) { NPCLLegMaxHealth = Value; }
	void SetNPCRLegMaxHealth(float Value) { NPCRLegMaxHealth = Value; }

	void SetNPCMinKillExp(float Value) { MinKillExp = Value; }
	void SetNPCMaxKillExp(float Value) { MaxKillExp = Value; }
	
	void SetNPCWalkSpeed(float Value) { WalkSpeed = Value; }
	void SetNPCCrouchSpeed(float Value) { CrouchSpeed = Value; }

	void SetNPCStrength(float Value) { NPCStrength = Value; }
	
	void SetNPCAccuracy(double Value) { NPCAccuracy = Value; }

	void SetNPCShotDelay(float Value) { NPCShotDelay = Value; }

	ACharacter* GetNPCTargetEnemy() { return NPCTargetEnemy; }

	bool GetMoveSpeedDebuff() { return MoveSpeedDebuff; }

	bool GetAccuracyDebuff() { return AccuracyDebuff; }

	void NPCAttackDelay();
	void NPCMeleeAttackDelay();

	void NPCHealDelay();

	// void NPCRepairDelay();

	float GetNPCHealDuration() const { return NPCHealDuration; }

	void SetNPCHealDuration(float Value) { NPCHealDuration = Value; }

	bool NPCTargetLineOfSight(uint8 ShotLocation);

	void NPCStrayBulletAttack(ACharacter* Target);

	bool NPCCheckTargetDead(ACharacter* Target);

	void NPCFaceTarget();

	void NPCWeaponDrop();

	void NPCItemDrop(uint8 Pos);

	void NPCArmourDrop(ERPGArmourType Type);

	void NPCThrowableDrop(ARPGThrowable* Throwable);

	bool NPCThrowableEquip();

	void NPCThrowableUnEquip();

	bool NPCAddToInventory(ARPGItem* Item);

	void NPCLevelUp();

	void NPCChangeMoveSpeed(const float Modifier = 1.f);

	void NPCSetMoveSpeedDebuff(const bool Value);

	// 0 - arms | 1 - legs
	void CheckHealRequired(uint8 Priority);
	void NPCHeal(uint8 BodyPart);

	void NPCDestroySelectedItem();

	void CheckRepairRequired();
	void NPCRepairArmour();

	uint8 NPCCheckInventory(FName Tag);
	
	uint8 NPCCountInventory(FName Tag);

	UPROPERTY(VisibleAnywhere)
	AAIController* NPCController;

	void MoveToEnemy();

	float SpeedInc = 1.0f;
	float SpeedIncInitial = 1.0f;

	float CurrentDistanceToEnemy;

	void NPCThrow();

	bool GetWasAttacked() const { return WasAttacked; }


	virtual bool WeaponCheckPriority(ARPGWeapon* Weapon);
	virtual bool ThrowableCheckPriority(ARPGThrowable* Throwable);
	virtual bool HeadArmourCheckPriority(ARPGArmour* HArmour);
	virtual bool ChestArmourCheckPriority(ARPGArmour* CArmour);
	virtual bool LegsArmourCheckPriority(ARPGArmour* LArmour);
	virtual bool ItemsCheckPriority(ARPGItem* Item);
	// virtual bool HealsCheckPriority();
	// virtual bool RepairsCheckPriority();

	UPROPERTY(VisibleAnywhere)
	bool NPCAlreadyAttacked = false;

	void RemoveNPCSelectedItem();

	virtual void DeathAction();

	ARPGCharacter* GetPlayerCharacter() { return PlayerCharacter; }
	
	

	/**
	 * changes the npc state
	 * @param State the new npc state
	 */
	void SetNPCState(ERPGNPCState State);

	// functions to change the current state of the character - used with world timers
	void ChangeNPCStateNormal() { SetNPCState(ERPGNPCState::Normal); }
	void ChangeNPCStateCrouch() { SetNPCState(ERPGNPCState::Crouch); }
	void ChangeNPCStateTrade() { SetNPCState(ERPGNPCState::Trade); }
	void ChangeNPCStateHeal() { SetNPCState(ERPGNPCState::Heal); }
	void ChangeNPCStateFleeing() { SetNPCState(ERPGNPCState::Fleeing); }
	void ChangeNPCStateCombat() { SetNPCState(ERPGNPCState::Combat); }
	void ChangeNPCStateSearching() { SetNPCState(ERPGNPCState::Searching); }	
	void ChangeNPCStateThrowing() { SetNPCState(ERPGNPCState::Throwing); }	
	void ChangeNPCStateShooting() { SetNPCState(ERPGNPCState::Shooting); }
	void ChangeNPCStateCooldown() { SetNPCState(ERPGNPCState::Cooldown); }
	void ChangeNPCStateReloading() { SetNPCState(ERPGNPCState::Reloading); }
	void ChangeNPCStateStagger() { SetNPCState(ERPGNPCState::Stagger); }
	void ChangeNPCStateRest() { SetNPCState(ERPGNPCState::Rest); }
	void ChangeNPCStateStationary() { SetNPCState(ERPGNPCState::Stationary); }
	void ChangeNPCStateDeactivated() { SetNPCState(ERPGNPCState::Deactivated); }
	void ChangeNPCStateActivated() { SetNPCState(ERPGNPCState::Activated); }
	void ChangeNPCStateDead() { SetNPCState(ERPGNPCState::Dead); }
	void ChangeNPCStatePreviousState() { SetNPCState(PreviousNPCState); }

private:
	UPROPERTY(EditAnywhere, Category = Mesh)
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY()
	float CapsuleRadius = 42.f; 
	UPROPERTY()
	float CapsuleHeight = 91.f;

	// 0 - Neutral
	// 1 - Aggressive
	// 2 - Passive
	// uint8 NPCType = 0;

	UPROPERTY(VisibleAnywhere)
	ERPGNPCState CurrentNPCState;
	UPROPERTY(VisibleAnywhere)
	ERPGNPCState PreviousNPCState;
	UPROPERTY(VisibleAnywhere)
	ERPGNPCState NextNPCState;

	UPROPERTY(EditAnywhere)
	ERPGNPCType NPCType;

	UPROPERTY(VisibleAnywhere)
	ERPGNPCClass NPCClass;

	// npc stats
	UPROPERTY(EditAnywhere, Category = Health)
	float NPCMaxHealth = 100.f;
	UPROPERTY(VisibleAnywhere, Category = Health)
	float NPCCurrentHealth;
	// body part health
	UPROPERTY(EditAnywhere, Category = Health)
	float NPCLArmMaxHealth = 50.f;
	UPROPERTY(VisibleAnywhere, Category = Health)
	float NPCLArmCurrentHealth;
	UPROPERTY(EditAnywhere, Category = Health)
	float NPCRArmMaxHealth = 50.f;
	UPROPERTY(VisibleAnywhere, Category = Health)
	float NPCRArmCurrentHealth;
	UPROPERTY(EditAnywhere, Category = Health)
	float NPCLLegMaxHealth = 50.f;
	UPROPERTY(VisibleAnywhere, Category = Health)
	float NPCLLegCurrentHealth;
	UPROPERTY(EditAnywhere, Category = Health)
	float NPCRLegMaxHealth = 50.f;
	UPROPERTY(VisibleAnywhere, Category = Health)
	float NPCRLegCurrentHealth;

	float NPCFleeHealth;

	UPROPERTY(EditAnywhere, Category = Movement)
	float WalkSpeed = 400.f;
	UPROPERTY(EditAnywhere, Category = Movement)
	float CrouchSpeed = 225.f;
	UPROPERTY(VisibleAnywhere, Category = Debuff)
	bool MoveSpeedDebuff = false;
	UPROPERTY(VisibleAnywhere, Category = Debuff)
	bool AccuracyDebuff = false;
	UPROPERTY(EditAnywhere, Category = Debuff)
	float DebuffVal = .6f;

	UPROPERTY(VisibleAnywhere)
	bool NPCBound = false;
	void SetNPCBoundFalse();

	UPROPERTY(VisibleAnywhere)
	uint8 MissSocketNumber = 12;
	
	UPROPERTY(VisibleAnywhere)
	ARPGInventory* NPCInventory;

	UPROPERTY(EditAnywhere)
	uint8 NPCInventorySize = 7;

	UPROPERTY()
	ARPGItem* NPCSelectedItem;
	UPROPERTY()
	uint8 NPCSelectedItemID;

	UPROPERTY(EditAnywhere, Category = Weapons)
	TSubclassOf<ARPGWeapon> NPCWeaponObject = nullptr;
	UPROPERTY(VisibleAnywhere, Category = Weapons)
	ARPGWeapon* NPCWeapon = nullptr;
	UPROPERTY(EditAnywhere, Category = Weapons)
	TSubclassOf<ARPGThrowable> NPCThrowableObject = nullptr;
	UPROPERTY(VisibleAnywhere, Category = Weapons)
	ARPGThrowable* NPCThrowable = nullptr;

	UPROPERTY(EditAnywhere, Category = Armour)
	TSubclassOf<ARPGArmour> NPCHeadArmourObject = nullptr;
	UPROPERTY(VisibleAnywhere, Category = Armour)
	ARPGArmour* NPCHeadArmour = nullptr;
	UPROPERTY(EditAnywhere, Category = Armour)
	TSubclassOf<ARPGArmour> NPCChestArmourObject = nullptr;
	UPROPERTY(VisibleAnywhere, Category = Armour)
	ARPGArmour* NPCChestArmour = nullptr;
	UPROPERTY(EditAnywhere, Category = Armour)
	TSubclassOf<ARPGArmour> NPCLegsArmourObject = nullptr;
	UPROPERTY(VisibleAnywhere, Category = Armour)
	ARPGArmour* NPCLegsArmour = nullptr;

	UPROPERTY(EditAnywhere, Category = Inventory)
	TSubclassOf<ARPGItem> NPCItem0Object = nullptr;
	UPROPERTY(VisibleAnywhere, Category = Inventory)
	ARPGItem* NPCItem0 = nullptr;
	UPROPERTY(EditAnywhere, Category = Inventory)
	TSubclassOf<ARPGItem> NPCItem1Object = nullptr;
	UPROPERTY(VisibleAnywhere, Category = Inventory)
	ARPGItem* NPCItem1 = nullptr;
	UPROPERTY(EditAnywhere, Category = Inventory)
	TSubclassOf<ARPGItem> NPCItem2Object = nullptr;
	UPROPERTY(VisibleAnywhere, Category = Inventory)
	ARPGItem* NPCItem2 = nullptr;
	UPROPERTY(EditAnywhere, Category = Inventory)
	TSubclassOf<ARPGItem> NPCItem3Object = nullptr;
	UPROPERTY(VisibleAnywhere, Category = Inventory)
	ARPGItem* NPCItem3 = nullptr;
	UPROPERTY(EditAnywhere, Category = Inventory)
	TSubclassOf<ARPGItem> NPCItem4Object = nullptr;
	UPROPERTY(VisibleAnywhere, Category = Inventory)
	ARPGItem* NPCItem4 = nullptr;
	UPROPERTY(EditAnywhere, Category = Inventory)
	TSubclassOf<ARPGItem> NPCItem5Object = nullptr;
	UPROPERTY(VisibleAnywhere, Category = Inventory)
	ARPGItem* NPCItem5 = nullptr;
	UPROPERTY(EditAnywhere, Category = Inventory)
	TSubclassOf<ARPGItem> NPCItem6Object = nullptr;
	UPROPERTY(VisibleAnywhere, Category = Inventory)
	ARPGItem* NPCItem6 = nullptr;

	float MinKillExp = 3.f;
	float MaxKillExp = 5.f;

	// max 10
	UPROPERTY(VisibleAnywhere, Category = Stats)
	uint8 Health = 0; // npc health
	UPROPERTY(VisibleAnywhere, Category = Stats)
	uint8 Strength = 0; // melee damage, throw range
	UPROPERTY(VisibleAnywhere, Category = Stats)
	uint8 Accuracy = 0; // weapon and throw accuracy
	UPROPERTY(VisibleAnywhere, Category = Stats)
	uint8 Agility = 0; // npc speed

	UPROPERTY(VisibleAnywhere, Category = Accuracy)
	double NPCAccuracy = .50;

	UPROPERTY(VisibleAnywhere, Category = Strngth)
	float NPCStrength = 1.f;
	

	UPROPERTY(VisibleAnywhere)
	ACharacter* NPCTargetEnemy;

	UPROPERTY(VisibleAnywhere)
	uint8 NPCBackwardsTargetTicks = 0;
	
	UPROPERTY(VisibleAnywhere)
	ACharacter* NPCBackwardsTarget;

	UPROPERTY()
	uint8 BodyPartToShoot;

	float NPCShotDelay = 1.f;

	FTimerHandle NPCCombatToNormalHandle;
	float NPCCombatToNormalDuration = 10.f;

	UPROPERTY(EditAnywhere, Category = DeathBag)
	TSubclassOf<ARPGBag> NPCDeathBagObject;
	UPROPERTY(VisibleAnywhere, Category = DeathBag)
	ARPGBag* NPCDeathBag;

	UPROPERTY()
	uint8 BodyPartToHeal;

	float NPCHealDuration = 2.f;
	
	//FTimerHandle ShieldDestroyHandle;
	
	void SetUpDeathBag();

	void DestroyNPC();

	UPROPERTY(VisibleAnywhere)
	ARPGCharacter* PlayerCharacter;
	
	void NPCThrowDelay();

	FTimerHandle NPCMoveSpeedRemoveDebuffHandle;
	void NPCStopThrowableSlow();

	UPROPERTY(VisibleAnywhere)
	bool WasAttacked = false;

	void NPCCheckDeathBag(ARPGBag* Bag);

	void NPCFlee();

	void NPCWander();

	void NPCReloadCurrentWeaponTimer();

	bool NPCStateChangeBlock = false;

	void SetNPCStateChangeBlockFalse() { NPCStateChangeBlock = false; }
	void SetNPCStateChangeBlockTrue() { NPCStateChangeBlock = true; }

	// UPROPERTY()
	// UAnimSequence* EmptyAnim;
	
};
