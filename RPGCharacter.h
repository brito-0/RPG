// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGArmour.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "RPGCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class USpringArmComponent;
class UUserWidget;

class ARPGNPC;
class ARPGInventory;
class ARPGItem;
class ARPGWeapon;
class ARPGArmour;
class ARPGThrowable;
class ARPGBag;

UENUM()
enum class ERPGCharacterState : uint8
{
	Normal,
	Crouch,
	Inventory,
	Heal,
	Combat,
	Throwing,
	Shooting,
	Cooldown,
	Reloading,
	Rest,
	Dead
};

UCLASS()
class RPG_API ARPGCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARPGCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	UCameraComponent* GetFollowCamera() const { return FollowCamera; }

    UFUNCTION(BlueprintCallable, Category = Health)
	float GetCharacterMaxHealth() const { return MaxHealth; }
	UFUNCTION(BlueprintCallable, Category = Health)
	float GetCharacterCurrentHealth() const { return CurrentHealth; }
	UFUNCTION(BlueprintCallable, Category = Health)
	float GetCharacterLArmMaxHealth() const { return LArmMaxHealth; }
	UFUNCTION(BlueprintCallable, Category = Health)
	float GetCharacterLArmCurrentHealth() const { return LArmCurrentHealth; }
	UFUNCTION(BlueprintCallable, Category = Health)
	float GetCharacterRArmMaxHealth() const { return RArmMaxHealth; }
	UFUNCTION(BlueprintCallable, Category = Health)
	float GetCharacterRArmCurrentHealth() const { return RArmCurrentHealth; }
	UFUNCTION(BlueprintCallable, Category = Health)
	float GetCharacterLLegMaxHealth() const { return LLegMaxHealth; }
	UFUNCTION(BlueprintCallable, Category = Health)
	float GetCharacterLLegCurrentHealth() const { return LLegCurrentHealth; }
	UFUNCTION(BlueprintCallable, Category = Health)
	float GetCharacterRLegMaxHealth() const { return RLegMaxHealth; }
	UFUNCTION(BlueprintCallable, Category = Health)
	float GetCharacterRLegCurrentHealth() const { return RLegCurrentHealth; }
	
	UFUNCTION(BlueprintCallable, Category = CharacterTarget)
	ARPGNPC* GetCharacterTarget () const { return TargetNPC; }

	UFUNCTION(BlueprintCallable, Category = CharacterState)
	ERPGCharacterState GetCharacterState() const { return CurrentCharacterState; }
	UFUNCTION(BlueprintCallable, Category = CharacterState)
	ERPGCharacterState GetPreviousCharacterState() const { return PreviousCharacterState; }

	UFUNCTION(BlueprintCallable, Category = Inventory)
	ARPGWeapon* GetCharacterWeapon() const { return CurrentWeapon; }
	UFUNCTION(BlueprintCallable, Category = Inventory)
	ARPGThrowable* GetCharacterThrowable() const { return ThrowableWeapon; }
	UFUNCTION(BlueprintCallable, Category = Inventory)
	ARPGArmour* GetCharacterHeadArmour() const { return HeadArmour; }
	UFUNCTION(BlueprintCallable, Category = Inventory)
	ARPGArmour* GetCharacterChestArmour() const { return ChestArmour; }
	UFUNCTION(BlueprintCallable, Category = Inventory)
	ARPGArmour* GetCharacterLegsArmour() const { return LegsArmour; }
	UFUNCTION(BlueprintCallable, Category = Inventory)
	ARPGInventory* GetCharacterInventory() const { return CharacterInventory; }

	uint8 GetMissSocketNumber() const { return MissSocketNumber; }

	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool BCharacterWeapon() const;
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool BCharacterThrowable() const;
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool BCharacterHeadArmour() const;
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool BCharacterChestArmour() const;
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool BCharacterLegsArmour() const;

	/**
	 * tries to attack the selected body part of the npc target
	 * @param BodyPart the body part that the character is trying to attack
	 */
	UFUNCTION(BlueprintCallable, Category = Offensive)
	void CharacterAttack(uint8 BodyPart);

	/**
	 * deals damage to the selected body part
	 * @param BodyPart body part that will take damage
	 * @param Damage amount of damage
	 */
	void CharacterTakeDamage(uint8 BodyPart, float Damage);

	UFUNCTION(BlueprintCallable, Category = HUD)
	void RemoveTargetNPC();

	UFUNCTION(BlueprintCallable, Category = HUD)
	void StartHealScreen();
	UFUNCTION(BlueprintCallable, Category = HUD)
	void StopHealScreen();

	/**
	 * heals the selected body part
	 * @param BodyPart body part that will be healed
	 */
	UFUNCTION(BlueprintCallable, Category = Heal)
	void CharacterHeal(uint8 BodyPart);

	/**
	 * increases one of the stats if the player has at least 1 level up point
	 * @param Stat the stat to increase (0 - health, 1 - strength, 2 - accuracy, 3 - agility)
	 */
	UFUNCTION(BlueprintCallable, Category = LevelUp)
	void CharacterLevelUp(uint8 Stat);

	UFUNCTION(BlueprintCallable, Category = HUD)
	void StartLevelUpScreen();
	UFUNCTION(BlueprintCallable, Category = HUD)
	void StopLevelUpScreen();

	/**
	 * drops the item in the selected position
	 * @param Pos item position
	 */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	void ItemDrop(uint8 Pos);
	
	UFUNCTION(BlueprintCallable, Category = Inventory)
	void WeaponDrop(ARPGWeapon* Weapon = nullptr);

	/**
	 * drops equipped armour depending on the selected type
	 * @param Type type of armour to drop
	 */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	void ArmourDrop(ERPGArmourType Type);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	void ThrowableEquip(ARPGThrowable* Throwable);
	
	UFUNCTION(BlueprintCallable, Category = Inventory)
	void ThrowableUnEquip();
	
	UFUNCTION(BlueprintCallable, Category = HUD)
	void StartInventoryScreen();
	UFUNCTION(BlueprintCallable, Category = HUD)
	void StopInventoryScreen();

	UFUNCTION(BlueprintCallable, Category = HUD)
	void StartBagScreen();
	UFUNCTION(BlueprintCallable, Category = HUD)
	void StopBagScreen();

	UFUNCTION(BlueprintCallable, Category = HUD)
	void RemoveSelectedItem();
	UFUNCTION(BlueprintCallable, Category = HUD)
	void DestroySelectedItem();

	UFUNCTION(BlueprintCallable, Category = HUD)
	void SetSelectedItem(ARPGItem* Item);
	UFUNCTION(BlueprintCallable, Category = HUD)
	void SetSelectedItemID(uint8 ID) { SelectedItemID = ID; }
	UFUNCTION(BlueprintCallable, Category = HUD)
	ARPGItem* GetSelectedItem() { return SelectedItem; }

	UFUNCTION(BlueprintCallable, Category = HUD)
	ARPGBag* GetSelectedBag() { return SelectedBag; }

	/**
	 * repairs equipped armour depending on the selected type
	 * @param Type type of armour to repair
	 */
	UFUNCTION(BlueprintCallable, Category = HUD)
	void CharacterRepairArmour(uint8 Type);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	void BagPickUpWeapon();
	UFUNCTION(BlueprintCallable, Category = Inventory)
	void BagPickUpThrowable();
	
	UFUNCTION(BlueprintCallable, Category = Inventory)
	void BagPickUpHeadArmour();
	UFUNCTION(BlueprintCallable, Category = Inventory)
	void BagPickUpChestArmour();
	UFUNCTION(BlueprintCallable, Category = Inventory)
	void BagPickUpLegsArmour();

	UFUNCTION(BlueprintCallable, Category = Inventory)
	void BagPickUpItem(uint8 Pos);

	/**
	 * slows the character for a set duration
	 * @param Duration duration that the character will be slowed
	 */
	void StartThrowableSlow(float Duration);

	UFUNCTION(BlueprintCallable, Category = HUD)
	void StartTradeScreen();
	UFUNCTION(BlueprintCallable, Category = HUD)
	void StopTradeScreen();
	UFUNCTION(BlueprintCallable, Category = HUD)
	void StartInventoryTradeScreen();
	UFUNCTION(BlueprintCallable, Category = HUD)
	void StopInventoryTradeScreen();

	UFUNCTION(BlueprintCallable, Category = Inventory)
	void TradePickUpWeapon();
	UFUNCTION(BlueprintCallable, Category = Inventory)
	void TradePickUpThrowable();
	
	UFUNCTION(BlueprintCallable, Category = Inventory)
	void TradePickUpHeadArmour();
	UFUNCTION(BlueprintCallable, Category = Inventory)
	void TradePickUpChestArmour();
	UFUNCTION(BlueprintCallable, Category = Inventory)
	void TradePickUpLegsArmour();

	UFUNCTION(BlueprintCallable, Category = Inventory)
	void TradePickUpItem(uint8 Pos);

	UFUNCTION(BlueprintCallable, Category = HUD)
	UUserWidget* GetLevelUpInfoMenu() { return LevelUpInfoMenu; }
	
	UFUNCTION(BlueprintCallable, Category = HUD)
    uint8 GetLevelUpPoints() { return LevelUpPoints; }

	UFUNCTION(BlueprintCallable, Category = HUD)
	void PauseGame();

	/**
	 * checks if the selected npc has the dead state
	 * @param Target npc that should be checked
	 */
	bool CheckTargetDead(ARPGNPC* Target);

	void CharacterSetBound(float Duration);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);
	
	/** Called for side to side input */
	void MoveRight(float Value);

private:	
	/** camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	UPROPERTY(EditAnywhere, Category = Mesh)
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY()
	float CapsuleRadius = 42.f; 
	UPROPERTY()
	float CapsuleHeight = 91.f;
	

	//// player stats ////
	UPROPERTY(VisibleAnywhere)
	ERPGCharacterState CurrentCharacterState;
	UPROPERTY(VisibleAnywhere)
	ERPGCharacterState PreviousCharacterState;


	// remove head and chest health ???
	
		// player health
	UPROPERTY(VisibleAnywhere, Category = Health)
	float MaxHealth = 100.f;
	UPROPERTY(VisibleAnywhere, Category = Health)
	float CurrentHealth;
		// body part health
	// float HeadMaxHealth = 100.f;
	// UPROPERTY(VisibleAnywhere, Category = Health)
	// float HeadCurrentHealth;
	// float ChestMaxHealth = 100.f;
	// UPROPERTY(VisibleAnywhere, Category = Health)
	// float ChestCurrentHealth;
	UPROPERTY(VisibleAnywhere, Category = Health)
	float LArmMaxHealth = 50.f;
	UPROPERTY(VisibleAnywhere, Category = Health)
	float LArmCurrentHealth;
	UPROPERTY(VisibleAnywhere, Category = Health)
	float RArmMaxHealth = 50.f;
	UPROPERTY(VisibleAnywhere, Category = Health)
	float RArmCurrentHealth;
	UPROPERTY(VisibleAnywhere, Category = Health)
	float LLegMaxHealth = 50.f;
	UPROPERTY(VisibleAnywhere, Category = Health)
	float LLegCurrentHealth;
	UPROPERTY(VisibleAnywhere, Category = Health)
	float RLegMaxHealth = 50.f;
	UPROPERTY(VisibleAnywhere, Category = Health)
	float RLegCurrentHealth;

	UPROPERTY(VisibleAnywhere, Category = Movement)
	float WalkSpeed = 400.f;
	UPROPERTY(VisibleAnywhere, Category = Movement)
	float CrouchSpeed = 225.f;
	UPROPERTY(VisibleAnywhere, Category = Debuffs)
	bool MoveSpeedDebuff = false;
	UPROPERTY(VisibleAnywhere, Category = Debuffs)
	bool AccuracyDebuff = false;
	UPROPERTY(VisibleAnywhere, Category = Debuffs)
	float DebuffVal = .8f;

	UPROPERTY(VisibleAnywhere)
	bool CharacterBound = false;
	void SetCharacterBoundFalse();

	UPROPERTY(VisibleAnywhere, Category = Accuracy)
	double CharacterAccuracy = .50;

	UPROPERTY(VisibleAnywhere, Category = Strngth)
	float CharacterStrength = 1.f;
	
	// add leveling system
	UPROPERTY(VisibleAnywhere, Category = Stats)
	float MaxExp = 35.f;
	UPROPERTY(VisibleAnywhere, Category = Stats)
	float CurrentExp = 0.f;
	UPROPERTY(VisibleAnywhere, Category = Stats)
	double ExpIncrease = 1.10;

	UPROPERTY(VisibleAnywhere, Category = Stats)
	uint8 LevelUpPoints = 0;
	// uint8 LevelUpPoints = 30;

	// max 10
	UPROPERTY(VisibleAnywhere, Category = Stats)
	uint8 Health = 0; // players health
	UPROPERTY(VisibleAnywhere, Category = Stats)
	uint8 Strength = 0; // carry weight amount, melee damage, throw range
	UPROPERTY(VisibleAnywhere, Category = Stats)
	uint8 Accuracy = 0; // weapon and throw accuracy
	UPROPERTY(VisibleAnywhere, Category = Stats)
	uint8 Agility = 0; // player speed ???

	/**
	 * increases current player exp value and gives 1 level up point to the player if the value reaches or surpasses the max exp value
	 * @param ExpValMin minimum amount of exp that can be gained from killing the enemy
	 * @param ExpValMax maximum amount of exp that can be gained from killing the enemy
	 */
	void CharacterGetExp(float ExpValMin, float ExpValMax);

	
	////              ////

	//
	UPROPERTY(VisibleAnywhere)
	ARPGInventory* CharacterInventory;

	UPROPERTY(VisibleAnywhere)
	ARPGItem* SelectedItem;
	UPROPERTY(VisibleAnywhere)
	uint8 SelectedItemID;

	UPROPERTY(VisibleAnywhere)
	ARPGBag* SelectedBag;
	
	
	// TArray<ARPGItem*> Inventory;
	// uint16 InventorySize = 20;

	// uint8 InventoryInitialCarryWeight = 15;
	// uint8 InventoryMaxCarryWeight;
	
	UPROPERTY(VisibleAnywhere, Category = Weapons)
	ARPGWeapon* CurrentWeapon = nullptr;
	UPROPERTY(VisibleAnywhere, Category = Weapons)
	ARPGWeapon* MeleeWeapon = nullptr;

	UPROPERTY(VisibleAnywhere, Category = Weapons)
	ARPGThrowable* ThrowableWeapon = nullptr;
	
	// remove otherweapon
	
	// UPROPERTY(VisibleAnywhere, Category = Weapons)
	// ARPGWeapon* OtherWeapon = nullptr;

	// /** switches the weapon that the player has currently equipped */
	// void SwitchWeapon();

	UPROPERTY(VisibleAnywhere, Category = Armour)
	ARPGArmour* HeadArmour = nullptr;
	UPROPERTY(VisibleAnywhere, Category = Armour)
	ARPGArmour* ChestArmour = nullptr;
	UPROPERTY(VisibleAnywhere, Category = Armour)
	ARPGArmour* LegsArmour = nullptr;

	UPROPERTY(EditAnywhere, Category = HUD)
	TSubclassOf<UUserWidget> AggressiveMenuWidget;
	UPROPERTY()
	UUserWidget* AggressiveMenu;
	UPROPERTY(EditAnywhere, Category = HUD)
	TSubclassOf<UUserWidget> InteractiveMenuWidget;
	UPROPERTY()
	UUserWidget* InteractiveMenu;
	UPROPERTY(EditAnywhere, Category = HUD)
	TSubclassOf<UUserWidget> HealMenuWidget;
	UPROPERTY()
	UUserWidget* HealMenu;
	UPROPERTY(EditAnywhere, Category = HUD)
	TSubclassOf<UUserWidget> LevelUpMenuWidget;
	UPROPERTY()
	UUserWidget* LevelUpMenu;
	UPROPERTY(EditAnywhere, Category = HUD)
    TSubclassOf<UUserWidget> LevelUpInfoMenuWidget;
    UPROPERTY()
    UUserWidget* LevelUpInfoMenu;
	UPROPERTY(EditAnywhere, Category = HUD)
	TSubclassOf<UUserWidget> InventoryMenuWidget;
	UPROPERTY()
	UUserWidget* InventoryMenu;
	UPROPERTY(EditAnywhere, Category = HUD)
	TSubclassOf<UUserWidget> BagMenuWidget;
	UPROPERTY()
	UUserWidget* BagMenu;
	UPROPERTY(EditAnywhere, Category = HUD)
	TSubclassOf<UUserWidget> TradeMenuWidget;
	UPROPERTY()
	UUserWidget* TradeMenu;
	UPROPERTY(EditAnywhere, Category = HUD)
	TSubclassOf<UUserWidget> InventoryTradeMenuWidget;
	UPROPERTY()
	UUserWidget* InventoryTradeMenu;
	UPROPERTY(EditAnywhere, Category = HUD)
	TSubclassOf<UUserWidget> PauseMenuWidget;
	UPROPERTY()
	UUserWidget* PauseMenu;
	
	//

	UPROPERTY()
	uint8 BodyPartToShoot;
	void CharacterAttackDelay();
	void CharacterMeleeAttackDelay();

	/**
	 * checks if the player has a direct line of sight with the target
	 * @param ShotLocation the section of the body where the shot will hit
	 */
	bool TargetLineOfSight(uint8 ShotLocation);
	
	FVector GetMouseLocation();
	
	/**  */
	UPROPERTY(VisibleAnywhere)
	ARPGNPC* TargetNPC;
	UPROPERTY(VisibleAnywhere)
	uint8 TargetType = 0;
	float MaxAggressiveDistance = 1550.f;
	float MaxInteractDistance = 400.f;
	UPROPERTY(VisibleAnywhere)
	uint8 MissSocketNumber = 12;
	/**  */
	void SelectNPC();
	/**  */
	void SelectNPCOffensive();

	void PlayerFaceTargetNPC();

	void RemoveAggressiveMenu();
	FTimerHandle TimeDilationHandle;
	void SetRegularTimeDilation() { UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f); }

	void CharacterStrayBulletAttack(ARPGNPC* Target);
	
	/**
	 * changes the character state
	 * @param State the new character state
	 */
	void SetCharacterState(ERPGCharacterState State);

	// functions to change the current state of the character - used with world timers
	void ChangeCharacterStateNormal() { SetCharacterState(ERPGCharacterState::Normal); }
	void ChangeCharacterStateCrouch() { SetCharacterState(ERPGCharacterState::Crouch); }
	void ChangeCharacterStateInventory() { SetCharacterState(ERPGCharacterState::Inventory); }
	void ChangeCharacterStateHeal() { SetCharacterState(ERPGCharacterState::Heal); }
	void ChangeCharacterStateCombat() { SetCharacterState(ERPGCharacterState::Combat); }
	void ChangeCharacterStateThrowing() { SetCharacterState(ERPGCharacterState::Throwing); }
	void ChangeCharacterStateShooting() { SetCharacterState(ERPGCharacterState::Shooting); }
	void ChangeCharacterStateCooldown() { SetCharacterState(ERPGCharacterState::Cooldown); }
	void ChangeCharacterStateReloading() { SetCharacterState(ERPGCharacterState::Reloading); }
	void ChangeCharacterStateRest() { SetCharacterState(ERPGCharacterState::Rest); }
	void ChangeCharacterStateDead() { SetCharacterState(ERPGCharacterState::Dead); }
	void ChangeCharacterStatePreviousState() { SetCharacterState(PreviousCharacterState); }

	FTimerHandle CombatToNormalHandle;
	float CombatToNormalDuration = 7.f;


	/***/
	void Interact();

	void CharacterThrow();

	void CharacterThrowDelay();

	UPROPERTY()
	uint8 BodyPartToHeal;
	void CharacterHealDelay();

	/***/
	void WeaponPickUp(ARPGWeapon* Weapon);

	bool ItemPickUp(ARPGItem* Item);

	void ArmourPickUp(ARPGArmour* Armour);

	void ThrowablePickUp(ARPGThrowable* Throwable);

	void ThrowableDrop(ARPGThrowable* Throwable);

	bool AddToInventory(ARPGItem* Item);

	void ChangeCharacterMoveSpeed(const float Modifier = 1.f);

	void SetMoveSpeedDebuff(const bool Value);

	// void IncreaseHealth();

	FTimerHandle MoveSpeedRemoveDebuffHandle;
	void StopThrowableSlow();

	void ReloadCurrentWeaponTimer();

	bool StateChangeBlock = false;

	void SetStateChangeBlockFalse() { StateChangeBlock = false; }
	void SetStateChangeBlockTrue() { StateChangeBlock = true; }

	void CharacterReload();

	

	void TestFunction();
	
};
