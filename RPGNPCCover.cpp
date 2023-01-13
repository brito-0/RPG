// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGNPCCover.h"
#include "RPGCharacter.h"
#include "RPGNPCShield.h"
#include "RPGInventory.h"
#include "RPGThrowable.h"
#include "RPGWeapon.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

ARPGNPCCover::ARPGNPCCover()
{
	PrimaryActorTick.bCanEverTick = true;

	SetNPCType(ERPGNPCType::Neutral);
	// SetNPCType(ERPGNPCType::Aggressive);
	SetNPCClass(ERPGNPCClass::Cover);
	SetNPCMaxHealth(100.f);
	SetNPCLArmMaxHealth(50.f);
	SetNPCRArmMaxHealth(50.f);
	SetNPCLLegMaxHealth(50.f);
	SetNPCRLegMaxHealth(50.f);
	SetNPCShotDelay(.7f);
	SetNPCWalkSpeed(360.f);
	SetNPCCrouchSpeed(202.5f);
	SetNPCStrength(.7f);
	SetNPCAccuracy(.75);
	SetNPCHealDuration(5.f);
	SetNPCMinKillExp(35.5);
	SetNPCMaxKillExp(38.5);
	// moderate damage weapon with high accuracy

	SetActorTickInterval(1.f);

	// static ConstructorHelpers::FObjectFinder<UBlueprint>NPCShieldAsset(TEXT(""));
	// if (NPCShieldAsset.Succeeded()) { NPCShieldObject = NPCShieldAsset.Object->GeneratedClass; }
	
	this->Tags.Add("Cover");
}

void ARPGNPCCover::BeginPlay()
{
	Super::BeginPlay();

	NPCController = Cast<AAIController>(GetController());

	if (!NPCShieldObject) UE_LOG(LogTemp, Warning, TEXT("NPC COVER NO SHIELD OBJECT"));

	NPCShields.Init(nullptr, 4);
	
}

void ARPGNPCCover::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

// test this
void ARPGNPCCover::NPCCombatAction()
{
	if (IsRelocating) return;
	if (GetNPCState() != ERPGNPCState::Combat) return;
	if (GetNPCCurrentHealth() < GetNPCMaxHealth() * .3f && !BarricadeUsed && GetDistanceTo(GetNPCTargetEnemy()) > 250.f) NPCShieldBarricade();

	CheckThrownShield();
	
	if (GetWasAttacked() && GetNPCThrowable() && !NPCAlreadyAttacked && GetDistanceTo(GetNPCTargetEnemy()) > 250.f)
	{
		NPCAlreadyAttacked = true;
		// place shield
		if (GetNPCThrowable()->GetThrowableType() == ERPGThrowableType::Shield)
		{
			ShieldPlaced = true;
			NPCThrow();
			ShieldPosition = RootComponent->GetComponentLocation() + (RootComponent->GetForwardVector() * 85);
			FTimerHandle FindShieldDelayHandle;
			GetWorldTimerManager().SetTimer(FindShieldDelayHandle, this, &ARPGNPCCover::FindShield, .6f, false);
		}
		else NPCThrow();
		return;
	}
	// attack and throw if someone gets in the area

	if (!NPCAlreadyAttacked && GetNPCThrowable() && GetDistanceTo(GetNPCTargetEnemy()) > 150.f)
	{
		NPCAlreadyAttacked = true;

		// attack
		NPCAttackArms();
		
		if (GetNPCThrowable()->GetThrowableType() == ERPGThrowableType::Shield)
		{
			// place shield 1.6 delay
			FTimerHandle PlaceShieldDelayHandle;
			GetWorldTimerManager().SetTimer(PlaceShieldDelayHandle, this, &ARPGNPCCover::NPCThrow, 1.6f, false);
		}
		return;
	}

	// move towards player if distance is bigger than 1700 ???
	if (GetDistanceTo(GetNPCTargetEnemy()) > 1350.f && !ThrownShield)
	{
		MoveToEnemy();
		FTimerHandle CombatActionHandle;
		GetWorldTimerManager().SetTimer(CombatActionHandle, this, &ARPGNPCCover::NPCCombatAction, .5f, false);
		return;
	}

	// if thrownshield use a different attack where npc moves to the side of the shild shoots then moves back into cover
	if (ThrownShield)
	{
		NPCCoverAttack();
	}
	else NPCAttackArms();
}

void ARPGNPCCover::NPCShieldBarricade()
{
	BarricadeUsed = true;
	
	const FRotator DropRot = FRotator(0.f, 0.f, 0.f);
	const FVector DropPos0 = GetRootComponent()->GetComponentLocation() + (GetRootComponent()->GetForwardVector() * 85);
	const FVector DropPos1 = GetRootComponent()->GetComponentLocation() + (GetRootComponent()->GetRightVector() * -85);
	const FVector DropPos2 = GetRootComponent()->GetComponentLocation() + (GetRootComponent()->GetForwardVector() * -85);
	const FVector DropPos3 = GetRootComponent()->GetComponentLocation() + (GetRootComponent()->GetRightVector() * 85);

	NPCShields[0] = GetWorld()->SpawnActor<ARPGNPCShield>(NPCShieldObject, DropPos0, DropRot);
	NPCShields[1] = GetWorld()->SpawnActor<ARPGNPCShield>(NPCShieldObject, DropPos1, DropRot);
	NPCShields[2] = GetWorld()->SpawnActor<ARPGNPCShield>(NPCShieldObject, DropPos2, DropRot);
	NPCShields[3] = GetWorld()->SpawnActor<ARPGNPCShield>(NPCShieldObject, DropPos3, DropRot);
	
	if (NPCShields[0]) NPCShields[0]->StartDestroyTimer(GetNPCHealDuration());
	if (NPCShields[1]) NPCShields[1]->StartDestroyTimer(GetNPCHealDuration());
	if (NPCShields[2]) NPCShields[2]->StartDestroyTimer(GetNPCHealDuration());
	if (NPCShields[3]) NPCShields[3]->StartDestroyTimer(GetNPCHealDuration());
	
	NPCRepairArmour();
	NPCHeal(0);

	// FTimerHandle NPCBarricadeHandle;
	// GetWorldTimerManager().SetTimer(NPCBarricadeHandle, this, &ARPGNPCCover::NPCShieldBarricadeDestroy, GetNPCHealDuration(), false);
}

// void ARPGNPCCover::NPCShieldBarricadeDestroy()
// {
// 	// for (ARPGNPCShield* Shield : NPCShields)
// 	// {
// 	// 	
// 	// }
// }

bool ARPGNPCCover::WeaponCheckPriority(ARPGWeapon* Weapon)
{
	if (Weapon->GetWeaponType() == ERPGWeaponType::Melee) return false;
	
	if (!GetNPCWeapon())
	{
		// pick up
		NPCWeaponPickUp(Weapon);
		return true;
	}
	if (GetNPCWeapon()->GetWeaponAccuracy() > Weapon->GetWeaponAccuracy() || Weapon->GetWeaponDamage() < (GetNPCWeapon()->GetWeaponDamage() - 5.f)) return false;
	// pick up
	NPCWeaponPickUp(Weapon);
	return true;
}

bool ARPGNPCCover::ThrowableCheckPriority(ARPGThrowable* Throwable)
{
	if (!GetNPCThrowable())
	{
		// pick up
		NPCThrowablePickUp(Throwable);
		return true;
	}
	if (NPCCountInventory("Throwable") >= 3)
	{
		if (NPCCheckInventory("Shield") != 99) return false;
		NPCItemDrop(NPCCheckInventory("Throwable"));
		// pick up
		// and equip
		NPCThrowablePickUp(Throwable);
		return true;
	}
	// if (NPCCountInventory("Throwable") < 3)
	// {
	// 	NPCThrowablePickUp(Throwable);
	// 	return true;
	// }

	// pick up
	// check if there is a shield in inventory 

	NPCThrowablePickUp(Throwable);
	
	return true;
}

bool ARPGNPCCover::HeadArmourCheckPriority(ARPGArmour* HArmour)
{
	if (!GetNPCHeadArmour())
	{
		// pick up
		NPCArmourPickUp(HArmour);
		return true;
	}
	if (GetNPCHeadArmour()->GetArmourDefense() >= HArmour->GetArmourDefense()) return false;
	// pick up
	NPCArmourPickUp(HArmour);
	return true;
}

bool ARPGNPCCover::ChestArmourCheckPriority(ARPGArmour* CArmour)
{
	if (!GetNPCChestArmour())
	{
		// pick up
		NPCArmourPickUp(CArmour);
		return true;
	}
	if (GetNPCChestArmour()->GetArmourDefense() >= CArmour->GetArmourDefense()) return false;
	// pick up
	NPCArmourPickUp(CArmour);
	return true;
}

bool ARPGNPCCover::LegsArmourCheckPriority(ARPGArmour* LArmour)
{
	if (!GetNPCLegsArmour())
	{
		// pick up
		NPCArmourPickUp(LArmour);
		return true;
	}
	if (GetNPCLegsArmour()->GetArmourDefense() >= LArmour->GetArmourDefense()) return false;
	// pick up
	NPCArmourPickUp(LArmour);
	return true;
}

void ARPGNPCCover::FindShield()
{
	FHitResult Out;
	FCollisionQueryParams CSQueryP;
	CSQueryP.TraceTag = "TraceTag";
	CSQueryP.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(Out, RootComponent->GetComponentLocation(), ShieldPosition, ECollisionChannel::ECC_PhysicsBody, CSQueryP))
	{
		if (Out.GetActor()->ActorHasTag("NPC") && Out.GetActor()->ActorHasTag("Shield"))
		{
			ThrownShield = Cast<ARPGNPCShield>(Out.GetActor());
		}
	}
}

void ARPGNPCCover::CheckThrownShield()
{
	if (ThrownShield) return;

	ThrownShield = nullptr;
	ShieldPlaced = false;
}

void ARPGNPCCover::NPCAttackArms()
{
	if (GetNPCTargetEnemy()->ActorHasTag("Player"))
	{
		const ARPGCharacter* NPCTarget = Cast<ARPGCharacter>(GetNPCTargetEnemy());

		if (NPCTarget->GetCharacterRArmCurrentHealth() > 0)
		{
			NPCAttack(3);
			return;
		}
		NPCAttack(1);
		return;
	}

	const ARPGNPC* NPCTarget = Cast<ARPGNPC>(GetNPCTargetEnemy());

	if (NPCTarget->GetNPCRArmCurrentHealth() > 0)
	{
		NPCAttack(3);
		return;
	}
	NPCAttack(1);
	// return;
}

void ARPGNPCCover::NPCCoverAttack()
{
	// move to one of the sides of the shield then attack
	// use get negative get direction to target to find a location to move to behind the shield after attacking (from shield to player ???)

	NPCFaceTarget();
	FVector MovePos;
	if (FMath::RandBool()) MovePos = ThrownShield->GetActorLocation() + RootComponent->GetRightVector() * -85.f;
	else MovePos = ThrownShield->GetActorLocation() + RootComponent->GetRightVector() * 85.f;

	UKismetSystemLibrary::DrawDebugSphere(GetWorld(), MovePos, 15.f, 12, FColor::Yellow, 3.f);

	NPCController->MoveToLocation(MovePos, -1, false, true, false, true);

	IsRelocating = true;
	
	if (!GetMoveSpeedDebuff())
	{
		FTimerHandle NPCAttackHandle;
		GetWorldTimerManager().SetTimer(NPCAttackHandle, this, &ARPGNPCCover::NPCAttackArms, .7f, false);
	
		FTimerHandle NPCRelocateHandle;
		GetWorldTimerManager().SetTimer(NPCRelocateHandle, this, &ARPGNPCCover::NPCMoveBehindShield, 2.3f, false);
	}
	else
	{
		FTimerHandle NPCAttackHandle;
		GetWorldTimerManager().SetTimer(NPCAttackHandle, this, &ARPGNPCCover::NPCAttackArms, .8f, false);
	
		FTimerHandle NPCRelocateHandle;
		GetWorldTimerManager().SetTimer(NPCRelocateHandle, this, &ARPGNPCCover::NPCMoveBehindShield, 2.4f, false);
	}
	
}

void ARPGNPCCover::NPCMoveBehindShield()
{
	if (!ThrownShield) return;
	
	FRotator MoveDirection = UKismetMathLibrary::FindLookAtRotation(ThrownShield->GetActorLocation(), GetNPCTargetEnemy()->GetActorLocation());

	FVector MovePos = ThrownShield->GetActorLocation() + MoveDirection.Vector() * -85.f;

	UKismetSystemLibrary::DrawDebugSphere(GetWorld(), MovePos, 15.f, 12, FColor::Green, 3.f);
	
	NPCController->MoveToLocation(MovePos, -1, false, true, false, true);

	if (!GetMoveSpeedDebuff())
	{
		FTimerHandle NPCFaceTargetHandle;
		GetWorldTimerManager().SetTimer(NPCFaceTargetHandle, this, &ARPGNPCCover::NPCFaceTarget, .7f, false);
	}
	else
	{
		FTimerHandle NPCFaceTargetHandle;
		GetWorldTimerManager().SetTimer(NPCFaceTargetHandle, this, &ARPGNPCCover::NPCFaceTarget, .8f, false);
	}
	FTimerHandle NPCStopRelocatingHandle;
	GetWorldTimerManager().SetTimer(NPCStopRelocatingHandle, this, &ARPGNPCCover::NPCStopRelocating, FMath::RandRange(2.f, 4.f), false);
	
}

void ARPGNPCCover::NPCStopRelocating()
{
	IsRelocating = false;
	// NPCFaceTarget();
	// FTimerHandle NPCCombatHandle;
	// GetWorldTimerManager().SetTimer(NPCCombatHandle, this, &ARPGNPCCover::NPCCombatAction, FMath::RandRange(5.f, 8.f), false);
	NPCCombatAction();
}
