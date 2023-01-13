// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGNPC.h"
#include "RPGArmour.h"
#include "RPGWeapon.h"
#include "RPGBag.h"
#include "RPGCharacter.h"
#include "RPGInventory.h"
#include "RPGItem.h"
#include "RPGThrowable.h"
#include "RPGThrowableShield.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ARPGNPC::ARPGNPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(CapsuleRadius, CapsuleHeight);

	
	// static ConstructorHelpers::FObjectFinder<UBlueprint>NPCDeathBagAsset(TEXT("Blueprint'/Game/Blueprints/NPCs/RPGBag_BP.RPGBag_BP'"));
	// if (NPCDeathBagAsset.Succeeded()) { NPCDeathBagObject = NPCDeathBagAsset.Object->GeneratedClass; }

	SetActorTickInterval(1.5f);
	
	this->Tags.Add("NPC");
}

// Called when the game starts or when spawned
void ARPGNPC::BeginPlay()
{
	Super::BeginPlay();

	
	NPCCurrentHealth = GetNPCMaxHealth();
	// HeadCurrentHealth = HeadMaxHealth;
	// ChestCurrentHealth = ChestMaxHealth;
	NPCLArmCurrentHealth = GetNPCLArmMaxHealth();
	NPCRArmCurrentHealth = GetNPCRArmMaxHealth();
	NPCLLegCurrentHealth = GetNPCLLegMaxHealth();
	NPCRLegCurrentHealth = GetNPCRLegMaxHealth();

	if (GetNPCType() == ERPGNPCType::Shield) return;

	ChangeNPCStateDeactivated();

	NPCInventory = NewObject<ARPGInventory>();
	NPCInventory->SetInventorySize(NPCInventorySize);

	const FAttachmentTransformRules AttachRules(EAttachmentRule::KeepRelative, true);
	
	if (NPCDeathBagObject)
	{
		NPCDeathBag = GetWorld()->SpawnActor<ARPGBag>(NPCDeathBagObject);
		NPCDeathBag->SetActorHiddenInGame(true);
		NPCDeathBag->AttachToComponent(GetMesh(), AttachRules, "InventorySocket");
	}

	if (NPCWeaponObject)
	{
		NPCWeapon = GetWorld()->SpawnActor<ARPGWeapon>(NPCWeaponObject);
		NPCWeapon->SetActorHiddenInGame(true);
		NPCWeapon->AttachToComponent(GetMesh(), AttachRules, "InventorySocket");
	}
	if (NPCThrowableObject)
	{
		NPCThrowable = GetWorld()->SpawnActor<ARPGThrowable>(NPCThrowableObject);
		NPCThrowable->SetActorHiddenInGame(true);
		NPCThrowable->AttachToComponent(GetMesh(), AttachRules, "InventorySocket");
	}
	if (NPCHeadArmourObject)
	{
		NPCHeadArmour = GetWorld()->SpawnActor<ARPGArmour>(NPCHeadArmourObject);
		NPCHeadArmour->SetActorHiddenInGame(true);
		NPCHeadArmour->AttachToComponent(GetMesh(), AttachRules, "InventorySocket");
	}
	if (NPCChestArmourObject)
	{
		NPCChestArmour = GetWorld()->SpawnActor<ARPGArmour>(NPCChestArmourObject);
		NPCChestArmour->SetActorHiddenInGame(true);
		NPCChestArmour->AttachToComponent(GetMesh(), AttachRules, "InventorySocket");
	}
	if (NPCLegsArmourObject)
	{
		NPCLegsArmour = GetWorld()->SpawnActor<ARPGArmour>(NPCLegsArmourObject);
		NPCLegsArmour->SetActorHiddenInGame(true);
		NPCLegsArmour->AttachToComponent(GetMesh(), AttachRules, "InventorySocket");
	}	
	if (NPCItem0Object)
	{
		NPCItem0 = GetWorld()->SpawnActor<ARPGItem>(NPCItem0Object);
		NPCItem0->SetActorHiddenInGame(true);
		NPCItem0->AttachToComponent(GetMesh(), AttachRules, "InventorySocket");
		NPCInventory->SetItemInventory(NPCItem0, 0);
	}
	if (NPCItem1Object)
	{
		NPCItem1 = GetWorld()->SpawnActor<ARPGItem>(NPCItem1Object);
		NPCItem1->SetActorHiddenInGame(true);
		NPCItem1->AttachToComponent(GetMesh(), AttachRules, "InventorySocket");
		NPCInventory->SetItemInventory(NPCItem1, 1);
	}
	if (NPCItem2Object)
	{
		NPCItem2 = GetWorld()->SpawnActor<ARPGItem>(NPCItem2Object);
		NPCItem2->SetActorHiddenInGame(true);
		NPCItem2->AttachToComponent(GetMesh(), AttachRules, "InventorySocket");
		NPCInventory->SetItemInventory(NPCItem2, 2);
	}
	if (NPCItem3Object)
	{
		NPCItem3 = GetWorld()->SpawnActor<ARPGItem>(NPCItem3Object);
		NPCItem3->SetActorHiddenInGame(true);
		NPCItem3->AttachToComponent(GetMesh(), AttachRules, "InventorySocket");
		NPCInventory->SetItemInventory(NPCItem3, 3);
	}
	if (NPCItem4Object)
	{
		NPCItem4 = GetWorld()->SpawnActor<ARPGItem>(NPCItem4Object);
		NPCItem4->SetActorHiddenInGame(true);
		NPCItem4->AttachToComponent(GetMesh(), AttachRules, "InventorySocket");
		NPCInventory->SetItemInventory(NPCItem4, 4);
	}
	if (NPCItem5Object)
	{
		NPCItem5 = GetWorld()->SpawnActor<ARPGItem>(NPCItem5Object);
		NPCItem5->SetActorHiddenInGame(true);
		NPCItem5->AttachToComponent(GetMesh(), AttachRules, "InventorySocket");
		NPCInventory->SetItemInventory(NPCItem5, 5);
	}
	if (NPCItem6Object)
	{
		NPCItem6 = GetWorld()->SpawnActor<ARPGItem>(NPCItem6Object);
		NPCItem6->SetActorHiddenInGame(true);
		NPCItem6->AttachToComponent(GetMesh(), AttachRules, "InventorySocket");
		NPCInventory->SetItemInventory(NPCItem6, 6);
	}

	// gives npc random stats
	NPCLevelUp();

	NPCFleeHealth = GetNPCMaxHealth() * FMath::RandRange(.1f, .3f);

	PlayerCharacter = Cast<ARPGCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called every frame
void ARPGNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	// check distance and remove taret if too far																																				asdfafasdfasdfadf

	if (GetNPCState() == ERPGNPCState::Deactivated && GetDistanceTo(PlayerCharacter) <= 4000) ChangeNPCStateActivated();
	if (GetNPCState() == ERPGNPCState::Normal)
	{
		if (GetDistanceTo(PlayerCharacter) > 4000) ChangeNPCStateDeactivated();
		else NPCNextAction();
	}
}


void ARPGNPC::SetNPCState(ERPGNPCState State)
{
	if ((State == ERPGNPCState::Combat && GetNPCState() == ERPGNPCState::Combat) || (State == ERPGNPCState::Combat && GetNPCState() == ERPGNPCState::Shooting)
		|| (NPCStateChangeBlock && State != ERPGNPCState::Dead && State == ERPGNPCState::Deactivated))
	{
		GetWorldTimerManager().ClearTimer(NPCCombatToNormalHandle);
		GetWorldTimerManager().SetTimer(NPCCombatToNormalHandle, this, &ARPGNPC::ChangeNPCStateNormal, NPCCombatToNormalDuration, false);
		return;
	}
	if (GetNPCState() == State || GetNPCState() == ERPGNPCState::Dead) return;

	if (GetNPCState() == ERPGNPCState::Stationary && State != ERPGNPCState::Dead) return;
	if (GetNPCState() == ERPGNPCState::Deactivated && State != ERPGNPCState::Activated) return;

	switch (State)
	{
	case ERPGNPCState::Normal:
		{
			if (GetNPCState() == ERPGNPCState::Combat) PreviousNPCState = GetNPCState();

			CurrentNPCState = ERPGNPCState::Normal;

			if (!NPCBound)
			{
				GetCharacterMovement()->SetMovementMode(MOVE_Walking);
				NPCChangeMoveSpeed();
			}
			// if (!MoveSpeedDebuff) GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
			// else GetCharacterMovement()->MaxWalkSpeed = WalkSpeed * DebuffVal;

			NPCTargetEnemy = nullptr;
			
			WasAttacked = false;

			SpeedInc = SpeedIncInitial;

			NPCAlreadyAttacked = false;

			CheckHealRequired(0);
			CheckRepairRequired();

			FTimerHandle NPCStartWanderHandle;
			GetWorldTimerManager().SetTimer(NPCStartWanderHandle, this, &ARPGNPC::NPCWander, 1.f, false);
			
			UE_LOG(LogTemp, Warning, TEXT("NPC -> Normal"));
			break;
		}
	case ERPGNPCState::Crouch:
		{
			if (GetNPCState() == ERPGNPCState::Normal || GetNPCState() == ERPGNPCState::Combat) PreviousNPCState = GetNPCState();

			CurrentNPCState = ERPGNPCState::Crouch;

			if (!NPCBound)
			{
				GetCharacterMovement()->SetMovementMode(MOVE_Walking);
				NPCChangeMoveSpeed();
			}
			// if (!MoveSpeedDebuff) GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed;
			// else GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed * DebuffVal;

			UE_LOG(LogTemp, Warning, TEXT("NPC -> Crouch"));
			break;
		}
	case ERPGNPCState::Trade:
		{
			// can only change state to inventory when in normal state
			PreviousNPCState = ERPGNPCState::Normal;

			CurrentNPCState = ERPGNPCState::Trade;

			GetCharacterMovement()->SetMovementMode(MOVE_None);
			if (NPCController) NPCController->StopMovement();
			
			UE_LOG(LogTemp, Warning, TEXT("NPC -> Trade"));
			break;
		}
	case ERPGNPCState::Heal:
		{
			if (GetNPCState() == ERPGNPCState::Normal || GetNPCState() == ERPGNPCState::Combat) PreviousNPCState = GetNPCState();

			CurrentNPCState = ERPGNPCState::Heal;

			GetCharacterMovement()->SetMovementMode(MOVE_None);
			if (NPCController) NPCController->StopMovement();

			// return to previous state after some time
			FTimerHandle NPCStopHealHandle;
			GetWorldTimerManager().SetTimer(NPCStopHealHandle, this, &ARPGNPC::ChangeNPCStatePreviousState, GetNPCHealDuration(), false);
			
			UE_LOG(LogTemp, Warning, TEXT("NPC -> Heal/Repair"));
			break;
		}
	case ERPGNPCState::Fleeing:
		{
			if (GetNPCState() == ERPGNPCState::Normal) PreviousNPCState = GetNPCState();
			
			CurrentNPCState = ERPGNPCState::Fleeing;

			if (!NPCBound)
			{
				GetCharacterMovement()->SetMovementMode(MOVE_Walking);
				NPCChangeMoveSpeed();
			}
			// if (!MoveSpeedDebuff) GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
			// else GetCharacterMovement()->MaxWalkSpeed = WalkSpeed * DebuffVal;

			// check if far away enough from the attacker
			NPCFlee();
			
			UE_LOG(LogTemp, Warning, TEXT("NPC -> Fleeing"));
			break;
		}
	case ERPGNPCState::Combat:
		{
			if (GetNPCState() == ERPGNPCState::Normal) PreviousNPCState = GetNPCState();
			
			CurrentNPCState = ERPGNPCState::Combat;

			if (!NPCBound)
			{
				GetCharacterMovement()->SetMovementMode(MOVE_Walking);
				NPCChangeMoveSpeed();
			}
			
			// if (!MoveSpeedDebuff) GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
			// else GetCharacterMovement()->MaxWalkSpeed = WalkSpeed * DebuffVal;

			NPCCombatAction();

			GetWorldTimerManager().ClearTimer(NPCCombatToNormalHandle);
			GetWorldTimerManager().SetTimer(NPCCombatToNormalHandle, this, &ARPGNPC::ChangeNPCStateNormal, NPCCombatToNormalDuration, false);
			
			UE_LOG(LogTemp, Warning, TEXT("NPC -> Combat"));
			break;
		}
	case ERPGNPCState::Searching:
		{
			if (GetNPCState() == ERPGNPCState::Normal) PreviousNPCState = GetNPCState();
			
			CurrentNPCState = ERPGNPCState::Searching;

			if (!NPCBound)
			{
				GetCharacterMovement()->SetMovementMode(MOVE_Walking);
				NPCChangeMoveSpeed();
			}
			// if (!MoveSpeedDebuff) GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
			// else GetCharacterMovement()->MaxWalkSpeed = WalkSpeed * DebuffVal;

			// add timer to return to normal state different types of npc have different search durations
			
			UE_LOG(LogTemp, Warning, TEXT("NPC -> Searching"));
			break;
		}
	case ERPGNPCState::Throwing:
		{
			PreviousNPCState = ERPGNPCState::Combat;

			CurrentNPCState = ERPGNPCState::Throwing;

			GetWorldTimerManager().ClearTimer(NPCCombatToNormalHandle);
			GetWorldTimerManager().SetTimer(NPCCombatToNormalHandle, this, &ARPGNPC::ChangeNPCStateNormal, NPCCombatToNormalDuration, false);

			NPCFaceTarget();
			GetCharacterMovement()->SetMovementMode(MOVE_None);
			if (NPCController) NPCController->StopMovement();
			
			// change to combat after some time
			FTimerHandle NPCStopThrowingHandle;
			GetWorldTimerManager().SetTimer(NPCStopThrowingHandle, this, &ARPGNPC::ChangeNPCStateCombat, .7f, false);
			
			UE_LOG(LogTemp, Warning, TEXT("NPC -> Throwing"));
			break;
		}
	case ERPGNPCState::Shooting:
		{
			// if (GetCharacterState() == ERPGCharacterState::Normal || GetCharacterState() == ERPGCharacterState::Combat) PreviousCharacterState = GetCharacterState();
			PreviousNPCState = ERPGNPCState::Combat;

			CurrentNPCState = ERPGNPCState::Shooting;

			GetWorldTimerManager().ClearTimer(NPCCombatToNormalHandle);
			GetWorldTimerManager().SetTimer(NPCCombatToNormalHandle, this, &ARPGNPC::ChangeNPCStateNormal, NPCCombatToNormalDuration, false);

			NPCFaceTarget();
			GetCharacterMovement()->SetMovementMode(MOVE_None);
			if (NPCController) NPCController->StopMovement();

			if (NPCWeapon->GetWeaponType() == ERPGWeaponType::Ranged) NPCWeapon->ReduceBulletAmount();
			
			if (NPCWeapon->GetWeaponCurrentBulletAmount() > 0)
			{
				FTimerHandle NPCStopShootingHandle;
				GetWorldTimerManager().SetTimer(NPCStopShootingHandle, this, &ARPGNPC::ChangeNPCStateCooldown, 1.5f, false);
			}
			else
			{
				FTimerHandle NPCStopShootingHandle;
				GetWorldTimerManager().SetTimer(NPCStopShootingHandle, this, &ARPGNPC::ChangeNPCStateReloading, 1.5f, false);
			}

			// GetMesh()->PlayAnimation(EmptyAnim, false);
			// GetMesh()->SetAnimation(EmptyAnim);
			
			UE_LOG(LogTemp, Warning, TEXT("NPC -> Shooting"));
			break;
		}
	case ERPGNPCState::Cooldown:
		{
			CurrentNPCState = ERPGNPCState::Cooldown;

			if (!NPCBound)
			{
				GetCharacterMovement()->SetMovementMode(MOVE_Walking);
				NPCChangeMoveSpeed();
			}
			// if (!MoveSpeedDebuff) GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
			// else GetCharacterMovement()->MaxWalkSpeed = WalkSpeed * DebuffVal;

			// prevents unwanted state changes when npc takes damage
			SetNPCStateChangeBlockTrue();
			FTimerHandle NPCStopStateChangeBlockHandle;
			GetWorldTimerManager().SetTimer(NPCStopStateChangeBlockHandle, this, &ARPGNPC::SetNPCStateChangeBlockFalse, NPCWeapon->GetWeaponCooldown() - .1f, false);
			
			// change to combat after some time
			FTimerHandle NPCStopCooldownHandle;
			GetWorldTimerManager().SetTimer(NPCStopCooldownHandle, this, &ARPGNPC::ChangeNPCStateCombat, NPCWeapon->GetWeaponCooldown(), false);

			UE_LOG(LogTemp, Warning, TEXT("NPC -> Cooldown"));
			break;
		}
	case ERPGNPCState::Reloading:
		{
			if (GetNPCState() == ERPGNPCState::Normal || GetNPCState() == ERPGNPCState::Combat) PreviousNPCState = GetNPCState();

			CurrentNPCState = ERPGNPCState::Reloading;

			if (!NPCBound)
			{
				GetCharacterMovement()->SetMovementMode(MOVE_Walking);
				NPCChangeMoveSpeed(.5);
			}
			// if (!MoveSpeedDebuff) GetCharacterMovement()->MaxWalkSpeed = WalkSpeed * .5f;
			// else GetCharacterMovement()->MaxWalkSpeed = (WalkSpeed * DebuffVal) * .5f;

			
			FTimerHandle NPCReloadWeaponHandle;
			GetWorldTimerManager().SetTimer(NPCReloadWeaponHandle, this, &ARPGNPC::NPCReloadCurrentWeaponTimer, NPCWeapon->GetWeaponReloadDuration() - .1f, false);

			// prevents unwanted state changes when npc takes damage
			SetNPCStateChangeBlockTrue();
			FTimerHandle NPCStopStateChangeBlockHandle;
			GetWorldTimerManager().SetTimer(NPCStopStateChangeBlockHandle, this, &ARPGNPC::SetNPCStateChangeBlockFalse, NPCWeapon->GetWeaponReloadDuration() - .1f, false);

			// return to previous state after some time
			FTimerHandle NPCStopReloadHandle;
			GetWorldTimerManager().SetTimer(NPCStopReloadHandle, this, &ARPGNPC::ChangeNPCStatePreviousState, NPCWeapon->GetWeaponReloadDuration(), false);

			UE_LOG(LogTemp, Warning, TEXT("NPC -> Reloading"));
			break;
		}
	case ERPGNPCState::Stagger:
		{
			// PreviousNPCState = ERPGNPCState::Combat;
			switch (GetNPCType())
			{
			case ERPGNPCType::Aggressive:
				{
					// PreviousNPCState = ERPGNPCState::Combat;
					// break;
				}
			case ERPGNPCType::Neutral:
				{
					// if (GetNPCCurrentHealth() < NPCFleeHealth) PreviousNPCState = ERPGNPCState::Fleeing;
					// else PreviousNPCState = ERPGNPCState::Combat;
					PreviousNPCState = ERPGNPCState::Combat;
					break;
				}
			case ERPGNPCType::Passive:
				{
					PreviousNPCState = ERPGNPCState::Fleeing;
					break;
				}
			case ERPGNPCType::Shield: { break; }
			}

			CurrentNPCState = ERPGNPCState::Stagger;

			GetCharacterMovement()->SetMovementMode(MOVE_None);
			if (NPCController) NPCController->StopMovement();

			// return to previous state after some time
			FTimerHandle NPCStopStaggerHandle;
			GetWorldTimerManager().SetTimer(NPCStopStaggerHandle, this, &ARPGNPC::ChangeNPCStatePreviousState, .5f, false);

			UE_LOG(LogTemp, Warning, TEXT("NPC -> Stagger"));
			break;
		}
	case ERPGNPCState::Rest:
		{
			// can only change state to rest when in normal state
			PreviousNPCState = ERPGNPCState::Normal;

			CurrentNPCState = ERPGNPCState::Rest;

			GetCharacterMovement()->SetMovementMode(MOVE_None);
			if (NPCController) NPCController->StopMovement();

			UE_LOG(LogTemp, Warning, TEXT("NPC -> Rest"));
			break;
		}
	case ERPGNPCState::Stationary:
		{
			CurrentNPCState = ERPGNPCState::Stationary;

			GetCharacterMovement()->SetMovementMode(MOVE_None);
			if (NPCController) NPCController->StopMovement();

			
			// GetWorldTimerManager().SetTimer(ShieldDestroyHandle, this, &ARPGNPC::ChangeNPCStateDead, 300.f, false);

			UE_LOG(LogTemp, Warning, TEXT("NPC -> Stationary"));
			break;
		}
	case ERPGNPCState::Deactivated:
		{
			CurrentNPCState = ERPGNPCState::Deactivated;

			GetCharacterMovement()->SetMovementMode(MOVE_None);
			if (NPCController) NPCController->StopMovement();

			NPCTargetEnemy = nullptr;
			
			WasAttacked = false;

			SpeedInc = SpeedIncInitial;

			NPCAlreadyAttacked = false;

			// this->SetActorHiddenInGame(true);

			SetActorTickInterval(3.f);

			UE_LOG(LogTemp, Warning, TEXT("NPC -> Deactivated"));
			break;
		}
	case ERPGNPCState::Activated:
		{
			CurrentNPCState = ERPGNPCState::Activated;

			GetCharacterMovement()->SetMovementMode(MOVE_None);
			if (NPCController) NPCController->StopMovement();

			// this->SetActorHiddenInGame(false);

			// SetActorTickInterval(2.f);
			SetActorTickInterval(1.5f);

			FTimerHandle NPCToNormalHandle;
			GetWorldTimerManager().SetTimer(NPCToNormalHandle, this, &ARPGNPC::ChangeNPCStateNormal, .5f, false);

			UE_LOG(LogTemp, Warning, TEXT("NPC -> Activated"));
			break;
		}
	case ERPGNPCState::Dead:
		{
			CurrentNPCState = ERPGNPCState::Dead;

			GetCharacterMovement()->SetMovementMode(MOVE_None);
			if (NPCController) NPCController->StopMovement();

			//GetWorldTimerManager().ClearTimer(ShieldDestroyHandle);

			DeathAction();

			// delete character
			FTimerHandle NPCDeathHandle;
			GetWorldTimerManager().SetTimer(NPCDeathHandle, this, &ARPGNPC::DestroyNPC, 1.f, false);

			UE_LOG(LogTemp, Warning, TEXT("NPC -> Dead"));
			break;
		}
	}
}


// think of a way to get the attacker from this function
void ARPGNPC::NPCTakeDamage(uint8 BodyPart, float Damage, ACharacter* Attacker)
{
	float FinalDamage;
	switch (BodyPart)
	{
	case 0: // head
		{
			Damage *= 1.3f;
			if (NPCHeadArmour) FinalDamage = NPCHeadArmour->CalcDamage(Damage);
			else FinalDamage = Damage;
			break;
		}
	case 1: // chest
		{
			if (NPCChestArmour) FinalDamage = NPCChestArmour->CalcDamage(Damage);
			else FinalDamage = Damage;
			break;
		}
	case 2: // larm
		{
			if (NPCChestArmour) FinalDamage = NPCChestArmour->CalcDamage(Damage);
			else FinalDamage = Damage;
			if (NPCLArmCurrentHealth - FinalDamage > 0.f) NPCLArmCurrentHealth -= FinalDamage;
			else 
			{
				NPCLArmCurrentHealth = 0.f;
				if (!AccuracyDebuff) AccuracyDebuff = true;
			}
			break;
		}
	case 3: // rarm
		{
			if (NPCChestArmour) FinalDamage = NPCChestArmour->CalcDamage(Damage);
			else FinalDamage = Damage;
			if (NPCRArmCurrentHealth - FinalDamage > 0.f) NPCRArmCurrentHealth -= FinalDamage;
			else 
			{
				NPCRArmCurrentHealth = 0.f;
				if (!AccuracyDebuff) AccuracyDebuff = true;
			}
			break;
		}
	case 4: // lleg
		{
			if (NPCLegsArmour) FinalDamage = NPCLegsArmour->CalcDamage(Damage);
			else FinalDamage = Damage;
			if (NPCLLegCurrentHealth - FinalDamage > 0.f) NPCLLegCurrentHealth -= FinalDamage;
			else
			{
				NPCLLegCurrentHealth = 0.f;
				if (!MoveSpeedDebuff) NPCSetMoveSpeedDebuff(true);
			}
			break;
		}
	case 5: // rleg
		{
			if (NPCLegsArmour) FinalDamage = NPCLegsArmour->CalcDamage(Damage);
			else FinalDamage = Damage;
			if (NPCRLegCurrentHealth - FinalDamage > 0.f) NPCRLegCurrentHealth -= FinalDamage;
			else
			{
				NPCRLegCurrentHealth = 0.f;
				if (!MoveSpeedDebuff) NPCSetMoveSpeedDebuff(true);
			}
			break;
		}
	default: // miss
		{
			FinalDamage = 0.f;
			break;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("NPC -> Damage Taken: %f"), FinalDamage);
	
	if (NPCCurrentHealth - FinalDamage <= 0.f)
	{
		NPCCurrentHealth = 0.f;
		ChangeNPCStateDead();
		return;
	}
	NPCCurrentHealth -= FinalDamage;
	WasAttacked = true;
	
	NPCAttackResponse(Attacker);
}

void ARPGNPC::NPCAttackResponse(ACharacter* Attacker)
{
	if (!NPCTargetEnemy) NPCTargetEnemy = Attacker;
	else if (NPCTargetEnemy->ActorHasTag("NPC"))
	{
		if (Cast<ARPGNPC>(NPCTargetEnemy)->GetNPCType() == ERPGNPCType::Passive)
		{
			NPCTargetEnemy = Attacker;
		}
	}

	ChangeNPCStateStagger();
	
	// switch (GetNPCType())
	// {
	// case ERPGNPCType::Aggressive: {}
	// case ERPGNPCType::Neutral:
	// 	{
	// 		ChangeNPCStateCombat();
	// 		break;
	// 	}
	// case ERPGNPCType::Passive:
	// 	{
	// 		ChangeNPCStateFleeing();
	// 		break;
	// 	}
	// }
}

void ARPGNPC::NPCCombatAction()
{
	
}

void ARPGNPC::NPCNextAction()
{
	if (GetNPCClass() == ERPGNPCClass::Trapper) return;
	
	switch (GetNPCType())
	{
	case ERPGNPCType::Aggressive:
		{
			TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
			ObjectTypes.Add(UEngineTypes::ConvertToObjectType((ECollisionChannel::ECC_WorldDynamic)));
			ObjectTypes.Add(UEngineTypes::ConvertToObjectType((ECollisionChannel::ECC_PhysicsBody)));
			ObjectTypes.Add(UEngineTypes::ConvertToObjectType((ECollisionChannel::ECC_Pawn)));
			const TArray<AActor*> IgnoreActor = {this};
			TArray<AActor*> FrontOutActors;
			TArray<AActor*> BackOutActors;

			// 1350.f

			const float ForwardRadius = 875.f;
			const float BackwardRadius = 700.f;
			FVector ForwardPos = GetRootComponent()->GetComponentLocation() + GetRootComponent()->GetForwardVector() * 775;
			FVector BackwardPos = GetRootComponent()->GetComponentLocation() + GetRootComponent()->GetForwardVector() * -600;
			
			UKismetSystemLibrary::DrawDebugSphere(GetWorld(), ForwardPos, ForwardRadius, 12, FColor::Blue, .5f);
			UKismetSystemLibrary::DrawDebugSphere(GetWorld(), BackwardPos, BackwardRadius, 12, FColor::Green, .5f);
			
			if (UKismetSystemLibrary::SphereOverlapActors(GetWorld(), ForwardPos, ForwardRadius, ObjectTypes, ACharacter::StaticClass(), IgnoreActor, FrontOutActors))
			{
				if (FrontOutActors[0]->ActorHasTag("Player") || FrontOutActors[0]->ActorHasTag("NPC")) NPCTargetEnemy = Cast<ACharacter>(FrontOutActors[0]);
				ChangeNPCStateCombat();
			}

			// add backwards
			if (UKismetSystemLibrary::SphereOverlapActors(GetWorld(), BackwardPos, BackwardRadius, ObjectTypes, ACharacter::StaticClass(), IgnoreActor, BackOutActors))
			{
				if (BackOutActors[0]->ActorHasTag("Player") || BackOutActors[0]->ActorHasTag("NPC"))
				{
					ACharacter* BackTarget = Cast<ACharacter>(BackOutActors[0]);
					if (!NPCBackwardsTarget) NPCBackwardsTarget = BackTarget;
					if (NPCBackwardsTarget != BackTarget)
					{
						NPCBackwardsTargetTicks = 0;
						NPCBackwardsTarget = BackTarget;
						return;
					}

					if (++NPCBackwardsTargetTicks >= 3)
					{
						NPCTargetEnemy = NPCBackwardsTarget;
						NPCBackwardsTargetTicks = 0;
						NPCBackwardsTarget = nullptr;
						ChangeNPCStateCombat();
					}
				}
				
			}
		}
	case ERPGNPCType::Neutral: {}
	case ERPGNPCType::Passive:
		{
			// check surroundings for bags, weapons, armour, and items
				// if bag is found compare - new function
					// virtual CheckWeapon(weapon) will compare the weapon with current weapon, depending on the npc - npc rush will always switch weapon if it has a higher damage value
					// virtual CheckArmour(armour) will compare the armour with current armour
					// virtual CheckItems(item) will check that there is space in the inventory and will pick up item, prioritizing certain ones depending on which npc it is 
				// if weapon is found compare to current weapon and equip if better - function above
				// if armour is found compare to current armour abd equip if better - function above
				// ignore item is inventory is full else pick up - function above

			// range ~1000

			// TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
			// ObjectTypes.Add(UEngineTypes::ConvertToObjectType((ECollisionChannel::ECC_GameTraceChannel1)));
			// const TArray<AActor*> IgnoreActor = {this};
			// TArray<AActor*> OutActors;
			//
			// UKismetSystemLibrary::DrawDebugSphere(GetWorld(), GetRootComponent()->GetComponentLocation(), 1000.f, 12, FColor::Blue);
			//
			// if (UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetRootComponent()->GetComponentLocation(), 1000.f,
			// 	ObjectTypes, ACharacter::StaticClass(), IgnoreActor, OutActors))


			FVector Loc = RootComponent->GetComponentLocation();
			Loc.Z = 93.f;
			RootComponent->SetWorldLocation(Loc);
	
			FVector BoxLoc = GetRootComponent()->GetComponentLocation();
			BoxLoc.Z -= 75.f;
			// const FVector BoxExtent = FVector(50.f, 50.f, 15.f);
			const FVector BoxExtent = FVector(250.f, 250.f, 5.f);
			TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
			ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1)); // Dropped
			TArray<AActor*> ActorsToIgnore;
			ActorsToIgnore.Add(this);
			TArray<AActor*> OutActors;

			UKismetSystemLibrary::DrawDebugBox(GetWorld(), BoxLoc, BoxExtent, FColor::Red, FRotator(0.f, 0.f, 0.f), 2.f);
			
			if (UKismetSystemLibrary::BoxOverlapActors(GetWorld(), RootComponent->GetComponentLocation(), BoxExtent, ObjectTypes, nullptr, ActorsToIgnore, OutActors))
			{
				if (OutActors[0]->ActorHasTag("Bag"))
				{
					ARPGBag* Bag = Cast<ARPGBag>(OutActors[0]);
					Bag->BagClosedTimer(false);
					NPCCheckDeathBag(Bag);
					UE_LOG(LogTemp, Warning, TEXT("NPC -> Bag Check"));
					return;
				}
				if (OutActors[0]->ActorHasTag("Weapon"))
				{
					WeaponCheckPriority(Cast<ARPGWeapon>(OutActors[0]));
					UE_LOG(LogTemp, Warning, TEXT("NPC -> Weapon Check"));
					return;
				}
				if (OutActors[0]->ActorHasTag("Throwable"))
				{
					ThrowableCheckPriority(Cast<ARPGThrowable>(OutActors[0]));
					UE_LOG(LogTemp, Warning, TEXT("NPC -> Throwable Check"));
					return;
				}
				if (OutActors[0]->ActorHasTag("Armour"))
				{
					ARPGArmour* Armour = Cast<ARPGArmour>(OutActors[0]);
					switch (Armour->GetArmourType())
					{
					case ERPGArmourType::Head:
						{
							HeadArmourCheckPriority(Armour);
							break;
						}
					case ERPGArmourType::Chest:
						{
							ChestArmourCheckPriority(Armour);
							break;
						}
					case ERPGArmourType::Legs:
						{
							LegsArmourCheckPriority(Armour);
							break;
						}
					}
					UE_LOG(LogTemp, Warning, TEXT("NPC -> Armour Check"));
					return;
				}
				if (OutActors[0]->ActorHasTag("Item"))
				{
					ItemsCheckPriority(Cast<ARPGItem>(OutActors[0]));
					UE_LOG(LogTemp, Warning, TEXT("NPC -> Item Check"));
					return;
				}
			}
			
			break;
		}
	case ERPGNPCType::Shield: { break; }
	}
}


void ARPGNPC::NPCAttack(uint8 BodyPart)
{
	if (!NPCWeapon || !NPCTargetEnemy) return;
	if (GetNPCState() != ERPGNPCState::Normal && GetNPCState() != ERPGNPCState::Combat) return;
	if (NPCWeapon->GetWeaponType() == ERPGWeaponType::Ranged && NPCWeapon->GetWeaponCurrentBulletAmount() <= 0)
	{
		ChangeNPCStateReloading();
		return;
	}
	if (NPCWeapon->GetWeaponType() == ERPGWeaponType::Melee && NPCWeapon->GetWeaponRange() < GetDistanceTo(NPCTargetEnemy))
	{
		UE_LOG(LogTemp, Warning, TEXT("NPC -> enemy outside of range"));
		return;
	}

	BodyPartToShoot = BodyPart;

	ChangeNPCStateShooting();

	if (NPCWeapon->GetWeaponType() == ERPGWeaponType::Ranged)
	{
		FTimerHandle NPCAttackDelayHandle;
		GetWorldTimerManager().SetTimer(NPCAttackDelayHandle, this, &ARPGNPC::NPCAttackDelay, NPCShotDelay, false);
		return;
	}
	
	FTimerHandle NPCMeleeAttackDelayHandle;
	GetWorldTimerManager().SetTimer(NPCMeleeAttackDelayHandle, this, &ARPGNPC::NPCMeleeAttackDelay, NPCShotDelay, false);
	
}

void ARPGNPC::NPCAttackDelay()
{
	if (GetDistanceTo(NPCTargetEnemy) <= 250)
	{
		if (NPCTargetLineOfSight(BodyPartToShoot))
		{
			NPCWeapon->NPCWeaponShoot(NPCTargetEnemy, BodyPartToShoot, true, this);
			UE_LOG(LogTemp, Warning, TEXT("NPC -> %d"), BodyPartToShoot);

			NPCCheckTargetDead(NPCTargetEnemy);
		}
		return;
	}
	
	double ShotAccuracy;
	if (!AccuracyDebuff) ShotAccuracy = NPCWeapon->GetWeaponAccuracy() * NPCAccuracy;
	else ShotAccuracy = NPCWeapon->GetWeaponAccuracy() * (NPCAccuracy * DebuffVal);

	
	if (FMath::RandRange(0.00, 1.00) <= ShotAccuracy)
	{
		// TargetNPC->NPCTakeDamage(BodyPart, CurrentWeapon->GetWeaponDamage());
		
		if (NPCTargetLineOfSight(BodyPartToShoot))
		{
			NPCWeapon->NPCWeaponShoot(NPCTargetEnemy, BodyPartToShoot, false, this);
			UE_LOG(LogTemp, Warning, TEXT("NPC -> %d"), BodyPartToShoot);

			NPCCheckTargetDead(NPCTargetEnemy);
		}
		
		return;
	}

	TArray ShotLocations = {0, 1, 2, 3, 4, 5};
	ShotLocations[BodyPartToShoot] = 6;
	
	// TargetNPC->NPCTakeDamage(FMath::RandRange(0, 6), CurrentWeapon->GetWeaponDamage());
	BodyPartToShoot = FMath::RandRange(0, 5);

	if (NPCTargetLineOfSight(ShotLocations[BodyPartToShoot]))
	{
		NPCWeapon->NPCWeaponShoot(NPCTargetEnemy, ShotLocations[BodyPartToShoot], false, this);
		UE_LOG(LogTemp, Warning, TEXT("NPC -> %d -> %d"), BodyPartToShoot, ShotLocations[BodyPartToShoot]);

		NPCCheckTargetDead(NPCTargetEnemy);
	}
}

void ARPGNPC::NPCMeleeAttackDelay()
{
	FHitResult Out;
	FCollisionQueryParams CSQueryP;
	CSQueryP.TraceTag = "TraceTag";
	CSQueryP.AddIgnoredActor(NPCTargetEnemy);

	if (GetWorld()->LineTraceSingleByChannel(Out, NPCTargetEnemy->GetActorLocation(),
		NPCTargetEnemy->GetActorLocation() + NPCTargetEnemy->GetActorForwardVector() * -150, ECollisionChannel::ECC_PhysicsBody, CSQueryP))
	{
		if (Out.GetActor() == this)
		{
			if (NPCTargetLineOfSight(BodyPartToShoot))
			{
				NPCWeapon->NPCWeaponShoot(NPCTargetEnemy, BodyPartToShoot, true, this);
				UE_LOG(LogTemp, Warning, TEXT("%d"), BodyPartToShoot);

				NPCCheckTargetDead(NPCTargetEnemy);
			}
		}
		return;
	}

	if (NPCTargetLineOfSight(BodyPartToShoot))
	{
		NPCWeapon->NPCWeaponShoot(NPCTargetEnemy, BodyPartToShoot, false, this);
		UE_LOG(LogTemp, Warning, TEXT("%d"), BodyPartToShoot);

		NPCCheckTargetDead(NPCTargetEnemy);
	}
	
}

bool ARPGNPC::NPCTargetLineOfSight(uint8 ShotLocation)
{
	FHitResult out;
	FCollisionQueryParams CSQueryP;
	CSQueryP.TraceTag = "TraceTag";
	CSQueryP.AddIgnoredActor(this);
	
	switch (ShotLocation)
	{
	case 0:
		{
			FVector PlayerLoc = RootComponent->GetComponentLocation();
			FVector SocketLoc = NPCTargetEnemy->GetMesh()->GetSocketLocation("CheckHead");
	
			if (GetWorld()->LineTraceSingleByChannel(out, PlayerLoc, SocketLoc, ECollisionChannel::ECC_PhysicsBody, CSQueryP))
			{
				if (out.GetActor()->ActorHasTag("NPC") || out.GetActor()->ActorHasTag("Player"))
				{
					if (out.GetActor() == NPCTargetEnemy)
					{
						UE_LOG(LogTemp, Warning, TEXT("NPC -> ENEMY HIT"));
						return true;
					}
					NPCStrayBulletAttack(Cast<ACharacter>(out.GetActor()));
				}
				return false;
			}
			
			return false;
		}
	case 1:
		{
			FVector PlayerLoc = RootComponent->GetComponentLocation();
			FVector SocketLoc1 = NPCTargetEnemy->GetMesh()->GetSocketLocation("CheckChestL");
			FVector SocketLoc2 = NPCTargetEnemy->GetMesh()->GetSocketLocation("CheckChestR");
			
			if (GetWorld()->LineTraceSingleByChannel(out, PlayerLoc, SocketLoc1, ECollisionChannel::ECC_PhysicsBody, CSQueryP))
			{
				if (out.GetActor()->ActorHasTag("NPC") || out.GetActor()->ActorHasTag("Player"))
				{
					if (out.GetActor() == NPCTargetEnemy)
					{
						UE_LOG(LogTemp, Warning, TEXT("NPC -> ENEMY HIT"));
						return true;
					}
					NPCStrayBulletAttack(Cast<ACharacter>(out.GetActor()));
					return false;
				}
				if (GetWorld()->LineTraceSingleByChannel(out, PlayerLoc, SocketLoc2, ECollisionChannel::ECC_PhysicsBody, CSQueryP))
				{
					if (out.GetActor()->ActorHasTag("NPC") || out.GetActor()->ActorHasTag("Player"))
					{
						if (out.GetActor() == NPCTargetEnemy)
						{
							UE_LOG(LogTemp, Warning, TEXT("NPC -> ENEMY HIT"));
							return true;
						}
						NPCStrayBulletAttack(Cast<ACharacter>(out.GetActor()));
					}
				}
				return false;
			}
			
			return false;
		}
	case 2:
		{
			FVector PlayerLoc = RootComponent->GetComponentLocation();
			FVector SocketLoc = NPCTargetEnemy->GetMesh()->GetSocketLocation("CheckLArm");
	
			if (GetWorld()->LineTraceSingleByChannel(out, PlayerLoc, SocketLoc, ECollisionChannel::ECC_PhysicsBody, CSQueryP))
			{
				if (out.GetActor()->ActorHasTag("NPC") || out.GetActor()->ActorHasTag("Player"))
				{
					if (out.GetActor() == NPCTargetEnemy)
					{
						UE_LOG(LogTemp, Warning, TEXT("NPC -> ENEMY HIT"));
						return true;
					}
					NPCStrayBulletAttack(Cast<ACharacter>(out.GetActor()));
				}
				return false;
			}
			
			return false;
		}
	case 3:
		{
			FVector PlayerLoc = RootComponent->GetComponentLocation();
			FVector SocketLoc = NPCTargetEnemy->GetMesh()->GetSocketLocation("CheckRArm");
	
			if (GetWorld()->LineTraceSingleByChannel(out, PlayerLoc, SocketLoc, ECollisionChannel::ECC_PhysicsBody, CSQueryP))
			{
				if (out.GetActor()->ActorHasTag("NPC") || out.GetActor()->ActorHasTag("Player"))
				{
					if (out.GetActor() == NPCTargetEnemy)
					{
						UE_LOG(LogTemp, Warning, TEXT("NPC -> ENEMY HIT"));
						return true;
					}
					NPCStrayBulletAttack(Cast<ACharacter>(out.GetActor()));
				}
				return false;
			}
			
			return false;
		}
	case 4:
		{
			FVector PlayerLoc = RootComponent->GetComponentLocation();
			FVector SocketLoc = NPCTargetEnemy->GetMesh()->GetSocketLocation("CheckLLeg");
	
			if (GetWorld()->LineTraceSingleByChannel(out, PlayerLoc, SocketLoc, ECollisionChannel::ECC_PhysicsBody, CSQueryP))
			{
				if (out.GetActor()->ActorHasTag("NPC") || out.GetActor()->ActorHasTag("Player"))
				{
					if (out.GetActor() == NPCTargetEnemy)
					{
						UE_LOG(LogTemp, Warning, TEXT("NPC -> ENEMY HIT"));
						return true;
					}
					NPCStrayBulletAttack(Cast<ACharacter>(out.GetActor()));
				}
				return false;
			}
			
			return false;
		}
	case 5:
		{
			FVector PlayerLoc = RootComponent->GetComponentLocation();
			FVector SocketLoc = NPCTargetEnemy->GetMesh()->GetSocketLocation("CheckRLeg");
	
			if (GetWorld()->LineTraceSingleByChannel(out, PlayerLoc, SocketLoc, ECollisionChannel::ECC_PhysicsBody, CSQueryP))
			{
				if (out.GetActor()->ActorHasTag("NPC") || out.GetActor()->ActorHasTag("Player"))
				{
					if (out.GetActor() == NPCTargetEnemy)
					{
						UE_LOG(LogTemp, Warning, TEXT("NPC -> ENEMY HIT"));
						return true;
					}
					NPCStrayBulletAttack(Cast<ACharacter>(out.GetActor()));
				}
				return false;
			}
			
			return false;
		}
	case 6:
		{
			FVector PlayerLoc = RootComponent->GetComponentLocation();
			FVector MissLoc;
			float DistanceToTarget = GetDistanceTo(NPCTargetEnemy);
			
			UE_LOG(LogTemp, Warning, TEXT("NPC -> distance: %f"), DistanceToTarget);

			FString MissString = "CheckMiss";
			FString MissStringTemp = MissString;
			uint8 TargetMissSocketNum;
			if (NPCTargetEnemy->ActorHasTag("Player")) TargetMissSocketNum = Cast<ARPGCharacter>(NPCTargetEnemy)->GetMissSocketNumber();
			else TargetMissSocketNum = Cast<ARPGNPC>(NPCTargetEnemy)->GetMissSocketNumber();
			uint8 MissNum = FMath::RandRange(0, TargetMissSocketNum-1);
			MissStringTemp.AppendInt(MissNum);

			MissLoc = NPCTargetEnemy->GetMesh()->GetSocketLocation(FName(*MissStringTemp));
			UE_LOG(LogTemp, Warning, TEXT("NPC -> MissSocket: %s"), *MissStringTemp);
			
			if (GetWorld()->LineTraceSingleByChannel(out, PlayerLoc, PlayerLoc + UKismetMathLibrary::FindLookAtRotation(PlayerLoc, MissLoc).Vector() * 2050, 
				ECollisionChannel::ECC_PhysicsBody, CSQueryP))
			{
				if (out.GetActor()->ActorHasTag("NPC") || out.GetActor()->ActorHasTag("Player"))
				{
					ACharacter* CrossFireTarget = nullptr;
					if (out.GetActor() == NPCTargetEnemy)
					{

						// clean up																																					agfafsadfsfsdafsdfsdf
						
						for (uint8 i = 0; i < (TargetMissSocketNum/3)-1; i++)
						{
							MissStringTemp = MissString;
							if (MissNum + 3 > TargetMissSocketNum-1)
							{
								uint8 Leftover = (MissNum + 3) - TargetMissSocketNum-1;
								MissNum = Leftover;	
							} else MissNum += 3;
							MissStringTemp.AppendInt(MissNum);

							MissLoc = NPCTargetEnemy->GetMesh()->GetSocketLocation(FName(*MissStringTemp));
							UE_LOG(LogTemp, Warning, TEXT("NPC -> MissSocket: %s"), *MissStringTemp);
							
							if (GetWorld()->LineTraceSingleByChannel(out, PlayerLoc, PlayerLoc + UKismetMathLibrary::FindLookAtRotation(PlayerLoc, MissLoc).Vector() * 2050, 
								ECollisionChannel::ECC_PhysicsBody, CSQueryP))
							{
								if (out.GetActor()->ActorHasTag("NPC") || out.GetActor()->ActorHasTag("Player"))
								{
									if (out.GetActor() != NPCTargetEnemy)
									{
										CrossFireTarget = Cast<ACharacter>(out.GetActor());
										break;
									}
								} else return true;
							} else return true;
						}
					} else CrossFireTarget = Cast<ACharacter>(out.GetActor());
					// deal damage to other 
					if (CrossFireTarget) NPCStrayBulletAttack(CrossFireTarget);
				}
			}
			return true;
		}
	default: { return false; }
	}
}

void ARPGNPC::NPCStrayBulletAttack(ACharacter* Target)
{
	NPCWeapon->NPCWeaponShoot(Target, 1, false, this);
	UE_LOG(LogTemp, Warning, TEXT("NPC -> PLAYER/NPC HIT BY STRAY BULLET"));

	NPCCheckTargetDead(Target);
}

bool ARPGNPC::NPCCheckTargetDead(ACharacter* Target)
{
	if (Target->ActorHasTag("Player"))
	{
		if (Cast<ARPGCharacter>(Target)->GetCharacterState() != ERPGCharacterState::Dead) return false;
	}
	else
	{
		if (Cast<ARPGNPC>(Target)->GetNPCState() != ERPGNPCState::Dead) return false;
	}

	if (Target == NPCTargetEnemy) NPCTargetEnemy = nullptr;

	// change state ????																																						fasfasfasfdsfdsfasdf

	UE_LOG(LogTemp, Warning, TEXT("NPC -> PLAYER/NPC KILLED"));
	
	return true;
}

void ARPGNPC::NPCFaceTarget()
{
	if (!NPCTargetEnemy) return;

	const FRotator NPCRot = UKismetMathLibrary::FindLookAtRotation(RootComponent->GetComponentLocation(), NPCTargetEnemy->GetActorLocation());

	RootComponent->SetWorldRotation(NPCRot);
}

void ARPGNPC::NPCWeaponPickUp(ARPGWeapon* Weapon)
{
	if (NPCWeapon)
	{
		// detach current weapon, move it to the ground and start rendering it
		NPCWeaponDrop();
		// stop rendering new weapon and attach it to the weapon socket
		
		Weapon->SetActorHiddenInGame(true);
		const FAttachmentTransformRules AttachRules(EAttachmentRule::KeepRelative, true);
		Weapon->AttachToComponent(GetMesh(), AttachRules, "InventorySocket");
		Weapon->SetDroppedWeapon(false);

		NPCWeapon = Weapon;
		return;
	}
	Weapon->SetActorHiddenInGame(true);
	const FAttachmentTransformRules AttachRules(EAttachmentRule::KeepRelative, true);
	Weapon->AttachToComponent(GetMesh(), AttachRules, "InventorySocket");
	Weapon->SetDroppedWeapon(false);

	NPCWeapon = Weapon;
}

void ARPGNPC::NPCWeaponDrop()
{
	if (!NPCWeapon) return;

	ARPGWeapon* Weapon = NPCWeapon;
	NPCWeapon = nullptr;

	// position to drop the weapon
	FVector DropPos = RootComponent->GetComponentLocation();
	DropPos.Z -= 49.f;

	// FDetachmentTransformRules DetachRules;
	// DetachRules.LocationRule(DropPos);
	Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Weapon->SetActorLocation(DropPos);
	Weapon->SetActorRotation(FRotator(0.f, 0.f, 0.f));
	Weapon->SetActorHiddenInGame(false);
	Weapon->SetActorEnableCollision(true);
	Weapon->SetDroppedWeapon(true);
}

bool ARPGNPC::NPCItemPickUp(ARPGItem* Item)
{
	for (uint8 i = 0; i < NPCInventory->GetInventorySize(); i++)
	{
		if (NPCInventory->GetItemInventory()[i] == nullptr)
		{
			Item->SetActorHiddenInGame(true);
			const FAttachmentTransformRules AttachRules(EAttachmentRule::KeepRelative, true);
			Item->AttachToComponent(GetMesh(), AttachRules, "InventorySocket");

			Item->SetDroppedItem(false);
			NPCInventory->SetItemInventory(Item, i);
			
			return true;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("NPC -> INVENTORY FULL"));
	return false;
}

void ARPGNPC::NPCItemDrop(uint8 Pos)
{
	ARPGItem* Item = GetNPCInventory()->GetItemInventory()[Pos];

	GetNPCInventory()->SetItemInventory(nullptr, Pos);
	
	// position to drop the item
	FVector DropPos = RootComponent->GetComponentLocation();
	DropPos.Z -= 49.f;

	// FDetachmentTransformRules DetachRules;
	// DetachRules.LocationRule(DropPos);
	Item->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Item->SetActorLocation(DropPos);
	Item->SetActorRotation(FRotator(0.f, 0.f, 0.f));
	Item->SetActorHiddenInGame(false);
	Item->SetActorEnableCollision(true);
	Item->SetDroppedItem(true);
}

void ARPGNPC::NPCArmourPickUp(ARPGArmour* Armour)
{
	const FAttachmentTransformRules AttachRules(EAttachmentRule::KeepRelative, true);
	switch (Armour->GetArmourType())
	{
	case ERPGArmourType::Head:
		{
			if (GetNPCHeadArmour())
			{
				NPCArmourDrop(ERPGArmourType::Head);
				Armour->SetActorHiddenInGame(true);
				Armour->AttachToComponent(GetMesh(), AttachRules, "InventorySocket");
				Armour->SetDroppedArmour(false);
				NPCHeadArmour = Armour;
				UE_LOG(LogTemp, Warning, TEXT("NPC -> HEAD ARMOUR PICKED UP"));
				return;
			}
			Armour->SetActorHiddenInGame(true);
			Armour->AttachToComponent(GetMesh(), AttachRules, "InventorySocket");
			Armour->SetDroppedArmour(false);
			NPCHeadArmour = Armour;
			UE_LOG(LogTemp, Warning, TEXT("NPC -> HEAD ARMOUR PICKED UP"));
			break;
		}
	case ERPGArmourType::Chest:
		{
			if (GetNPCChestArmour())
			{
				NPCArmourDrop(ERPGArmourType::Chest);
				Armour->SetActorHiddenInGame(true);
				Armour->AttachToComponent(GetMesh(), AttachRules, "InventorySocket");
				Armour->SetDroppedArmour(false);
				NPCChestArmour = Armour;
				UE_LOG(LogTemp, Warning, TEXT("NPC -> CHEST ARMOUR PICKED UP"));
				return;
			}
			Armour->SetActorHiddenInGame(true);
			Armour->AttachToComponent(GetMesh(), AttachRules, "InventorySocket");
			Armour->SetDroppedArmour(false);
			NPCChestArmour = Armour;
			UE_LOG(LogTemp, Warning, TEXT("NPC -> CHEST ARMOUR PICKED UP"));
			break;
		}
	case ERPGArmourType::Legs:
		{
			if (GetNPCLegsArmour())
			{
				NPCArmourDrop(ERPGArmourType::Legs);
				Armour->SetActorHiddenInGame(true);
				Armour->AttachToComponent(GetMesh(), AttachRules, "InventorySocket");
				Armour->SetDroppedArmour(false);
				NPCLegsArmour = Armour;
				UE_LOG(LogTemp, Warning, TEXT("NPC -> LEGS ARMOUR PICKED UP"));
				return;
			}
			Armour->SetActorHiddenInGame(true);
			Armour->AttachToComponent(GetMesh(), AttachRules, "InventorySocket");
			Armour->SetDroppedArmour(false);
			NPCLegsArmour = Armour;
			UE_LOG(LogTemp, Warning, TEXT("NPC -> LEGS ARMOUR PICKED UP"));
			break;
		}
	}
}

void ARPGNPC::NPCArmourDrop(ERPGArmourType Type)
{
	// position to drop the armour
	FVector DropPos = RootComponent->GetComponentLocation();
	DropPos.Z -= 49.f;
	switch (Type)
	{
	case ERPGArmourType::Head:
		{
			NPCHeadArmour->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			NPCHeadArmour->SetActorLocation(DropPos);
			NPCHeadArmour->SetActorRotation(FRotator(0.f, 0.f, 0.f));
			NPCHeadArmour->SetActorHiddenInGame(false);
			NPCHeadArmour->SetActorEnableCollision(true);
			NPCHeadArmour->SetDroppedArmour(true);
			NPCHeadArmour = nullptr;
			UE_LOG(LogTemp, Warning, TEXT("NPC -> HEAD ARMOUR DROPPED"));
			break;
		}
	case ERPGArmourType::Chest:
		{
			NPCChestArmour->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			NPCChestArmour->SetActorLocation(DropPos);
			NPCChestArmour->SetActorRotation(FRotator(0.f, 0.f, 0.f));
			NPCChestArmour->SetActorHiddenInGame(false);
			NPCChestArmour->SetActorEnableCollision(true);
			NPCChestArmour->SetDroppedArmour(true);
			NPCChestArmour = nullptr;
			UE_LOG(LogTemp, Warning, TEXT("NPC -> CHEST ARMOUR DROPPED"));
			break;
		}
	case ERPGArmourType::Legs:
		{
			NPCLegsArmour->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			NPCLegsArmour->SetActorLocation(DropPos);
			NPCLegsArmour->SetActorRotation(FRotator(0.f, 0.f, 0.f));
			NPCLegsArmour->SetActorHiddenInGame(false);
			NPCLegsArmour->SetActorEnableCollision(true);
			NPCLegsArmour->SetDroppedArmour(true);
			NPCLegsArmour = nullptr;
			UE_LOG(LogTemp, Warning, TEXT("NPC -> LEGS ARMOUR DROPPED"));
			break;
		}
	}
}

void ARPGNPC::NPCThrowablePickUp(ARPGThrowable* Throwable)
{
	const FAttachmentTransformRules AttachRules(EAttachmentRule::KeepRelative, true);

	NPCThrowableUnEquip();
	
	Throwable->SetActorHiddenInGame(true);
	Throwable->AttachToComponent(GetMesh(), AttachRules, "InventorySocket");
	Throwable->SetDroppedItem(false);
	NPCThrowable = Throwable;
}

void ARPGNPC::NPCThrowableDrop(ARPGThrowable* Throwable)
{
	// position to drop the throwable weapon
	FVector DropPos = RootComponent->GetComponentLocation();
	DropPos.Z -= 49.f;
	
	Throwable->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Throwable->SetActorLocation(DropPos);
	Throwable->SetActorRotation(FRotator(0.f, 0.f, 0.f));
	Throwable->SetActorHiddenInGame(false);
	Throwable->SetActorEnableCollision(true);
	Throwable->SetDroppedItem(true);
}

bool ARPGNPC::NPCThrowableEquip()
{
	const uint8 ThrowablePos = NPCCheckInventory("Throwable");
	if (ThrowablePos == 99) return false;
	
	NPCThrowableUnEquip();
	
	NPCThrowable = Cast<ARPGThrowable>(GetNPCInventory()->GetItemInventory()[ThrowablePos]);

	GetNPCInventory()->SetItemInventory(nullptr, ThrowablePos);

	return true;
}

void ARPGNPC::NPCThrowableUnEquip()
{
	if (!NPCThrowable) return;

	if (!NPCAddToInventory(NPCThrowable))
	{
		NPCThrowableDrop(NPCThrowable);
	}
	NPCThrowable = nullptr;
}

bool ARPGNPC::NPCAddToInventory(ARPGItem* Item)
{
	for (uint8 i = 0; i < GetNPCInventory()->GetInventorySize(); i++)
	{
		if (GetNPCInventory()->GetItemInventory()[i] == nullptr)
		{
			GetNPCInventory()->SetItemInventory(Item, i);
			return true;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("NPC -> INVENTORY FULL"));
	return false;
}

void ARPGNPC::NPCLevelUp()
{
	// give random values (0-10) to npc stats and apply them
	Health = FMath::RandRange(0, 10);
	for (uint8 i = 0; i < Health; i++)
	{
		NPCMaxHealth += 5;
		NPCLArmMaxHealth += 5;
		NPCRArmMaxHealth += 5;
		NPCLLegMaxHealth += 5;
		NPCRLegMaxHealth += 5;
	}
	NPCCurrentHealth = NPCMaxHealth;
	NPCLArmCurrentHealth = NPCLArmMaxHealth;
	NPCRArmCurrentHealth = NPCRArmMaxHealth;
	NPCLLegCurrentHealth = NPCLLegMaxHealth;
	NPCRLegCurrentHealth = NPCRLegMaxHealth;
	
	Strength = FMath::RandRange(0, 10);
	for (uint8 i = 0; i < Strength; i++)
	{
		NPCStrength += .1f;
	}
	
	Accuracy = FMath::RandRange(0, 10);
	for (uint8 i = 0; i < Accuracy; i++)
	{
		NPCAccuracy += .05;
	}
	
	Agility = FMath::RandRange(0, 10);
	for (uint8 i = 0; i < Agility; i++)
	{
		WalkSpeed += 5.f;
		CrouchSpeed += 5.f;
	}
	NPCChangeMoveSpeed();
}

void ARPGNPC::NPCChangeMoveSpeed(const float Modifier)
{
	if (!MoveSpeedDebuff)
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed * Modifier;
		GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed * Modifier;
		return;
	}
	GetCharacterMovement()->MaxWalkSpeed = (WalkSpeed * DebuffVal) * Modifier;
	GetCharacterMovement()->MaxWalkSpeedCrouched = (CrouchSpeed * DebuffVal) * Modifier;
}

void ARPGNPC::NPCSetMoveSpeedDebuff(const bool Value)
{
	MoveSpeedDebuff = Value;
	if (GetNPCState() == ERPGNPCState::Reloading)
	{
		NPCChangeMoveSpeed(.5f);
		return;
	}
	NPCChangeMoveSpeed();
}

void ARPGNPC::CheckHealRequired(uint8 Priority)
{
	switch (Priority)
	{
	default: {}
	case 0:
		{
			if (GetNPCLArmCurrentHealth() == GetNPCLArmMaxHealth() || GetNPCRArmCurrentHealth() == GetNPCRArmMaxHealth() ||
				GetNPCLLegCurrentHealth() == GetNPCLLegMaxHealth() || GetNPCRLegCurrentHealth() == GetNPCRLegMaxHealth())
			{
				if (GetNPCCurrentHealth() < GetNPCMaxHealth()/2) NPCHeal(0);
			}
			if (GetNPCLArmCurrentHealth() < GetNPCLArmMaxHealth()/2)  NPCHeal(2);
			else if (GetNPCRArmCurrentHealth() < GetNPCRArmMaxHealth()/2)  NPCHeal(3);
			else if (GetNPCLLegCurrentHealth() < GetNPCLLegMaxHealth()/2)  NPCHeal(4);
			else if (GetNPCRLegCurrentHealth() < GetNPCRLegMaxHealth()/2)  NPCHeal(5);
			break;
		}
	case 1:
		{
			if (GetNPCLArmCurrentHealth() == GetNPCLArmMaxHealth() || GetNPCRArmCurrentHealth() == GetNPCRArmMaxHealth() ||
				GetNPCLLegCurrentHealth() == GetNPCLLegMaxHealth() || GetNPCRLegCurrentHealth() == GetNPCRLegMaxHealth())
			{
				if (GetNPCCurrentHealth() < GetNPCMaxHealth()/2) NPCHeal(0);
			}
			if (GetNPCLLegCurrentHealth() < GetNPCLLegMaxHealth()/2)  NPCHeal(4);
			else if (GetNPCRLegCurrentHealth() < GetNPCRLegMaxHealth()/2)  NPCHeal(5);
			else if (GetNPCLArmCurrentHealth() < GetNPCLArmMaxHealth()/2)  NPCHeal(2);
			else if (GetNPCRArmCurrentHealth() < GetNPCRArmMaxHealth()/2)  NPCHeal(3);
			break;
		}
	}
}

void ARPGNPC::NPCHeal(uint8 BodyPart)
{
	if (GetNPCState() != ERPGNPCState::Normal && GetNPCState() != ERPGNPCState::Combat) return;

	NPCSelectedItemID = NPCCheckInventory("Heal");
	if (NPCSelectedItemID == 99) return;
	NPCSelectedItem = GetNPCInventory()->GetItemInventory()[NPCSelectedItemID];

	BodyPartToShoot = BodyPart;

	ChangeNPCStateHeal();

	FTimerHandle NPCHealDelayHandle;
	GetWorldTimerManager().SetTimer(NPCHealDelayHandle, this, &ARPGNPC::NPCHealDelay, GetNPCHealDuration() - .3f, false);
}

void ARPGNPC::NPCHealDelay()
{
	NPCCurrentHealth = GetNPCMaxHealth();
	NPCLArmCurrentHealth = GetNPCLArmMaxHealth();
	NPCRArmCurrentHealth = GetNPCRArmMaxHealth();
	NPCLLegCurrentHealth = GetNPCLLegMaxHealth();
	NPCRLegCurrentHealth = GetNPCRLegMaxHealth();
	if (AccuracyDebuff) AccuracyDebuff = false;
	if (MoveSpeedDebuff) NPCSetMoveSpeedDebuff(false);
	NPCDestroySelectedItem();
	UE_LOG(LogTemp, Warning, TEXT("NPC -> Heal"));

	
	// switch (BodyPartToHeal)
	// {
	// case 0: {}	// head
	// case 1:		// chest
	// 	{
	// 		// ChangeNPCStateHeal();
	// 		NPCCurrentHealth = GetNPCMaxHealth();
	// 		if (MoveSpeedDebuff) NPCSetMoveSpeedDebuff(false);
	//
	// 		NPCDestroySelectedItem();
	// 		UE_LOG(LogTemp, Warning, TEXT("NPC -> Head/Chest Heal"));
	// 		break;
	// 	}
	// case 2:		// larm 
	// 	{
	// 		// ChangeNPCStateHeal();
	//
	// 		const float HealValue = GetNPCLArmMaxHealth() - GetNPCLArmCurrentHealth();
	// 		GetNPCCurrentHealth() + HealValue >= GetNPCMaxHealth() ? NPCCurrentHealth = GetNPCMaxHealth() : NPCCurrentHealth += HealValue;
	// 		
	// 		NPCLArmCurrentHealth = GetNPCLArmMaxHealth();
	//		if (AccuracyDebuff && GetNPCRArmCurrentHealth() > 0.f) AccuracyDebuff = false;
	//
	// 		NPCDestroySelectedItem();
	// 		UE_LOG(LogTemp, Warning, TEXT("NPC -> Left Arm Heal"));
	// 		break;
	// 	}
	// case 3:		// rarm
	// 	{
	// 		// ChangeNPCStateHeal();
	//
	// 		const float HealValue = GetNPCRArmMaxHealth() - GetNPCRArmCurrentHealth();
	// 		GetNPCCurrentHealth() + HealValue >= GetNPCMaxHealth() ? NPCCurrentHealth = GetNPCMaxHealth() : NPCCurrentHealth += HealValue;
	// 		
	// 		NPCRArmCurrentHealth = GetNPCRArmMaxHealth();
	// 		if (AccuracyDebuff && GetNPCLArmCurrentHealth() > 0.f) AccuracyDebuff = false;
	//
	// 		NPCDestroySelectedItem();
	// 		UE_LOG(LogTemp, Warning, TEXT("NPC -> Right Arm Heal"));
	// 		break;
	// 	}
	// case 4:		// lleg
	// 	{
	// 		// ChangeNPCStateHeal();
	//
	// 		const float HealValue = GetNPCLLegMaxHealth() - GetNPCLLegCurrentHealth();
	// 		GetNPCCurrentHealth() + HealValue >= GetNPCMaxHealth() ? NPCCurrentHealth = GetNPCMaxHealth() : NPCCurrentHealth += HealValue;
	// 		
	// 		NPCLLegCurrentHealth = GetNPCLLegMaxHealth();
	// 		if (MoveSpeedDebuff) NPCSetMoveSpeedDebuff(false);
	//
	// 		NPCDestroySelectedItem();
	// 		UE_LOG(LogTemp, Warning, TEXT("NPC -> Left leg Heal"));
	// 		break;
	// 	}
	// case 5:		// rleg
	// 	{
	// 		// ChangeNPCStateHeal();
	//
	// 		const float HealValue = GetNPCRLegMaxHealth() - GetNPCRLegCurrentHealth();
	// 		GetNPCCurrentHealth() + HealValue >= GetNPCMaxHealth() ? NPCCurrentHealth = GetNPCMaxHealth() : NPCCurrentHealth += HealValue;
	// 		
	// 		NPCRLegCurrentHealth = GetNPCRLegMaxHealth();
	// 		if (MoveSpeedDebuff) NPCSetMoveSpeedDebuff(false);
	//
	// 		NPCDestroySelectedItem();
	// 		UE_LOG(LogTemp, Warning, TEXT("NPC -> Right Leg Heal"));
	// 		break;
	// 	}
	// default: { break; }
	// }
}

void ARPGNPC::NPCDestroySelectedItem()
{
	if (!NPCSelectedItem) return;

	NPCSelectedItem->ItemUsed();

	GetNPCInventory()->SetItemInventory(nullptr, NPCSelectedItemID);

	NPCSelectedItem = nullptr;
}

void ARPGNPC::CheckRepairRequired()
{
	uint8 Count = 0;
	if (GetNPCHeadArmour())
		if (GetNPCHeadArmour()->GetArmourCurrentDurability() <= GetNPCHeadArmour()->GetArmourMaxDurability() / 3)
			++Count;
	if (GetNPCChestArmour())
		if (GetNPCChestArmour()->GetArmourCurrentDurability() <= GetNPCChestArmour()->GetArmourMaxDurability() / 3)
			++Count;
	if (GetNPCLegsArmour())
		if (GetNPCLegsArmour()->GetArmourCurrentDurability() <= GetNPCLegsArmour()->GetArmourMaxDurability() / 3)
			++Count;

	if (Count >= 2) NPCRepairArmour();
}

void ARPGNPC::NPCRepairArmour()
{
	if (GetNPCState() != ERPGNPCState::Normal) return;

	NPCSelectedItemID = NPCCheckInventory("Repair");
	if (NPCSelectedItemID == 99) return;
	NPCSelectedItem = GetNPCInventory()->GetItemInventory()[NPCSelectedItemID];

	if (GetNPCHeadArmour()) GetNPCHeadArmour()->RepairArmour();
	if (GetNPCChestArmour()) GetNPCHeadArmour()->RepairArmour();
	if (GetNPCLegsArmour()) GetNPCHeadArmour()->RepairArmour();

	NPCDestroySelectedItem();
	UE_LOG(LogTemp, Warning, TEXT("NPC -> Armour Repaired"));
}

uint8 ARPGNPC::NPCCheckInventory(FName Tag)
{
	for (uint8 i = 0; i < GetNPCInventory()->GetInventorySize(); i++)
	{
		if (GetNPCInventory()->GetItemInventory()[i])
		{
			if (GetNPCInventory()->GetItemInventory()[i]->ActorHasTag(Tag))
			{
				return i;
			}
		}
	}
	return 99;
}

uint8 ARPGNPC::NPCCountInventory(FName Tag)
{
	uint8 Num = 0;
	for (uint8 i = 0; i < GetNPCInventory()->GetInventorySize(); i++)
	{
		if (GetNPCInventory()->GetItemInventory()[i])
		{
			if (GetNPCInventory()->GetItemInventory()[i]->ActorHasTag(Tag))
			{
				++Num;
			}
		}
	}
	return Num;
}

void ARPGNPC::MoveToEnemy()
{
	if (NPCController && NPCTargetEnemy) NPCController->MoveToActor(GetNPCTargetEnemy(), -1, true, true, true);
}





void ARPGNPC::SetUpDeathBag()
{
	if (GetNPCType() == ERPGNPCType::Shield) return;
	
	if (NPCWeapon) NPCDeathBag->SetBagWeapon(NPCWeapon);
	if (NPCThrowable) NPCDeathBag->SetBagThrowable(NPCThrowable);
	if (NPCHeadArmour) NPCDeathBag->SetBagHeadArmour(NPCHeadArmour);
	if (NPCChestArmour) NPCDeathBag->SetBagChestArmour(NPCChestArmour);
	if (NPCLegsArmour) NPCDeathBag->SetBagLegsArmour(NPCLegsArmour);
	if (NPCInventory) NPCDeathBag->SetBagInventory(NPCInventory);
	
	// position to drop the item
	FVector DropPos = RootComponent->GetComponentLocation();
	DropPos.Z -= 49.f;

	// FDetachmentTransformRules DetachRules;
	// DetachRules.LocationRule(DropPos);
	NPCDeathBag->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	NPCDeathBag->SetActorLocation(DropPos);
	NPCDeathBag->SetActorRotation(FRotator(0.f, 0.f, 0.f));
	NPCDeathBag->SetActorHiddenInGame(false);

	NPCDeathBag->SetDroppedBag();
}

void ARPGNPC::DestroyNPC()
{
	SetUpDeathBag();
	this->Destroy();
}

void ARPGNPC::NPCThrow()
{
	if (!NPCThrowable) if (!NPCThrowableEquip()) return;
	
	if (NPCThrowable->GetThrowableType() != ERPGThrowableType::Shield && !NPCTargetEnemy) return;
	if (GetNPCState() != ERPGNPCState::Normal && GetNPCState() != ERPGNPCState::Combat) return;

	ChangeNPCStateThrowing();

	FTimerHandle NPCThrowDelayHandle;
	GetWorldTimerManager().SetTimer(NPCThrowDelayHandle, this, &ARPGNPC::NPCThrowDelay, .5f, false);
}

void ARPGNPC::NPCThrowDelay()
{
	switch (NPCThrowable->GetThrowableType())
	{
	case ERPGThrowableType::Explosive: {}
	case ERPGThrowableType::Incendiary: {}
	case ERPGThrowableType::Slow:
		{
			NPCThrowable->ThrowableThrow(NPCTargetEnemy, this);
			break;
		}
	case ERPGThrowableType::Shield:
		{
			Cast<ARPGThrowableShield>(NPCThrowable)->ThrowableThrowShield(GetRootComponent()->GetComponentLocation(), GetRootComponent()->GetForwardVector());
			break;
		}
	}

	if (NPCThrowable->DecreaseRemainingUses()) NPCThrowable = nullptr;
}

void ARPGNPC::NPCStartThrowableSlow(float Duration)
{
	NPCSetMoveSpeedDebuff(true);
	GetWorldTimerManager().ClearTimer(NPCMoveSpeedRemoveDebuffHandle);
	GetWorldTimerManager().SetTimer(NPCMoveSpeedRemoveDebuffHandle, this, &ARPGNPC::NPCStopThrowableSlow, Duration, false);
}

void ARPGNPC::NPCStopThrowableSlow()
{
	if (GetNPCLLegCurrentHealth() == 0.f || GetNPCRLegCurrentHealth() == 0.f) return;
	NPCSetMoveSpeedDebuff(false);
}

bool ARPGNPC::WeaponCheckPriority(ARPGWeapon* Weapon)
{
	return false;
}

bool ARPGNPC::ThrowableCheckPriority(ARPGThrowable* Throwable)
{
	return false;
}

bool ARPGNPC::HeadArmourCheckPriority(ARPGArmour* HArmour)
{
	return false;
}

bool ARPGNPC::ChestArmourCheckPriority(ARPGArmour* CArmour)
{
	return false;
}

bool ARPGNPC::LegsArmourCheckPriority(ARPGArmour* LArmour)
{
	return false;
}

bool ARPGNPC::ItemsCheckPriority(ARPGItem* Item)
{
	if (GetNPCInventory()->GetInventoryItemNumber() == GetNPCInventory()->GetInventorySize()) return false;

	switch (Item->GetItemType())
	{
	case ERPGItemType::Heal:
		{
			if (NPCCountInventory("Heal") >= 2) return false;
			// pick up
			NPCItemPickUp(Item);
			return true;
		}
	case ERPGItemType::Repair:
		{
			if (NPCCountInventory("Repair") >= 1) return false;
			// pick up
			NPCItemPickUp(Item);
			return true;
		}
	case ERPGItemType::Bullets: { break; }
	case ERPGItemType::Throwable: { break; }
	}
	
	// if (Item->ActorHasTag("Heal"))
	// {
	// 	if (NPCCountInventory("Heal") >= 2) return false;
	// 	// pick up
	// 	return true;
	// }
	// if (Item->ActorHasTag("Repair"))
	// {
	// 	if (NPCCountInventory("Repair") >= 2) return false;
	// 	// pick up
	// 	return true;
	// }
	
	return false;
}

// bool ARPGNPC::HealsCheckPriority()
// {
// 	return false;
// }
//
// bool ARPGNPC::RepairsCheckPriority()
// {
// 	return false;
// }

void ARPGNPC::NPCCheckDeathBag(ARPGBag* Bag)
{
	if (Bag->GetBagWeapon()) if (WeaponCheckPriority(Bag->GetBagWeapon())) Bag->SetBagWeapon(nullptr);
	if (Bag->GetBagThrowable()) if (ThrowableCheckPriority(Bag->GetBagThrowable())) Bag->SetBagThrowable(nullptr);
	if (Bag->GetBagHeadArmour()) if (HeadArmourCheckPriority(Bag->GetBagHeadArmour())) Bag->SetBagHeadArmour(nullptr);
	if (Bag->GetBagChestArmour()) if (ChestArmourCheckPriority(Bag->GetBagChestArmour())) Bag->SetBagChestArmour(nullptr);
	if (Bag->GetBagLegsArmour()) if (LegsArmourCheckPriority(Bag->GetBagLegsArmour())) Bag->SetBagLegsArmour(nullptr);
	for (uint8 i = 0; i < Bag->GetBagInventory()->GetInventorySize(); i++)
	{
		if (Bag->GetBagInventory()->GetItemInventory()[i])
			if (ItemsCheckPriority(Bag->GetBagInventory()->GetItemInventory()[i]))
				Bag->GetBagInventory()->SetItemInventory(nullptr, i);
	}

	Bag->BagClosedTimer(true);
}

void ARPGNPC::NPCFlee()
{
	if (GetNPCState() != ERPGNPCState::Fleeing) return;
	// NPCFaceTarget();

	FRotator NPCFleeRot = UKismetMathLibrary::FindLookAtRotation(RootComponent->GetComponentLocation(), NPCTargetEnemy->GetActorLocation());

	FVector NPCFleePos = GetRootComponent()->GetComponentLocation() + NPCFleeRot.Vector() * -2500.f;

	NPCController->MoveToLocation(NPCFleePos, 0.f, true, true, false, false);
	
	if (GetDistanceTo(NPCTargetEnemy) > 2750.f) ChangeNPCStateNormal();
	else
	{
		FTimerHandle NPCFleeHandle;
		GetWorldTimerManager().SetTimer(NPCFleeHandle, this, &ARPGNPC::NPCFlee, 2.f, false);
	}
}

void ARPGNPC::NPCWander()
{
	if (GetNPCState() != ERPGNPCState::Normal) return;

	if (GetNPCClass() == ERPGNPCClass::Trapper) return;

	FRotator WanderRot = FRotator(0.f,FMath::RandRange(GetActorRotation().Yaw + 0.f, GetActorRotation().Yaw + 360.f),0.f);
	
	FVector WanderPos = GetRootComponent()->GetComponentLocation() + WanderRot.Vector() * 1500.f;
	
	// // check if location is within the map bounds
	// FHitResult CSResult;
	// FCollisionQueryParams CSQueryP;
	// CSQueryP.TraceTag = "TraceTag";
	// CSQueryP.AddIgnoredActor(this);
	// FCollisionResponseParams CSResponseP;
	// GetWorld()->LineTraceSingleByChannel(CSResult, GetRootComponent()->GetComponentLocation(),
	// 									WanderPos, ECollisionChannel::ECC_Visibility, CSQueryP, CSResponseP);
	//
	// if (CSResult.Distance == 0.f)
	// {
	// 	NPCController->MoveToLocation(WanderPos, 0.f, true, true, false, false);
	// }

	// UKismetSystemLibrary::DrawDebugSphere(GetWorld(), WanderPos, 25.f, 12, FColor::Green, 10.f);
	
	NPCController->MoveToLocation(WanderPos, 0.f, true, true, false, false);

	// 2.f, 10.f
	FTimerHandle NPCWanderHandle;
	GetWorldTimerManager().SetTimer(NPCWanderHandle, this, &ARPGNPC::NPCWander, FMath::RandRange(2.f, 10.f), false);
}

void ARPGNPC::NPCStartTrade(ACharacter* Character)
{
	if (GetNPCState() != ERPGNPCState::Normal) return;

	NPCTargetEnemy = Character;
	NPCFaceTarget();
	ChangeNPCStateTrade();
}

void ARPGNPC::NPCEndTrade()
{
	if (GetNPCState() != ERPGNPCState::Trade) return;
	
	ChangeNPCStateNormal();
}

void ARPGNPC::SetNPCSelectedItem(ARPGItem* Item)
{
	if (GetNPCSelectedItem()) NPCSelectedItem = nullptr;

	NPCSelectedItem = Item;
}

void ARPGNPC::RemoveNPCSelectedItem()
{
	if (!GetNPCSelectedItem()) return;

	NPCSelectedItem = nullptr;
}

void ARPGNPC::NPCReloadCurrentWeaponTimer()
{
	if (!NPCWeapon) return;
	NPCWeapon->ResetCurrentBullets();
}

void ARPGNPC::NPCSetBound(float Duration)
{
	NPCBound = true;
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	NPCTakeDamage(1, 0.f, nullptr);
	FTimerHandle NPCBoundHandle;
	GetWorldTimerManager().SetTimer(NPCBoundHandle, this, &ARPGNPC::SetNPCBoundFalse, Duration, false);
}

void ARPGNPC::SetNPCBoundFalse()
{
	NPCBound = false;
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void ARPGNPC::DeathAction()
{
	//
}










// Called to bind functionality to input
void ARPGNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
