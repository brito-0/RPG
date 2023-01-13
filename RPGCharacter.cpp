// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGCharacter.h"

// #include "InputState.h"
#include "RPGArmour.h"
#include "RPGNPC.h"
#include "RPGWeapon.h"
#include "RPGInventory.h"
#include "RPGItem.h"
#include "RPGThrowable.h"
#include "RPGBag.h"
#include "RPGThrowableShield.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ARPGCharacter::ARPGCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(CapsuleRadius, CapsuleHeight);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 0.f;
	GetCharacterMovement()->AirControl = 0.f;

	// configure camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 1250.f;
	CameraBoom->bInheritPitch = false;
	CameraBoom->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	CameraBoom->bUsePawnControlRotation = true;
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, .0f));
	// static ConstructorHelpers::FObjectFinder<USkeletalMesh>MeshAsset(TEXT("SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));
	// if (MeshAsset.Succeeded()) GetMesh()->SetSkeletalMesh(MeshAsset.Object);

	// get this to work (mesh)

	SetActorTickInterval(0.5f);
	
	this->Tags.Add("Player");
}

// Called when the game starts or when spawned
void ARPGCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->DebugDrawTraceTag = "TraceTag";

	CurrentCharacterState = ERPGCharacterState::Normal;
	PreviousCharacterState = ERPGCharacterState::Normal;

	CurrentHealth = GetCharacterMaxHealth();
	// HeadCurrentHealth = HeadMaxHealth;
	// ChestCurrentHealth = ChestMaxHealth;
	LArmCurrentHealth = GetCharacterLArmMaxHealth();
	RArmCurrentHealth = GetCharacterRArmMaxHealth();
	LLegCurrentHealth = GetCharacterLLegMaxHealth();
	RLegCurrentHealth = GetCharacterRLegMaxHealth();

	// InventoryMaxCarryWeight = InventoryInitialCarryWeight;

	CharacterInventory = NewObject<ARPGInventory>();
	CharacterInventory->SetInventorySize(6);

	if (AggressiveMenuWidget) AggressiveMenu = CreateWidget<UUserWidget>(GetWorld(),  AggressiveMenuWidget);
	if (InteractiveMenuWidget) InteractiveMenu = CreateWidget<UUserWidget>(GetWorld(),  InteractiveMenuWidget);
	if (HealMenuWidget) HealMenu = CreateWidget<UUserWidget>(GetWorld(),  HealMenuWidget);
	if (LevelUpMenuWidget) LevelUpMenu = CreateWidget<UUserWidget>(GetWorld(),  LevelUpMenuWidget);
	if (LevelUpInfoMenuWidget) LevelUpInfoMenu = CreateWidget<UUserWidget>(GetWorld(),  LevelUpInfoMenuWidget);
	if (InventoryMenuWidget) InventoryMenu = CreateWidget<UUserWidget>(GetWorld(),  InventoryMenuWidget);
	if (BagMenuWidget) BagMenu = CreateWidget<UUserWidget>(GetWorld(),  BagMenuWidget);
	if (TradeMenuWidget) TradeMenu = CreateWidget<UUserWidget>(GetWorld(),  TradeMenuWidget);
	if (InventoryTradeMenuWidget) InventoryTradeMenu = CreateWidget<UUserWidget>(GetWorld(),  InventoryTradeMenuWidget);
	if (PauseMenuWidget) PauseMenu = CreateWidget<UUserWidget>(GetWorld(),  PauseMenuWidget);

	// move this into player state																																		asdaasdasfasdfsdfsdfasfdfs
	GetCharacterMovement()->SetJumpAllowed(false);
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed;
}

// Called every frame
void ARPGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TargetNPC)
	{
		switch (TargetType)
		{
		default: {}
		case 0:
			{
				if (GetDistanceTo(TargetNPC) > MaxInteractDistance)
				{
					RemoveTargetNPC();
				}
				break;
			}
		case 1:
			{
				if (GetDistanceTo(TargetNPC) > MaxAggressiveDistance)
				{
					RemoveTargetNPC();
				}
				break;
			}
		}
	}
	
	
}


void ARPGCharacter::SetCharacterState(ERPGCharacterState State)
{
	if ((State == ERPGCharacterState::Combat && GetCharacterState() == ERPGCharacterState::Combat) || (State == ERPGCharacterState::Combat && GetCharacterState() == ERPGCharacterState::Shooting)
		|| StateChangeBlock && State != ERPGCharacterState::Dead)
	{
		GetWorldTimerManager().ClearTimer(CombatToNormalHandle);
		GetWorldTimerManager().SetTimer(CombatToNormalHandle, this, &ARPGCharacter::ChangeCharacterStateNormal, CombatToNormalDuration, false);
		return;
	}
	if (GetCharacterState() == State || GetCharacterState() == ERPGCharacterState::Dead) return;
	if (GetCharacterState() == ERPGCharacterState::Inventory && State == ERPGCharacterState::Heal) return;

	switch (State)
	{
	case ERPGCharacterState::Normal:
		{
			if (UGameplayStatics::IsGamePaused(GetWorld())) UGameplayStatics::SetGamePaused(GetWorld(), false);
			
			if (GetCharacterState() == ERPGCharacterState::Combat) PreviousCharacterState = GetCharacterState();

			CurrentCharacterState = ERPGCharacterState::Normal;

			if (!CharacterBound)
			{
				GetCharacterMovement()->SetMovementMode(MOVE_Walking);
				ChangeCharacterMoveSpeed();
			}
			// if (!MoveSpeedDebuff) GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
			// else GetCharacterMovement()->MaxWalkSpeed = WalkSpeed * DebuffVal;
			
			UE_LOG(LogTemp, Warning, TEXT("Player -> Normal"));
			break;
		}
	case ERPGCharacterState::Crouch:
		{
			if (GetCharacterState() == ERPGCharacterState::Normal || GetCharacterState() == ERPGCharacterState::Combat) PreviousCharacterState = GetCharacterState();

			CurrentCharacterState = ERPGCharacterState::Crouch;

			if (!CharacterBound)
			{
				GetCharacterMovement()->SetMovementMode(MOVE_Walking);
				ChangeCharacterMoveSpeed();
			}
			// if (!MoveSpeedDebuff) GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed;
			// else GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed * DebuffVal;

			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("Crouch Animation"));
			UE_LOG(LogTemp, Warning, TEXT("Player -> Crouch"));
			break;
		}
	case ERPGCharacterState::Inventory:
		{
			// can only change state to inventory when in normal state
			PreviousCharacterState = ERPGCharacterState::Normal;

			CurrentCharacterState = ERPGCharacterState::Inventory;

			UGameplayStatics::SetGamePaused(GetWorld(), true);

			GetCharacterMovement()->SetMovementMode(MOVE_None);
			
			UE_LOG(LogTemp, Warning, TEXT("Player -> Inventory"));
			break;
		}
	case ERPGCharacterState::Heal:
		{
			if (GetCharacterState() == ERPGCharacterState::Normal || GetCharacterState() == ERPGCharacterState::Combat) PreviousCharacterState = GetCharacterState();

			CurrentCharacterState = ERPGCharacterState::Heal;

			GetCharacterMovement()->SetMovementMode(MOVE_None);

			// return to previous state after some time
			FTimerHandle StopHealHandle;
			GetWorldTimerManager().SetTimer(StopHealHandle, this, &ARPGCharacter::ChangeCharacterStatePreviousState, 2.f, false);

			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("Heal Animation"));
			UE_LOG(LogTemp, Warning, TEXT("Player -> Heal"));
			break;
		}
	case ERPGCharacterState::Combat:
		{
			if (GetCharacterState() == ERPGCharacterState::Normal) PreviousCharacterState = GetCharacterState();
			
			CurrentCharacterState = ERPGCharacterState::Combat;

			if (!CharacterBound)
			{
				GetCharacterMovement()->SetMovementMode(MOVE_Walking);
				ChangeCharacterMoveSpeed();
			}
			// if (!MoveSpeedDebuff) GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
			// else GetCharacterMovement()->MaxWalkSpeed = WalkSpeed * DebuffVal;

			GetWorldTimerManager().ClearTimer(CombatToNormalHandle);
			GetWorldTimerManager().SetTimer(CombatToNormalHandle, this, &ARPGCharacter::ChangeCharacterStateNormal, CombatToNormalDuration, false);
			
			UE_LOG(LogTemp, Warning, TEXT("Player -> Combat"));
			break;
		}
	case ERPGCharacterState::Throwing:
		{
			PreviousCharacterState = ERPGCharacterState::Combat;

			CurrentCharacterState = ERPGCharacterState::Throwing;

			GetWorldTimerManager().ClearTimer(CombatToNormalHandle);
			GetWorldTimerManager().SetTimer(CombatToNormalHandle, this, &ARPGCharacter::ChangeCharacterStateNormal, CombatToNormalDuration, false);

			PlayerFaceTargetNPC();
			GetCharacterMovement()->SetMovementMode(MOVE_None);
			
			// change to combat after some time
			FTimerHandle StopThrowingHandle;
			GetWorldTimerManager().SetTimer(StopThrowingHandle, this, &ARPGCharacter::ChangeCharacterStateCombat, .7f, false);

			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("Throwing Animation"));
			UE_LOG(LogTemp, Warning, TEXT("Player -> Throwing"));
			break;
		}
	case ERPGCharacterState::Shooting:
		{
			// if (GetCharacterState() == ERPGCharacterState::Normal || GetCharacterState() == ERPGCharacterState::Combat) PreviousCharacterState = GetCharacterState();
			PreviousCharacterState = ERPGCharacterState::Combat;

			CurrentCharacterState = ERPGCharacterState::Shooting;

			GetWorldTimerManager().ClearTimer(CombatToNormalHandle);
			GetWorldTimerManager().SetTimer(CombatToNormalHandle, this, &ARPGCharacter::ChangeCharacterStateNormal, CombatToNormalDuration, false);

			PlayerFaceTargetNPC();
			GetCharacterMovement()->SetMovementMode(MOVE_None);

			if (CurrentWeapon->GetWeaponType() == ERPGWeaponType::Ranged) CurrentWeapon->ReduceBulletAmount();
			
			if (CurrentWeapon->GetWeaponCurrentBulletAmount() > 0)
			{
				FTimerHandle StopShootingHandle;
				GetWorldTimerManager().SetTimer(StopShootingHandle, this, &ARPGCharacter::ChangeCharacterStateCooldown, 1.f, false);
			}
			else
			{
				FTimerHandle StopShootingHandle;
				GetWorldTimerManager().SetTimer(StopShootingHandle, this, &ARPGCharacter::ChangeCharacterStateReloading, 1.f, false);
			}
			
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("Shooting Animation"));
			UE_LOG(LogTemp, Warning, TEXT("Player -> Shooting"));
			break;
		}
	case ERPGCharacterState::Cooldown:
		{
			CurrentCharacterState = ERPGCharacterState::Cooldown;

			if (!CharacterBound)
			{
				GetCharacterMovement()->SetMovementMode(MOVE_Walking);
				ChangeCharacterMoveSpeed();
			}
			// if (!MoveSpeedDebuff) GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
			// else GetCharacterMovement()->MaxWalkSpeed = WalkSpeed * DebuffVal;

			// prevents unwanted state changes when player takes damage
			SetStateChangeBlockTrue();
			FTimerHandle StopStateChangeBlockHandle;
			GetWorldTimerManager().SetTimer(StopStateChangeBlockHandle, this, &ARPGCharacter::SetStateChangeBlockFalse, CurrentWeapon->GetWeaponCooldown() - .1f, false);
			
			// change to combat after some time
			FTimerHandle StopCooldownHandle;
			GetWorldTimerManager().SetTimer(StopCooldownHandle, this, &ARPGCharacter::ChangeCharacterStateCombat, CurrentWeapon->GetWeaponCooldown(), false);

			UE_LOG(LogTemp, Warning, TEXT("Player -> Cooldown"));
			break;
		}
	case ERPGCharacterState::Reloading:
		{
			if (GetCharacterState() == ERPGCharacterState::Normal || GetCharacterState() == ERPGCharacterState::Combat) PreviousCharacterState = GetCharacterState();

			CurrentCharacterState = ERPGCharacterState::Reloading;

			if (!CharacterBound)
			{
				GetCharacterMovement()->SetMovementMode(MOVE_Walking);
				ChangeCharacterMoveSpeed(.5f);
			}
			// if (!MoveSpeedDebuff) GetCharacterMovement()->MaxWalkSpeed = WalkSpeed * .5f;
			// else GetCharacterMovement()->MaxWalkSpeed = (WalkSpeed * DebuffVal) * .5f;
			
			FTimerHandle ReloadWeaponHandle;
			GetWorldTimerManager().SetTimer(ReloadWeaponHandle, this, &ARPGCharacter::ReloadCurrentWeaponTimer, CurrentWeapon->GetWeaponReloadDuration() - .1f, false);

			// prevents unwanted state changes when player takes damage
			SetStateChangeBlockTrue();
			FTimerHandle StopStateChangeBlockHandle;
			GetWorldTimerManager().SetTimer(StopStateChangeBlockHandle, this, &ARPGCharacter::SetStateChangeBlockFalse, CurrentWeapon->GetWeaponReloadDuration() - .1f, false);
			
			// return to previous state after some time
			FTimerHandle StopReloadHandle;
			GetWorldTimerManager().SetTimer(StopReloadHandle, this, &ARPGCharacter::ChangeCharacterStatePreviousState, CurrentWeapon->GetWeaponReloadDuration(), false);

			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("Reload Animation"));
			UE_LOG(LogTemp, Warning, TEXT("Player -> Reloading"));
			break;
		}
	case ERPGCharacterState::Rest:
		{
			// can only change state to rest when in normal state
			PreviousCharacterState = ERPGCharacterState::Normal;

			CurrentCharacterState = ERPGCharacterState::Rest;
			
			UGameplayStatics::SetGamePaused(GetWorld(), true);

			GetCharacterMovement()->SetMovementMode(MOVE_None);

			UE_LOG(LogTemp, Warning, TEXT("Player -> Rest"));
			break;
		}
	case ERPGCharacterState::Dead:
		{
			CurrentCharacterState = ERPGCharacterState::Dead;

			GetCharacterMovement()->SetMovementMode(MOVE_None);

			CurrentHealth = 0.f;
			LArmCurrentHealth = 0.f;
			RArmCurrentHealth = 0.f;
			LLegCurrentHealth = 0.f;
			RLegCurrentHealth = 0.f;

			UGameplayStatics::SetGamePaused(GetWorld(), true);

			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("Death Animation"));
			UE_LOG(LogTemp, Warning, TEXT("Player -> Dead"));
			break;
		}
	}
}


void ARPGCharacter::CharacterTakeDamage(uint8 BodyPart, float Damage)
{
	float FinalDamage = 0;
	switch (BodyPart)
	{
	case 0: // head
		{
			Damage *= 1.3f;
			if (HeadArmour) FinalDamage = HeadArmour->CalcDamage(Damage);
			else FinalDamage = Damage;
			break;
		}
	case 1: // chest
		{
			if (ChestArmour) FinalDamage = ChestArmour->CalcDamage(Damage);
			else FinalDamage = Damage;
			break;
		}
	case 2: // larm
		{
			if (ChestArmour) FinalDamage = ChestArmour->CalcDamage(Damage);
			else FinalDamage = Damage;
			if (LArmCurrentHealth - FinalDamage > 0.f) LArmCurrentHealth -= FinalDamage;
			else 
			{
				LArmCurrentHealth = 0.f;
				if (!AccuracyDebuff) AccuracyDebuff = true;
			}
			break;
		}
	case 3: // rarm
		{
			if (ChestArmour) FinalDamage = ChestArmour->CalcDamage(Damage);
			else FinalDamage = Damage;
			if (RArmCurrentHealth - FinalDamage > 0.f) RArmCurrentHealth -= FinalDamage;
			else 
			{
				RArmCurrentHealth = 0.f;
				if (!AccuracyDebuff) AccuracyDebuff = true;
			}
			break;
		}
	case 4: // lleg
		{
			if (LegsArmour) FinalDamage = LegsArmour->CalcDamage(Damage);
			else FinalDamage = Damage;
			if (LLegCurrentHealth - FinalDamage > 0.f) LLegCurrentHealth -= FinalDamage;
			else
			{
				LLegCurrentHealth = 0.f;
				if (!MoveSpeedDebuff) SetMoveSpeedDebuff(true);
			}
			break;
		}
	case 5: // rleg
		{
			if (LegsArmour) FinalDamage = LegsArmour->CalcDamage(Damage);
			else FinalDamage = Damage;
			if (RLegCurrentHealth - FinalDamage > 0.f) RLegCurrentHealth -= FinalDamage;
			else
			{
				RLegCurrentHealth = 0.f;
				if (!MoveSpeedDebuff) SetMoveSpeedDebuff(true);
			}
			break;
		}
	default: { break; }		// miss
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Player -> Damage Taken: %f"), FinalDamage);
	
	if (CurrentHealth - FinalDamage <= 0.f)
	{
		CurrentHealth = 0.f;
		ChangeCharacterStateDead();
		return;
	}

	CurrentHealth -= FinalDamage;
	ChangeCharacterStateCombat();
}

void ARPGCharacter::CharacterAttack(uint8 BodyPart)
{
	if (!CurrentWeapon || !TargetNPC) return;
	if (GetCharacterState() != ERPGCharacterState::Normal && GetCharacterState() != ERPGCharacterState::Combat) return;
	if (CurrentWeapon->GetWeaponType() == ERPGWeaponType::Ranged && CurrentWeapon->GetWeaponCurrentBulletAmount() == 0) return;
	if (CurrentWeapon->GetWeaponType() == ERPGWeaponType::Melee && CurrentWeapon->GetWeaponRange() < GetDistanceTo(TargetNPC))
	{
		UE_LOG(LogTemp, Warning, TEXT("enemy outside of range"));
		return;
	}

	BodyPartToShoot = BodyPart;

	ChangeCharacterStateShooting();

	if (CurrentWeapon->GetWeaponType() == ERPGWeaponType::Ranged)
	{
		FTimerHandle CharacterAttackDelayHandle;
		GetWorldTimerManager().SetTimer(CharacterAttackDelayHandle, this, &ARPGCharacter::CharacterAttackDelay, .7f, false);
		return;
	}
	
	FTimerHandle CharacterMeleeAttackDelayHandle;
	GetWorldTimerManager().SetTimer(CharacterMeleeAttackDelayHandle, this, &ARPGCharacter::CharacterMeleeAttackDelay, .7f, false);
	
}

void ARPGCharacter::CharacterAttackDelay()
{
	if (GetDistanceTo(TargetNPC) <= 250)
	{
		if (TargetLineOfSight(BodyPartToShoot))
		{
			CurrentWeapon->WeaponShoot(TargetNPC, BodyPartToShoot, true, this);
			UE_LOG(LogTemp, Warning, TEXT("%d"), BodyPartToShoot);

			CheckTargetDead(TargetNPC);
		}
		return;
	}
	
	double ShotAccuracy;
	if (!AccuracyDebuff) ShotAccuracy = CurrentWeapon->GetWeaponAccuracy() * CharacterAccuracy;
	else ShotAccuracy = CurrentWeapon->GetWeaponAccuracy() * (CharacterAccuracy * DebuffVal);
	
	
	if (FMath::RandRange(0.00, 1.00) <= ShotAccuracy)
	{
		// TargetNPC->NPCTakeDamage(BodyPart, CurrentWeapon->GetWeaponDamage());
		
		if (TargetLineOfSight(BodyPartToShoot))
		{
			CurrentWeapon->WeaponShoot(TargetNPC, BodyPartToShoot, false, this);
			UE_LOG(LogTemp, Warning, TEXT("%d"), BodyPartToShoot);

			CheckTargetDead(TargetNPC);
		}
		
		return;
	}

	TArray ShotLocations = {0, 1, 2, 3, 4, 5};
	ShotLocations[BodyPartToShoot] = 6;
	
	// TargetNPC->NPCTakeDamage(FMath::RandRange(0, 6), CurrentWeapon->GetWeaponDamage());
	BodyPartToShoot = FMath::RandRange(0, 5);

	if (TargetLineOfSight(ShotLocations[BodyPartToShoot]))
	{
		CurrentWeapon->WeaponShoot(TargetNPC, ShotLocations[BodyPartToShoot], false, this);
		UE_LOG(LogTemp, Warning, TEXT("%d -> %d"), BodyPartToShoot, ShotLocations[BodyPartToShoot]);

		CheckTargetDead(TargetNPC);
	}
}

void ARPGCharacter::CharacterMeleeAttackDelay()
{
	FHitResult Out;
	FCollisionQueryParams CSQueryP;
	CSQueryP.TraceTag = "TraceTag";
	CSQueryP.AddIgnoredActor(TargetNPC);

	if (GetWorld()->LineTraceSingleByChannel(Out, TargetNPC->GetActorLocation(),
		TargetNPC->GetActorLocation() + TargetNPC->GetActorForwardVector() * -150, ECollisionChannel::ECC_PhysicsBody, CSQueryP))
	{
		if (Out.GetActor() == this)
		{
			if (TargetLineOfSight(BodyPartToShoot))
			{
				CurrentWeapon->WeaponShoot(TargetNPC, BodyPartToShoot, true, this);
				UE_LOG(LogTemp, Warning, TEXT("%d"), BodyPartToShoot);

				CheckTargetDead(TargetNPC);
			}
		}
		return;
	}

	if (TargetLineOfSight(BodyPartToShoot))
	{
		CurrentWeapon->WeaponShoot(TargetNPC, BodyPartToShoot, false, this);
		UE_LOG(LogTemp, Warning, TEXT("%d"), BodyPartToShoot);

		CheckTargetDead(TargetNPC);
	}
	
}

bool ARPGCharacter::TargetLineOfSight(uint8 ShotLocation)
{
	// switch
	// 0 - CheckHead
	// 1 - CheckChestL and CheckChestR
	// 2 - CheckLArm
	// 3 - CheckRArm
	// 4 - CheckLLeg
	// 5 - CheckRLeg
	// 6 - Miss

	FHitResult out;
	FCollisionQueryParams CSQueryP;
	CSQueryP.TraceTag = "TraceTag";
	CSQueryP.AddIgnoredActor(this);
	
	switch (ShotLocation)
	{
	case 0:
		{
			FVector PlayerLoc = RootComponent->GetComponentLocation();
			FVector SocketLoc = TargetNPC->GetMesh()->GetSocketLocation("CheckHead");
	
			if (GetWorld()->LineTraceSingleByChannel(out, PlayerLoc, SocketLoc, ECollisionChannel::ECC_PhysicsBody, CSQueryP))
			{
				if (out.GetActor()->ActorHasTag("NPC"))
				{
					if (out.GetActor() == TargetNPC)
					{
						UE_LOG(LogTemp, Warning, TEXT("ENEMY HIT"));
						return true;
					}
					CharacterStrayBulletAttack(Cast<ARPGNPC>(out.GetActor()));
				}
				return false;
			}
			
			return false;
		}
	case 1:
		{
			FVector PlayerLoc = RootComponent->GetComponentLocation();
			FVector SocketLoc1 = TargetNPC->GetMesh()->GetSocketLocation("CheckChestL");
			FVector SocketLoc2 = TargetNPC->GetMesh()->GetSocketLocation("CheckChestR");
			
			if (GetWorld()->LineTraceSingleByChannel(out, PlayerLoc, SocketLoc1, ECollisionChannel::ECC_PhysicsBody, CSQueryP))
			{
				if (out.GetActor()->ActorHasTag("NPC"))
				{
					if (out.GetActor() == TargetNPC)
					{
						UE_LOG(LogTemp, Warning, TEXT("ENEMY HIT"));
						return true;
					}
					CharacterStrayBulletAttack(Cast<ARPGNPC>(out.GetActor()));
					return false;
				}
				if (GetWorld()->LineTraceSingleByChannel(out, PlayerLoc, SocketLoc2, ECollisionChannel::ECC_PhysicsBody, CSQueryP))
				{
					if (out.GetActor()->ActorHasTag("NPC"))
					{
						if (out.GetActor() == TargetNPC)
						{
							UE_LOG(LogTemp, Warning, TEXT("ENEMY HIT"));
							return true;
						}
						CharacterStrayBulletAttack(Cast<ARPGNPC>(out.GetActor()));
					}
				}
				return false;
			}
			
			return false;
		}
	case 2:
		{
			FVector PlayerLoc = RootComponent->GetComponentLocation();
			FVector SocketLoc = TargetNPC->GetMesh()->GetSocketLocation("CheckLArm");
	
			if (GetWorld()->LineTraceSingleByChannel(out, PlayerLoc, SocketLoc, ECollisionChannel::ECC_PhysicsBody, CSQueryP))
			{
				if (out.GetActor()->ActorHasTag("NPC"))
				{
					if (out.GetActor() == TargetNPC)
					{
						UE_LOG(LogTemp, Warning, TEXT("ENEMY HIT"));
						return true;
					}
					CharacterStrayBulletAttack(Cast<ARPGNPC>(out.GetActor()));
				}
				return false;
			}
			
			return false;
		}
	case 3:
		{
			FVector PlayerLoc = RootComponent->GetComponentLocation();
			FVector SocketLoc = TargetNPC->GetMesh()->GetSocketLocation("CheckRArm");
	
			if (GetWorld()->LineTraceSingleByChannel(out, PlayerLoc, SocketLoc, ECollisionChannel::ECC_PhysicsBody, CSQueryP))
			{
				if (out.GetActor()->ActorHasTag("NPC"))
				{
					if (out.GetActor() == TargetNPC)
					{
						UE_LOG(LogTemp, Warning, TEXT("ENEMY HIT"));
						return true;
					}
					CharacterStrayBulletAttack(Cast<ARPGNPC>(out.GetActor()));
				}
				return false;
			}
			
			return false;
		}
	case 4:
		{
			FVector PlayerLoc = RootComponent->GetComponentLocation();
			FVector SocketLoc = TargetNPC->GetMesh()->GetSocketLocation("CheckLLeg");
	
			if (GetWorld()->LineTraceSingleByChannel(out, PlayerLoc, SocketLoc, ECollisionChannel::ECC_PhysicsBody, CSQueryP))
			{
				if (out.GetActor()->ActorHasTag("NPC"))
				{
					if (out.GetActor() == TargetNPC)
					{
						UE_LOG(LogTemp, Warning, TEXT("ENEMY HIT"));
						return true;
					}
					CharacterStrayBulletAttack(Cast<ARPGNPC>(out.GetActor()));
				}
				return false;
			}
			
			return false;
		}
	case 5:
		{
			FVector PlayerLoc = RootComponent->GetComponentLocation();
			FVector SocketLoc = TargetNPC->GetMesh()->GetSocketLocation("CheckRLeg");
	
			if (GetWorld()->LineTraceSingleByChannel(out, PlayerLoc, SocketLoc, ECollisionChannel::ECC_PhysicsBody, CSQueryP))
			{
				if (out.GetActor()->ActorHasTag("NPC"))
				{
					if (out.GetActor() == TargetNPC)
					{
						UE_LOG(LogTemp, Warning, TEXT("ENEMY HIT"));
						return true;
					}
					CharacterStrayBulletAttack(Cast<ARPGNPC>(out.GetActor()));
				}
				return false;
			}
			
			return false;
		}
	case 6:
		{
			FVector PlayerLoc = RootComponent->GetComponentLocation();
			FVector MissLoc;
			float DistanceToTarget = GetDistanceTo(TargetNPC);
			
			UE_LOG(LogTemp, Warning, TEXT("distance: %f"), DistanceToTarget);

			FString MissString = "CheckMiss";
			FString MissStringTemp = MissString;
			uint8 TargetMissSocketNum = TargetNPC->GetMissSocketNumber();
			uint8 MissNum = FMath::RandRange(0, TargetMissSocketNum-1);
			MissStringTemp.AppendInt(MissNum);

			MissLoc = TargetNPC->GetMesh()->GetSocketLocation(FName(*MissStringTemp));
			UE_LOG(LogTemp, Warning, TEXT("MissSocket: %s"), *MissStringTemp);
			
			if (GetWorld()->LineTraceSingleByChannel(out, PlayerLoc, PlayerLoc + UKismetMathLibrary::FindLookAtRotation(PlayerLoc, MissLoc).Vector() * 2050, 
				ECollisionChannel::ECC_PhysicsBody, CSQueryP))
			{
				if (out.GetActor()->ActorHasTag("NPC"))
				{
					ARPGNPC* CrossFireTarget = nullptr;
					if (Cast<ARPGNPC>(out.GetActor()) == TargetNPC)
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

							MissLoc = TargetNPC->GetMesh()->GetSocketLocation(FName(*MissStringTemp));
							UE_LOG(LogTemp, Warning, TEXT("MissSocket: %s"), *MissStringTemp);
							
							if (GetWorld()->LineTraceSingleByChannel(out, PlayerLoc, PlayerLoc + UKismetMathLibrary::FindLookAtRotation(PlayerLoc, MissLoc).Vector() * 2050, 
								ECollisionChannel::ECC_PhysicsBody, CSQueryP))
							{
								if (out.GetActor()->ActorHasTag("NPC"))
								{
									if (Cast<ARPGNPC>(out.GetActor()) != TargetNPC)
									{
										CrossFireTarget = Cast<ARPGNPC>(out.GetActor());
										break;
									}
								} else return true;
							} else return true;
						}
					} else CrossFireTarget = Cast<ARPGNPC>(out.GetActor());
					
					// deal damage to npc
					if (CrossFireTarget) CharacterStrayBulletAttack(CrossFireTarget);
				}
			}
			
			// check if it hits a different enemy
			return true;
		}
	default: { return false; }
	}
	
	// TArray<FName> SocketNames = {"CheckHead", "CheckChestL", "CheckLArm", "CheckRArm", "CheckLLeg", "CheckRLeg", "CheckChestR"};
	//
	// if (ShotLocation == 1)
	// {
	// 	FVector PlayerLoc = RootComponent->GetComponentLocation();
	// 	FVector SocketLoc1 = TargetNPC->GetMesh()->GetSocketLocation(SocketNames[ShotLocation]);
	// 	FVector SocketLoc2 = TargetNPC->GetMesh()->GetSocketLocation(SocketNames[6]);
	// 	
	// 	if (GetWorld()->LineTraceSingleByChannel(out, PlayerLoc, SocketLoc1, ECollisionChannel::ECC_PhysicsBody, CSQueryP)
	// 			|| GetWorld()->LineTraceSingleByChannel(out, PlayerLoc, SocketLoc2, ECollisionChannel::ECC_PhysicsBody, CSQueryP))
	// 	{
	// 		if (out.GetActor()->ActorHasTag("NPC"))
	// 		{
	// 			UE_LOG(LogTemp, Warning, TEXT("ENEMY HIT"));
	// 			return true;
	// 		}
	// 	}
	// }
	// else
	// {
	// 	FVector PlayerLoc = RootComponent->GetComponentLocation();
	// 	FVector SocketLoc = TargetNPC->GetMesh()->GetSocketLocation(SocketNames[ShotLocation]);
	// 	
	// 	if (GetWorld()->LineTraceSingleByChannel(out, PlayerLoc, SocketLoc, ECollisionChannel::ECC_PhysicsBody, CSQueryP))
	// 	{
	// 		if (out.GetActor()->ActorHasTag("NPC"))
	// 		{
	// 			UE_LOG(LogTemp, Warning, TEXT("ENEMY HIT"));
	// 			return true;
	// 		}
	// 	}
	// }
	//
	// return false;
}

void ARPGCharacter::CharacterStrayBulletAttack(ARPGNPC* Target)
{
	CurrentWeapon->WeaponShoot(Target, 1, false, this);
	UE_LOG(LogTemp, Warning, TEXT("NPC HIT BY STRAY BULLET"));

	CheckTargetDead(Target);
}

bool ARPGCharacter::CheckTargetDead(ARPGNPC* Target)
{
	if (Target->GetNPCState() != ERPGNPCState::Dead) return false;

	CharacterGetExp(Target->GetNPCMinKillExp(), Target->GetNPCMaxKillExp());
	if (Target == TargetNPC) RemoveTargetNPC();

	UE_LOG(LogTemp, Warning, TEXT("NPC KILLED"));
	
	return true;
}


// Called to bind functionality to input
void ARPGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ARPGCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARPGCharacter::MoveRight);
	
	PlayerInputComponent->BindAction("SelectInteract", IE_Pressed, this, &ARPGCharacter::SelectNPC);
	PlayerInputComponent->BindAction("SelectOffensive", IE_Pressed, this, &ARPGCharacter::SelectNPCOffensive);

	// PlayerInputComponent->BindAction("SwitchWeapon", IE_Pressed, this, &ARPGCharacter::SwitchWeapon);
	
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ARPGCharacter::Interact);

	PlayerInputComponent->BindAction("Throw", IE_Pressed, this, &ARPGCharacter::CharacterThrow);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ARPGCharacter::CharacterReload);
	
	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &ARPGCharacter::PauseGame).bExecuteWhenPaused = true;



	PlayerInputComponent->BindAction("Test", IE_Pressed, this, &ARPGCharacter::TestFunction);
}

void ARPGCharacter::MoveForward(float Value)
{
	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ARPGCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
    	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

// void ARPGCharacter::SwitchWeapon()
// {
// 	if (CurrentWeapon && OtherWeapon)
// 	{
// 		// CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
// 		// OtherWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
// 		
// 		ARPGWeapon* temp = CurrentWeapon;
// 		temp->SetActorHiddenInGame(true);
// 		const FAttachmentTransformRules CurrentWeaponRules(EAttachmentRule::KeepRelative, true);
// 		temp->AttachToComponent(GetMesh(), CurrentWeaponRules, "InventorySocket");
// 		
// 		// CurrentWeapon = OtherWeapon;
// 		// OtherWeapon = temp;
// 		// temp->Destroy();
//
// 		// // CurrentWeapon->SetActorHiddenInGame(true);
// 		// const FAttachmentTransformRules CurrentWeaponRules(EAttachmentRule::KeepRelative, true);
// 		// CurrentWeapon->AttachToComponent(GetMesh(), CurrentWeaponRules, "InventorySocket");
// 		//
// 		// // OtherWeapon->SetActorHiddenInGame(true);
// 		// const FAttachmentTransformRules OtherWeaponRules(EAttachmentRule::KeepRelative, true);
// 		// OtherWeapon->AttachToComponent(GetMesh(), OtherWeaponRules, "InventorySocket");
// 	}
// 	
// 	// switch gun model
//
//
// 	// does not work properly
// 	// think of another way																																								agsdgwsdgasdgsdagsdg
// }

FVector ARPGCharacter::GetMouseLocation()
{
	FVector MouseLoc, MouseDir;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->DeprojectMousePositionToWorld(MouseLoc, MouseDir);
	return MouseLoc + MouseDir * (CameraBoom->TargetArmLength*2);
}

void ARPGCharacter::SelectNPC()
{
	if (GetCharacterState() != ERPGCharacterState::Normal) return;
	// select npc
	// bring up the interaction menu
	
	
	FHitResult out;
	FVector Start = GetFollowCamera()->GetComponentLocation();
	FVector End = GetMouseLocation();
	FCollisionQueryParams CSQueryP;
	CSQueryP.TraceTag = "TraceTag";
	CSQueryP.AddIgnoredActor(this);
	// UKismetSystemLibrary::DrawDebugSphere(GetWorld(), End, 25.f, 12, FColor::Blue, 2.f, 1);
	
	if (GetWorld()->LineTraceSingleByChannel(out, Start, End, ECollisionChannel::ECC_PhysicsBody, CSQueryP))
	{
		// check if it hits an npc and check the
		// check if the character is close to the npc
		
		if (out.GetActor()->ActorHasTag("NPC"))
		{
			if (GetDistanceTo(out.GetActor()) <= MaxInteractDistance)
			{
				if (Cast<ARPGNPC>(out.GetActor())->GetNPCState() == ERPGNPCState::Dead) return;
				if (Cast<ARPGNPC>(out.GetActor())->GetNPCType() == ERPGNPCType::Aggressive) return;
				
				TargetNPC = Cast<ARPGNPC>(out.GetActor());
				TargetType = 0;
				UE_LOG(LogTemp, Warning, TEXT("INTERACTIVE MENU"));

				// bring up interactive menu																																				asfasfasfasfasfafafa
				if (AggressiveMenu->IsInViewport()) RemoveAggressiveMenu();
				if (HealMenu->IsInViewport()) HealMenu->RemoveFromViewport();
				if (!InteractiveMenu->IsInViewport()) InteractiveMenu->AddToViewport();

				// stop movement ?????
				TargetNPC->NPCStartTrade(this);
			}
		}
	}
}

void ARPGCharacter::SelectNPCOffensive()
{
	if (GetCharacterState() != ERPGCharacterState::Normal && GetCharacterState() != ERPGCharacterState::Combat) return;
	// select npc
	// slow down game
	// bring up the combat menu
	// add a cooldown ???
	

	if (!CurrentWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("NO WEAPON"));
		return;
	}
	
	FHitResult out;
	FVector Start = GetFollowCamera()->GetComponentLocation();
	FVector End = GetMouseLocation();
	FCollisionQueryParams CSQueryP;
	CSQueryP.TraceTag = "TraceTag";
	CSQueryP.AddIgnoredActor(this);
	// UKismetSystemLibrary::DrawDebugSphere(GetWorld(), GetMouseLocation(), 25.f, 12, FColor::Red, 2.f, 1);
	
	if (GetWorld()->LineTraceSingleByChannel(out, Start, End, ECollisionChannel::ECC_PhysicsBody, CSQueryP))
	{
		// check if it hits an npc and check the
		
		if (out.GetActor()->ActorHasTag("NPC"))
		{
			if (GetDistanceTo(out.GetActor()) <= MaxAggressiveDistance)
			{
				if (Cast<ARPGNPC>(out.GetActor())->GetNPCState() == ERPGNPCState::Dead) return;
				
				TargetNPC = Cast<ARPGNPC>(out.GetActor());
				TargetType = 1;
				UE_LOG(LogTemp, Warning, TEXT("COMBAT MENU"));

				// slow the game ???
				// bring up offensive menu (have offensive menu also show the target hp)																									asfasfasfasfasfafafa
				if (InteractiveMenu->IsInViewport()) InteractiveMenu->RemoveFromViewport();
				if (HealMenu->IsInViewport()) HealMenu->RemoveFromViewport();
				if (!AggressiveMenu->IsInViewport()) AggressiveMenu->AddToViewport();

				
				UGameplayStatics::SetGlobalTimeDilation(GetWorld(), .3f);
				
				GetWorldTimerManager().SetTimer(TimeDilationHandle, this, &ARPGCharacter::SetRegularTimeDilation, .5f, false);
			}
		}
	}
}

void ARPGCharacter::PlayerFaceTargetNPC()
{
	if (!TargetNPC) return;

	const FRotator PlayerRot = UKismetMathLibrary::FindLookAtRotation(RootComponent->GetComponentLocation(), TargetNPC->GetActorLocation());

	RootComponent->SetWorldRotation(PlayerRot);
}

void ARPGCharacter::RemoveAggressiveMenu()
{
	AggressiveMenu->RemoveFromViewport();
		
	GetWorldTimerManager().ClearTimer(TimeDilationHandle);
	SetRegularTimeDilation();
}

void ARPGCharacter::RemoveTargetNPC()
{
	if (InteractiveMenu->IsInViewport())
	{
		TargetNPC->NPCEndTrade();
		InteractiveMenu->RemoveFromViewport();
	}
	else if (AggressiveMenu->IsInViewport())
	{
		RemoveAggressiveMenu();
	}
	else if (TradeMenu->IsInViewport())
	{
		TargetNPC->NPCEndTrade();
		TradeMenu->RemoveFromViewport();
	}
	else if (InventoryTradeMenu->IsInViewport())
	{
		TargetNPC->NPCEndTrade();
		InventoryTradeMenu->RemoveFromViewport();
	}
	
	TargetNPC = nullptr;
}

void ARPGCharacter::Interact()
{
	if (GetCharacterState() != ERPGCharacterState::Normal && GetCharacterState() != ERPGCharacterState::Combat) return;
	// only use when standing still

	FVector Loc = RootComponent->GetComponentLocation();
	Loc.Z = 93.f;
	RootComponent->SetWorldLocation(Loc);
	
	FVector BoxLoc = GetRootComponent()->GetComponentLocation();
	BoxLoc.Z -= 75.f;
	// const FVector BoxExtent = FVector(50.f, 50.f, 15.f);
	const FVector BoxExtent = FVector(100.f, 100.f, 5.f);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1)); // Dropped
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	TArray<AActor*> OutActors;

	UKismetSystemLibrary::DrawDebugBox(GetWorld(), BoxLoc, BoxExtent, FColor::Red, FRotator(0.f, 0.f, 0.f), 2.f);
	UKismetSystemLibrary::BoxOverlapActors(GetWorld(), RootComponent->GetComponentLocation(), BoxExtent, ObjectTypes, nullptr, ActorsToIgnore, OutActors);

	// // UE_LOG(LogTemp, Warning, TEXT("interacted: "));
	// for (AActor* a : OutActors)
	// {
	// 	// UE_LOG(LogTemp, Warning, TEXT("%s"), &a->Tags[0]);
	// 	UE_LOG(LogTemp, Warning, TEXT("asfaf"));
	// }

	if (!OutActors.IsEmpty())
	{
		if (OutActors[0]->ActorHasTag("Weapon"))
		{
			// CurrentWeapon = Cast<ARPGWeapon>(OutActors[0]);
			WeaponPickUp(Cast<ARPGWeapon>(OutActors[0]));
			UE_LOG(LogTemp, Warning, TEXT("WEAPON PICKUP"));
			return;
		}
		if (OutActors[0]->ActorHasTag("Throwable"))
		{
			ThrowablePickUp(Cast<ARPGThrowable>(OutActors[0]));
			UE_LOG(LogTemp, Warning, TEXT("THROWABLE PICKUP"));
			return;
		}
		if (OutActors[0]->ActorHasTag("Item"))
		{
			ItemPickUp(Cast<ARPGItem>(OutActors[0]));
			UE_LOG(LogTemp, Warning, TEXT("ITEM PICKUP"));
			return;
		}
		if (OutActors[0]->ActorHasTag("Armour"))
		{
			ArmourPickUp(Cast<ARPGArmour>(OutActors[0]));
			UE_LOG(LogTemp, Warning, TEXT("ARMOUR PICKUP"));
			return;
		}
		if (OutActors[0]->ActorHasTag("Bag"))
		{
			if (GetCharacterState() != ERPGCharacterState::Normal) return;
			SelectedBag = Cast<ARPGBag>(OutActors[0]);
			StartBagScreen();
			UE_LOG(LogTemp, Warning, TEXT("BAG MENU"));
			return;
		}
	}
	
	// write the rest
}

void ARPGCharacter::WeaponPickUp(ARPGWeapon* Weapon)
{
	if (CurrentWeapon)
	{
		WeaponDrop();
		
		Weapon->SetActorHiddenInGame(true);
		const FAttachmentTransformRules AttachRules(EAttachmentRule::KeepRelative, true);
		Weapon->AttachToComponent(GetMesh(), AttachRules, "InventorySocket");
		Weapon->SetDroppedWeapon(false);

		CurrentWeapon = Weapon;
		return;
	}
	Weapon->SetActorHiddenInGame(true);
	const FAttachmentTransformRules AttachRules(EAttachmentRule::KeepRelative, true);
	Weapon->AttachToComponent(GetMesh(), AttachRules, "InventorySocket");
	Weapon->SetDroppedWeapon(false);

	CurrentWeapon = Weapon;
}

void ARPGCharacter::WeaponDrop(ARPGWeapon* Weapon)
{
	if (!Weapon && !CurrentWeapon) return;
	// drop primary weapon

	// detach weapon from socket
	// move weapon to the ground under the player
	// start rendering the weapon
	if (!Weapon && CurrentWeapon)
	{
		Weapon = CurrentWeapon;
		CurrentWeapon = nullptr;
	}

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

bool ARPGCharacter::ItemPickUp(ARPGItem* Item)
{
	for (uint8 i = 0; i < CharacterInventory->GetInventorySize(); i++)
	{
		if (CharacterInventory->GetItemInventory()[i] == nullptr)
		{
			Item->SetActorHiddenInGame(true);
			const FAttachmentTransformRules AttachRules(EAttachmentRule::KeepRelative, true);
			Item->AttachToComponent(GetMesh(), AttachRules, "InventorySocket");

			Item->SetDroppedItem(false);
			CharacterInventory->SetItemInventory(Item, i);
			
			return true;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("INVENTORY FULL"));
	return false;
}

void ARPGCharacter::ItemDrop(uint8 Pos)
{
	ARPGItem* Item = CharacterInventory->GetItemInventory()[Pos];
	
	if (SelectedItem == Item) RemoveSelectedItem();

	CharacterInventory->SetItemInventory(nullptr, Pos);
	
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

void ARPGCharacter::ArmourPickUp(ARPGArmour* Armour)
{
	const FAttachmentTransformRules AttachRules(EAttachmentRule::KeepRelative, true);
	switch (Armour->GetArmourType())
	{
	case ERPGArmourType::Head:
		{
			if (GetCharacterHeadArmour())
			{
				ArmourDrop(ERPGArmourType::Head);
				Armour->SetActorHiddenInGame(true);
				Armour->AttachToComponent(GetMesh(), AttachRules, "InventorySocket");
				Armour->SetDroppedArmour(false);
				HeadArmour = Armour;
				UE_LOG(LogTemp, Warning, TEXT("HEAD ARMOUR PICKED UP"));
				return;
			}
			Armour->SetActorHiddenInGame(true);
			Armour->AttachToComponent(GetMesh(), AttachRules, "InventorySocket");
			Armour->SetDroppedArmour(false);
			HeadArmour = Armour;
			UE_LOG(LogTemp, Warning, TEXT("HEAD ARMOUR PICKED UP"));
			break;
		}
	case ERPGArmourType::Chest:
		{
			if (GetCharacterChestArmour())
			{
				ArmourDrop(ERPGArmourType::Chest);
				Armour->SetActorHiddenInGame(true);
				Armour->AttachToComponent(GetMesh(), AttachRules, "InventorySocket");
				Armour->SetDroppedArmour(false);
				ChestArmour = Armour;
				UE_LOG(LogTemp, Warning, TEXT("CHEST ARMOUR PICKED UP"));
				return;
			}
			Armour->SetActorHiddenInGame(true);
			Armour->AttachToComponent(GetMesh(), AttachRules, "InventorySocket");
			Armour->SetDroppedArmour(false);
			ChestArmour = Armour;
			UE_LOG(LogTemp, Warning, TEXT("CHEST ARMOUR PICKED UP"));
			break;
		}
	case ERPGArmourType::Legs:
		{
			if (GetCharacterLegsArmour())
			{
				ArmourDrop(ERPGArmourType::Legs);
				Armour->SetActorHiddenInGame(true);
				Armour->AttachToComponent(GetMesh(), AttachRules, "InventorySocket");
				Armour->SetDroppedArmour(false);
				LegsArmour = Armour;
				UE_LOG(LogTemp, Warning, TEXT("LEGS ARMOUR PICKED UP"));
				return;
			}
			Armour->SetActorHiddenInGame(true);
			Armour->AttachToComponent(GetMesh(), AttachRules, "InventorySocket");
			Armour->SetDroppedArmour(false);
			LegsArmour = Armour;
			UE_LOG(LogTemp, Warning, TEXT("LEGS ARMOUR PICKED UP"));
			break;
		}
	}
}

void ARPGCharacter::ArmourDrop(ERPGArmourType Type)
{
	// position to drop the armour
	FVector DropPos = RootComponent->GetComponentLocation();
	DropPos.Z -= 49.f;
	switch (Type)
	{
	case ERPGArmourType::Head:
		{
			HeadArmour->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			HeadArmour->SetActorLocation(DropPos);
			HeadArmour->SetActorRotation(FRotator(0.f, 0.f, 0.f));
			HeadArmour->SetActorHiddenInGame(false);
			HeadArmour->SetActorEnableCollision(true);
			HeadArmour->SetDroppedArmour(true);
			HeadArmour = nullptr;
			UE_LOG(LogTemp, Warning, TEXT("HEAD ARMOUR DROPPED"));
			break;
		}
	case ERPGArmourType::Chest:
		{
			ChestArmour->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			ChestArmour->SetActorLocation(DropPos);
			ChestArmour->SetActorRotation(FRotator(0.f, 0.f, 0.f));
			ChestArmour->SetActorHiddenInGame(false);
			ChestArmour->SetActorEnableCollision(true);
			ChestArmour->SetDroppedArmour(true);
			ChestArmour = nullptr;
			UE_LOG(LogTemp, Warning, TEXT("CHEST ARMOUR DROPPED"));
			break;
		}
	case ERPGArmourType::Legs:
		{
			LegsArmour->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			LegsArmour->SetActorLocation(DropPos);
			LegsArmour->SetActorRotation(FRotator(0.f, 0.f, 0.f));
			LegsArmour->SetActorHiddenInGame(false);
			LegsArmour->SetActorEnableCollision(true);
			LegsArmour->SetDroppedArmour(true);
			LegsArmour = nullptr;
			UE_LOG(LogTemp, Warning, TEXT("LEGS ARMOUR DROPPED"));
			break;
		}
	}
}

void ARPGCharacter::ThrowablePickUp(ARPGThrowable* Throwable)
{
	const FAttachmentTransformRules AttachRules(EAttachmentRule::KeepRelative, true);
	
	// if (ThrowableWeapon)
	// {
	// 	ThrowableUnEquip();
	//
	// 	Throwable->SetActorHiddenInGame(true);
	// 	Throwable->AttachToComponent(GetMesh(), AttachRules, "InventorySocket");
	// 	Throwable->SetDroppedItem(false);
	// 	ThrowableWeapon = Throwable;
	// 	return;
	// }

	ThrowableUnEquip();
	
	Throwable->SetActorHiddenInGame(true);
	Throwable->AttachToComponent(GetMesh(), AttachRules, "InventorySocket");
	Throwable->SetDroppedItem(false);
	ThrowableWeapon = Throwable;
}

void ARPGCharacter::ThrowableDrop(ARPGThrowable* Throwable)
{
	// position to drop the throwable weapon
	FVector DropPos = RootComponent->GetComponentLocation();
	DropPos.Z -= 49.f;

	// FDetachmentTransformRules DetachRules;
	// DetachRules.LocationRule(DropPos);
	Throwable->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Throwable->SetActorLocation(DropPos);
	Throwable->SetActorRotation(FRotator(0.f, 0.f, 0.f));
	Throwable->SetActorHiddenInGame(false);
	Throwable->SetActorEnableCollision(true);
	Throwable->SetDroppedItem(true);
}

void ARPGCharacter::ThrowableEquip(ARPGThrowable* Throwable)
{	
	GetCharacterInventory()->SetItemInventory(nullptr, SelectedItemID);
	
	ThrowableUnEquip();
	
	ThrowableWeapon = Throwable;
	
	RemoveSelectedItem();
}

void ARPGCharacter::ThrowableUnEquip()
{
	if (!ThrowableWeapon) return;

	if (!AddToInventory(ThrowableWeapon))
	{
		ThrowableDrop(ThrowableWeapon);
	}
	ThrowableWeapon = nullptr;
}

bool ARPGCharacter::AddToInventory(ARPGItem* Item)
{
	for (uint8 i = 0; i < CharacterInventory->GetInventorySize(); i++)
	{
		if (CharacterInventory->GetItemInventory()[i] == nullptr)
		{
			CharacterInventory->SetItemInventory(Item, i);
			return true;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("INVENTORY FULL"));
	return false;
}

void ARPGCharacter::CharacterGetExp(const float ExpValMin, const float ExpValMax)
{
	const float ExpVal = FMath::RandRange(ExpValMin, ExpValMax);
	UE_LOG(LogTemp, Warning, TEXT("Player got %f exp"), ExpVal);
	if (CurrentExp + ExpVal >= MaxExp)
	{
		++LevelUpPoints;

		const float Leftover = (CurrentExp + ExpVal) - MaxExp;

		CurrentExp = Leftover;

		MaxExp *= ExpIncrease;
		ExpIncrease *= 1.15;
		return;
	}
	CurrentExp += ExpVal;
}

void ARPGCharacter::StartLevelUpScreen()
{
	if (GetCharacterState() != ERPGCharacterState::Normal && GetCharacterState() != ERPGCharacterState::Rest) return;

	if (GetCharacterCurrentHealth() != GetCharacterMaxHealth())
	{
		UE_LOG(LogTemp, Warning, TEXT("FULL HP REQUIRED"));
		return;
	}
	
	if (LevelUpPoints == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("NO LEVEL UP POINTS"));
		return;
	}
	
	RemoveTargetNPC();
	if (HealMenu->IsInViewport()) StopHealScreen();
	if (!LevelUpMenu->IsInViewport()) LevelUpMenu->AddToViewport();
	else
	{
		StopLevelUpScreen();
		return;
	}
	
	ChangeCharacterStateRest();
}

void ARPGCharacter::StopLevelUpScreen()
{
	if (GetCharacterState() != ERPGCharacterState::Rest) return;
	
	if (LevelUpMenu->IsInViewport()) LevelUpMenu->RemoveFromViewport();

	ChangeCharacterStateNormal();
}

void ARPGCharacter::CharacterLevelUp(uint8 Stat)
{
	if (GetCharacterState() != ERPGCharacterState::Normal && GetCharacterCurrentHealth() != GetCharacterMaxHealth()) return;

	// ChangeCharacterStateRest();
	
	if (LevelUpPoints == 0) return;

	switch (Stat)
	{
	case 0: // health
		{
			if (Health == 10)
			{
				UE_LOG(LogTemp, Warning, TEXT("health is already maxed"));
				return;
			}
			MaxHealth += 5.f;
			LArmMaxHealth += 5.f;
			RArmMaxHealth += 5.f;
			LLegMaxHealth += 5.f;
			RLegMaxHealth += 5.f;

			CurrentHealth = GetCharacterMaxHealth();
			LArmCurrentHealth = GetCharacterLArmMaxHealth();
			RArmCurrentHealth = GetCharacterRArmMaxHealth();
			LLegCurrentHealth = GetCharacterLLegMaxHealth();
			RLegCurrentHealth = GetCharacterLLegMaxHealth();

			++Health;
			UE_LOG(LogTemp, Warning, TEXT("HEALTH INCREASED"));
			break;
		}
	case 1: // strength
		{
			if (Strength == 10)
			{
				UE_LOG(LogTemp, Warning, TEXT("strength is already maxed"));
				return;
			}

			CharacterStrength += .1f;

			// InventoryMaxCarryWeight = InventoryInitialCarryWeight * CharacterStrength;

			CharacterInventory->IncreaseInventorySize();

			++Strength;
			UE_LOG(LogTemp, Warning, TEXT("STRENGTH INCREASED"));
			break;
		}
	case 2: // accuracy
		{
			if (Accuracy == 10)
			{
				UE_LOG(LogTemp, Warning, TEXT("accuracy is already maxed"));
				return;
			}

			CharacterAccuracy += .05;

			++Accuracy;
			UE_LOG(LogTemp, Warning, TEXT("ACCURACY INCREASED"));
			break;
		}
	case 3: // agility
		{
			if (Agility == 10)
			{
				UE_LOG(LogTemp, Warning, TEXT("agility is already maxed"));
				return;
			}

			WalkSpeed += 5.f;
			CrouchSpeed += 5.f;

			ChangeCharacterMoveSpeed();

			++Agility;
			UE_LOG(LogTemp, Warning, TEXT("AGILITY INCREASED"));
			break;
		}
	default: { break; }
	}
	
	--LevelUpPoints;
}

void ARPGCharacter::ChangeCharacterMoveSpeed(const float Modifier)
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

void ARPGCharacter::SetMoveSpeedDebuff(const bool Value)
{
	MoveSpeedDebuff = Value;
	if (GetCharacterState() == ERPGCharacterState::Reloading)
	{
		ChangeCharacterMoveSpeed(.5f);
		return;
	}
	ChangeCharacterMoveSpeed();
}

void ARPGCharacter::StartHealScreen()
{
	if (GetCharacterState() != ERPGCharacterState::Normal && GetCharacterState() != ERPGCharacterState::Combat) return;

	// check if there are healing items in inventory
	for (uint8 i = 0; i < GetCharacterInventory()->GetInventorySize(); i++)
	{
		if (GetCharacterInventory()->GetItemInventory()[i])
		{
			if (GetCharacterInventory()->GetItemInventory()[i]->ActorHasTag("Heal"))
			{
				SetSelectedItem(GetCharacterInventory()->GetItemInventory()[i]);
				SetSelectedItemID(i);
				break;
			}
		}
	}

	if (!SelectedItem)
	{
		UE_LOG(LogTemp, Warning, TEXT("NO HEALING ITEM"));
		return;
	}
	
	RemoveTargetNPC();
	if (!HealMenu->IsInViewport()) HealMenu->AddToViewport();
	else
	{
		StopHealScreen();
	}
}

void ARPGCharacter::StopHealScreen()
{
	if (GetCharacterState() != ERPGCharacterState::Normal && GetCharacterState() != ERPGCharacterState::Combat
		&& GetCharacterState() != ERPGCharacterState::Inventory && GetCharacterState() != ERPGCharacterState::Heal) return;
	
	if (HealMenu->IsInViewport()) HealMenu->RemoveFromViewport();

	if (GetSelectedItem()) RemoveSelectedItem();
}

void ARPGCharacter::CharacterHeal(uint8 BodyPart)
{
	if (GetCharacterState() != ERPGCharacterState::Normal && GetCharacterState() != ERPGCharacterState::Combat && GetCharacterState() != ERPGCharacterState::Inventory) return;

	BodyPartToHeal = BodyPart;

	FTimerHandle CharacterHealDelayHandle;
	// GetWorldTimerManager().SetTimer(CharacterHealDelayHandle, this, &ARPGCharacter::CharacterHealDelay, 1.7f, false);

	// wont work - find anmother way
	// chngaes state too late

	switch (BodyPart)
	{
	case 0: {}	// head
	case 1:		// chest
		{
			if (GetCharacterCurrentHealth() == GetCharacterMaxHealth())
			{
				UE_LOG(LogTemp, Warning, TEXT("HEAD/CHEST ALREADY AT MAX HP"));
				return;
			}
			if (GetCharacterLArmCurrentHealth() != GetCharacterLArmMaxHealth() || GetCharacterRArmCurrentHealth() != GetCharacterRArmMaxHealth() ||
				GetCharacterLLegCurrentHealth() != GetCharacterLLegMaxHealth() || GetCharacterRLegCurrentHealth() != GetCharacterRLegMaxHealth())
			{
				UE_LOG(LogTemp, Warning, TEXT("heal limbs before healing head/chest"));
				return;
			}

			if (GetCharacterState() == ERPGCharacterState::Inventory)
			{
				CharacterHealDelay();
				break;
			}
			
			ChangeCharacterStateHeal();
			GetWorldTimerManager().SetTimer(CharacterHealDelayHandle, this, &ARPGCharacter::CharacterHealDelay, 1.7f, false);
			
			// CurrentHealth = GetCharacterMaxHealth();

			// DestroySelectedItem();
			// UE_LOG(LogTemp, Warning, TEXT("Head/Chest Heal"));
			break;
		}
	case 2:		// larm 
		{
			if (GetCharacterLArmCurrentHealth() == GetCharacterLArmMaxHealth())
			{
				UE_LOG(LogTemp, Warning, TEXT("LEFT ARM ALREADY AT MAX HP"));
				return;
			}

			if (GetCharacterState() == ERPGCharacterState::Inventory)
			{
				CharacterHealDelay();
				break;
			}
			
			ChangeCharacterStateHeal();
			GetWorldTimerManager().SetTimer(CharacterHealDelayHandle, this, &ARPGCharacter::CharacterHealDelay, 1.7f, false);

			// const float HealValue = GetCharacterLArmMaxHealth() - GetCharacterLArmCurrentHealth();
			// GetCharacterCurrentHealth() + HealValue >= GetCharacterMaxHealth() ? CurrentHealth = GetCharacterMaxHealth() : CurrentHealth += HealValue;
			// // if (GetCharacterCurrentHealth() + HealValue >= GetCharacterMaxHealth()) CurrentHealth = GetCharacterMaxHealth();
			// // else CurrentHealth += HealValue;
			//
			// LArmCurrentHealth = GetCharacterLArmMaxHealth();
			// if (AccuracyDebuff && GetCharacterRArmCurrentHealth() > 0.f) AccuracyDebuff = false;
			//
			// DestroySelectedItem();
			// UE_LOG(LogTemp, Warning, TEXT("Left Arm Heal"));
			break;
		}
	case 3:		// rarm
		{
			if (GetCharacterRArmCurrentHealth() == GetCharacterRArmMaxHealth())
			{
				UE_LOG(LogTemp, Warning, TEXT("RIGHT ARM ALREADY AT MAX HP"));
				return;
			}
			
			if (GetCharacterState() == ERPGCharacterState::Inventory)
			{
				CharacterHealDelay();
				break;
			}
			
			ChangeCharacterStateHeal();
			GetWorldTimerManager().SetTimer(CharacterHealDelayHandle, this, &ARPGCharacter::CharacterHealDelay, 1.7f, false);

			// const float HealValue = GetCharacterRArmMaxHealth() - GetCharacterRArmCurrentHealth();
			// GetCharacterCurrentHealth() + HealValue >= GetCharacterMaxHealth() ? CurrentHealth = GetCharacterMaxHealth() : CurrentHealth += HealValue;
			//
			// RArmCurrentHealth = GetCharacterRArmMaxHealth();
			// if (AccuracyDebuff && GetCharacterLArmCurrentHealth() > 0.f) AccuracyDebuff = false;
			//
			// DestroySelectedItem();
			// UE_LOG(LogTemp, Warning, TEXT("Right Arm Heal"));
			break;
		}
	case 4:		// lleg
		{
			if (GetCharacterLLegCurrentHealth() == GetCharacterLLegMaxHealth())
			{
				UE_LOG(LogTemp, Warning, TEXT("LEFT LEG ALREADY AT MAX HP"));
				return;
			}

			if (GetCharacterState() == ERPGCharacterState::Inventory)
			{
				CharacterHealDelay();
				break;
			}
			
			ChangeCharacterStateHeal();
			GetWorldTimerManager().SetTimer(CharacterHealDelayHandle, this, &ARPGCharacter::CharacterHealDelay, 1.7f, false);

			// const float HealValue = GetCharacterLLegMaxHealth() - GetCharacterLLegCurrentHealth();
			// GetCharacterCurrentHealth() + HealValue >= GetCharacterMaxHealth() ? CurrentHealth = GetCharacterMaxHealth() : CurrentHealth += HealValue;
			//
			// LLegCurrentHealth = GetCharacterLLegMaxHealth();
			// if (MoveSpeedDebuff && GetCharacterRLegCurrentHealth() > 0) SetMoveSpeedDebuff(false);
			//
			// DestroySelectedItem();
			// UE_LOG(LogTemp, Warning, TEXT("Left leg Heal"));
			break;
		}
	case 5:		// rleg
		{
			if (GetCharacterRLegCurrentHealth() == GetCharacterRLegMaxHealth())
			{
				UE_LOG(LogTemp, Warning, TEXT("RIGHT LEG ALREADY AT MAX HP"));
				return;
			}

			if (GetCharacterState() == ERPGCharacterState::Inventory)
			{
				CharacterHealDelay();
				break;
			}
			
			ChangeCharacterStateHeal();
			GetWorldTimerManager().SetTimer(CharacterHealDelayHandle, this, &ARPGCharacter::CharacterHealDelay, 1.7f, false);

			// const float HealValue = GetCharacterRLegMaxHealth() - GetCharacterRLegCurrentHealth();
			// GetCharacterCurrentHealth() + HealValue >= GetCharacterMaxHealth() ? CurrentHealth = GetCharacterMaxHealth() : CurrentHealth += HealValue;
			//
			// RLegCurrentHealth = GetCharacterRLegMaxHealth();
			// if (MoveSpeedDebuff && GetCharacterLLegCurrentHealth() > 0) SetMoveSpeedDebuff(false);
			//
			// DestroySelectedItem();
			// UE_LOG(LogTemp, Warning, TEXT("Right Leg Heal"));
			break;
		}
	default: { break; }
	}

	// StopHealScreen();
}

void ARPGCharacter::CharacterHealDelay()
{
	switch (BodyPartToHeal)
	{
	case 0: {}
	case 1:
		{
			CurrentHealth = GetCharacterMaxHealth();
			DestroySelectedItem();
			UE_LOG(LogTemp, Warning, TEXT("Head/Chest Heal"));
			break;
		}
	case 2:
		{
			const float HealValue = GetCharacterLArmMaxHealth() - GetCharacterLArmCurrentHealth();
			GetCharacterCurrentHealth() + HealValue >= GetCharacterMaxHealth() ? CurrentHealth = GetCharacterMaxHealth() : CurrentHealth += HealValue;
			// if (GetCharacterCurrentHealth() + HealValue >= GetCharacterMaxHealth()) CurrentHealth = GetCharacterMaxHealth();
			// else CurrentHealth += HealValue;
			
			LArmCurrentHealth = GetCharacterLArmMaxHealth();
			if (AccuracyDebuff && GetCharacterRArmCurrentHealth() > 0.f) AccuracyDebuff = false;

			DestroySelectedItem();
			UE_LOG(LogTemp, Warning, TEXT("Left Arm Heal"));
			break;
		}
	case 3:
		{
			const float HealValue = GetCharacterRArmMaxHealth() - GetCharacterRArmCurrentHealth();
			GetCharacterCurrentHealth() + HealValue >= GetCharacterMaxHealth() ? CurrentHealth = GetCharacterMaxHealth() : CurrentHealth += HealValue;
			
			RArmCurrentHealth = GetCharacterRArmMaxHealth();
			if (AccuracyDebuff && GetCharacterLArmCurrentHealth() > 0.f) AccuracyDebuff = false;

			DestroySelectedItem();
			UE_LOG(LogTemp, Warning, TEXT("Right Arm Heal"));
			break;
		}
	case 4:
		{
			const float HealValue = GetCharacterLLegMaxHealth() - GetCharacterLLegCurrentHealth();
			GetCharacterCurrentHealth() + HealValue >= GetCharacterMaxHealth() ? CurrentHealth = GetCharacterMaxHealth() : CurrentHealth += HealValue;
			
			LLegCurrentHealth = GetCharacterLLegMaxHealth();
			if (MoveSpeedDebuff && GetCharacterRLegCurrentHealth() > 0) SetMoveSpeedDebuff(false);

			DestroySelectedItem();
			UE_LOG(LogTemp, Warning, TEXT("Left leg Heal"));
			break;
		}
	case 5:
		{
			const float HealValue = GetCharacterRLegMaxHealth() - GetCharacterRLegCurrentHealth();
			GetCharacterCurrentHealth() + HealValue >= GetCharacterMaxHealth() ? CurrentHealth = GetCharacterMaxHealth() : CurrentHealth += HealValue;
			
			RLegCurrentHealth = GetCharacterRLegMaxHealth();
			if (MoveSpeedDebuff && GetCharacterLLegCurrentHealth() > 0) SetMoveSpeedDebuff(false);

			DestroySelectedItem();
			UE_LOG(LogTemp, Warning, TEXT("Right Leg Heal"));
			break;
		}
		default: { break; }
	}
	StopHealScreen();
}

void ARPGCharacter::StartInventoryScreen()
{
	if (GetCharacterState() != ERPGCharacterState::Normal && GetCharacterState() != ERPGCharacterState::Inventory) return;
	if (BagMenu->IsInViewport()) return;
	
	RemoveTargetNPC();
	if (HealMenu->IsInViewport()) StopHealScreen();
	if (!InventoryMenu->IsInViewport()) InventoryMenu->AddToViewport();
	else
	{
		StopInventoryScreen();
		return;
	}
	
	ChangeCharacterStateInventory();
}

void ARPGCharacter::StopInventoryScreen()
{
	if (GetCharacterState() != ERPGCharacterState::Inventory) return;

	RemoveSelectedItem();
	
	if (InventoryMenu->IsInViewport()) InventoryMenu->RemoveFromViewport();

	ChangeCharacterStateNormal();
}

void ARPGCharacter::StartBagScreen()
{
	if (GetCharacterState() != ERPGCharacterState::Normal) return;
	
	RemoveTargetNPC();
	if (HealMenu->IsInViewport()) StopHealScreen();
	if (!BagMenu->IsInViewport()) BagMenu->AddToViewport();
	else
	{
		StopBagScreen();
		return;
	}

	SelectedBag->BagClosedTimer(false);
	
	ChangeCharacterStateInventory();
}

void ARPGCharacter::StopBagScreen()
{
	if (GetCharacterState() != ERPGCharacterState::Inventory) return;

	if (SelectedBag)
	{
		SelectedBag->BagClosedTimer(true);
		SelectedBag = nullptr;
	}
	
	if (BagMenu->IsInViewport()) BagMenu->RemoveFromViewport();
	
	// check if this causes problems
	ChangeCharacterStateNormal();
}

bool ARPGCharacter::BCharacterWeapon() const
{
	if (CurrentWeapon) return true;
	return false;
}

bool ARPGCharacter::BCharacterThrowable() const
{
	if (ThrowableWeapon) return true;
	return false;
}

bool ARPGCharacter::BCharacterHeadArmour() const
{
	if (HeadArmour) return true;
	return false;
}

bool ARPGCharacter::BCharacterChestArmour() const
{
	if (ChestArmour) return true;
	return false;
}

bool ARPGCharacter::BCharacterLegsArmour() const
{
	if (LegsArmour) return true;
	return false;
}

void ARPGCharacter::SetSelectedItem(ARPGItem* Item)
{
	if (SelectedItem) RemoveSelectedItem();

	SelectedItem = Item;
}

void ARPGCharacter::RemoveSelectedItem()
{
	if (!SelectedItem) return;

	SelectedItem->StopScreen();
	SelectedItem = nullptr;
}

void ARPGCharacter::DestroySelectedItem()
{
	if (!SelectedItem) return;

	SelectedItem->ItemUsed();

	GetCharacterInventory()->SetItemInventory(nullptr, SelectedItemID);

	SelectedItem = nullptr;
	
	// set to nullptr
}

void ARPGCharacter::CharacterRepairArmour(uint8 Type)
{
	if (GetCharacterState() != ERPGCharacterState::Inventory) return;

	switch (Type)
	{
	case 0:		// head
		{
			if (!BCharacterHeadArmour())
			{
				UE_LOG(LogTemp, Warning, TEXT("NO HEAD ARMOUR"));
				return;
			}
			if (GetCharacterHeadArmour()->GetArmourCurrentDurability() == GetCharacterHeadArmour()->GetArmourMaxDurability())
			{
				UE_LOG(LogTemp, Warning, TEXT("HEAD ALREADY AT MAX DURABILITY"));
				return;
			}
			
			GetCharacterHeadArmour()->RepairArmour();

			DestroySelectedItem();
			UE_LOG(LogTemp, Warning, TEXT("Head Repair"));
			break;
		}
	case 1:		// chest 
		{
			if (!BCharacterChestArmour())
			{
				UE_LOG(LogTemp, Warning, TEXT("NO CHEST ARMOUR"));
				return;
			}
			if (GetCharacterChestArmour()->GetArmourCurrentDurability() == GetCharacterChestArmour()->GetArmourMaxDurability())
			{
				UE_LOG(LogTemp, Warning, TEXT("CHEST ALREADY AT MAX DURABILITY"));
				return;
			}
			
			GetCharacterChestArmour()->RepairArmour();

			DestroySelectedItem();
			UE_LOG(LogTemp, Warning, TEXT("Chest Repair"));
			break;
		}
	case 2:		// legs
		{
			if (!BCharacterLegsArmour())
			{
				UE_LOG(LogTemp, Warning, TEXT("NO LEGS ARMOUR"));
				return;
			}
			if (GetCharacterLegsArmour()->GetArmourCurrentDurability() == GetCharacterLegsArmour()->GetArmourMaxDurability())
			{
				UE_LOG(LogTemp, Warning, TEXT("LEGS ALREADY AT MAX DURABILITY"));
				return;
			}
			
			GetCharacterLegsArmour()->RepairArmour();

			DestroySelectedItem();
			UE_LOG(LogTemp, Warning, TEXT("Legs Repair"));
			break;
		}
	default: { break; }
	}
}

void ARPGCharacter::BagPickUpWeapon()
{
	if (!SelectedBag) return;
	
	if (CurrentWeapon)
	{
		ARPGWeapon* Temp = CurrentWeapon;
		CurrentWeapon = nullptr;
		WeaponPickUp(SelectedBag->GetBagWeapon());
		SelectedBag->SetBagWeapon(Temp);
		Temp = nullptr;
		return;
	}
	WeaponPickUp(SelectedBag->GetBagWeapon());
	SelectedBag->SetBagWeapon(nullptr);
}

void ARPGCharacter::BagPickUpThrowable()
{
	if (!SelectedBag) return;
	
	if (ThrowableWeapon)
	{
		ARPGThrowable* Temp = ThrowableWeapon;
		ThrowableWeapon = nullptr;
		ThrowablePickUp(SelectedBag->GetBagThrowable());
		if (AddToInventory(Temp)) SelectedBag->SetBagThrowable(nullptr);
		else SelectedBag->SetBagThrowable(Temp);
		Temp = nullptr;
		return;
	}
	ThrowablePickUp(SelectedBag->GetBagThrowable());
	SelectedBag->SetBagThrowable(nullptr);
}

void ARPGCharacter::BagPickUpHeadArmour()
{
	if (!SelectedBag) return;
	
	if (HeadArmour)
	{
		ARPGArmour* Temp = HeadArmour;
		HeadArmour = nullptr;
		ArmourPickUp(SelectedBag->GetBagHeadArmour());
		SelectedBag->SetBagHeadArmour(Temp);
		Temp = nullptr;
		return;
	}
	ArmourPickUp(SelectedBag->GetBagHeadArmour());
	SelectedBag->SetBagHeadArmour(nullptr);
}

void ARPGCharacter::BagPickUpChestArmour()
{
	if (!SelectedBag) return;
	
	if (ChestArmour)
	{
		ARPGArmour* Temp = ChestArmour;
		ChestArmour = nullptr;
		ArmourPickUp(SelectedBag->GetBagChestArmour());
		SelectedBag->SetBagChestArmour(Temp);
		Temp = nullptr;
		return;
	}
	ArmourPickUp(SelectedBag->GetBagChestArmour());
	SelectedBag->SetBagChestArmour(nullptr);
}


void ARPGCharacter::BagPickUpLegsArmour()
{
	if (!SelectedBag) return;
	
	if (LegsArmour)
	{
		ARPGArmour* Temp = LegsArmour;
		LegsArmour = nullptr;
		ArmourPickUp(SelectedBag->GetBagLegsArmour());
		SelectedBag->SetBagLegsArmour(Temp);
		Temp = nullptr;
		return;
	}
	ArmourPickUp(SelectedBag->GetBagLegsArmour());
	SelectedBag->SetBagLegsArmour(nullptr);
}

void ARPGCharacter::BagPickUpItem(uint8 Pos)
{
	if (!SelectedBag) return;
	
	if (ItemPickUp(SelectedBag->GetBagInventory()->GetItemInventory()[Pos])) SelectedBag->GetBagInventory()->SetItemInventory(nullptr, Pos);
}

void ARPGCharacter::CharacterThrow()
{
	if (!ThrowableWeapon) return;
	if (ThrowableWeapon->GetThrowableType() != ERPGThrowableType::Shield && !TargetNPC) return;
	if (GetCharacterState() != ERPGCharacterState::Normal && GetCharacterState() != ERPGCharacterState::Combat) return;

	ChangeCharacterStateThrowing();

	FTimerHandle CharacterThrowDelayHandle;
	GetWorldTimerManager().SetTimer(CharacterThrowDelayHandle, this, &ARPGCharacter::CharacterThrowDelay, .5f, false);
}

void ARPGCharacter::CharacterThrowDelay()
{
	switch (ThrowableWeapon->GetThrowableType())
	{
	case ERPGThrowableType::Explosive: {}
	case ERPGThrowableType::Incendiary: {}
	case ERPGThrowableType::Slow:
		{
			ThrowableWeapon->ThrowableThrow(TargetNPC, this);
			// CheckTargetDead(TargetNPC);
			break;
		}
	case ERPGThrowableType::Shield:
		{
			Cast<ARPGThrowableShield>(ThrowableWeapon)->ThrowableThrowShield(GetRootComponent()->GetComponentLocation(), GetRootComponent()->GetForwardVector());
			break;
		}
	}

	if (ThrowableWeapon->DecreaseRemainingUses()) ThrowableWeapon = nullptr;
}

void ARPGCharacter::StartThrowableSlow(float Duration)
{
	SetMoveSpeedDebuff(true);
	GetWorldTimerManager().ClearTimer(MoveSpeedRemoveDebuffHandle);
	GetWorldTimerManager().SetTimer(MoveSpeedRemoveDebuffHandle, this, &ARPGCharacter::StopThrowableSlow, Duration, false);
}

void ARPGCharacter::StopThrowableSlow()
{
	if (GetCharacterLLegCurrentHealth() == 0.f || GetCharacterRLegCurrentHealth() == 0.f) return;
	SetMoveSpeedDebuff(false);
}

void ARPGCharacter::StartTradeScreen()
{
	if (GetCharacterState() != ERPGCharacterState::Normal && GetCharacterState() != ERPGCharacterState::Inventory) return;
	if (!TargetNPC) return;

	if (InteractiveMenu->IsInViewport()) InteractiveMenu->RemoveFromViewport();
	if (InventoryTradeMenu->IsInViewport()) InventoryTradeMenu->RemoveFromViewport();
	if (!TradeMenu->IsInViewport()) TradeMenu->AddToViewport();
	else
	{
		StopTradeScreen();
		return;
	}
	ChangeCharacterStateInventory();
}

void ARPGCharacter::StopTradeScreen()
{
	if (GetCharacterState() != ERPGCharacterState::Inventory) return;
	if (TradeMenu->IsInViewport()) TradeMenu->RemoveFromViewport();
	RemoveTargetNPC();
	ChangeCharacterStateNormal();
}

void ARPGCharacter::StartInventoryTradeScreen()
{
	if (GetCharacterState() != ERPGCharacterState::Inventory) return;

	SelectedItem = nullptr;
	TargetNPC->SetNPCSelectedItem(nullptr);
	
	if (TradeMenu->IsInViewport()) TradeMenu->RemoveFromViewport();
	if (!InventoryTradeMenu->IsInViewport()) InventoryTradeMenu->AddToViewport();
	else
	{
		StopInventoryTradeScreen();
	}
}

void ARPGCharacter::StopInventoryTradeScreen()
{
	if (GetCharacterState() != ERPGCharacterState::Inventory) return;
	if (InventoryTradeMenu->IsInViewport()) InventoryTradeMenu->RemoveFromViewport();
	RemoveTargetNPC();
	ChangeCharacterStateNormal();
}

void ARPGCharacter::TradePickUpWeapon()
{
	ARPGWeapon* Temp = CurrentWeapon;
	CurrentWeapon = nullptr;
	WeaponPickUp(TargetNPC->GetNPCWeapon());
	TargetNPC->NPCSetWeaponNull();
	TargetNPC->NPCWeaponPickUp(Temp);
	Temp = nullptr;
}

void ARPGCharacter::TradePickUpThrowable()
{
	ARPGThrowable* Temp = ThrowableWeapon;
	ThrowableWeapon = nullptr;
	ThrowablePickUp(TargetNPC->GetNPCThrowable());
	TargetNPC->NPCSetThrowableNull();
	TargetNPC->NPCThrowablePickUp(Temp);
	Temp = nullptr;
}

void ARPGCharacter::TradePickUpHeadArmour()
{
	ARPGArmour* Temp = HeadArmour;
	HeadArmour = nullptr;
	ArmourPickUp(TargetNPC->GetNPCHeadArmour());
	TargetNPC->NPCSetHeadArmourNull();
	TargetNPC->NPCArmourPickUp(Temp);
	Temp = nullptr;
}

void ARPGCharacter::TradePickUpChestArmour()
{
	ARPGArmour* Temp = ChestArmour;
	ChestArmour = nullptr;
	ArmourPickUp(TargetNPC->GetNPCChestArmour());
	TargetNPC->NPCSetChestArmourNull();
	TargetNPC->NPCArmourPickUp(Temp);
	Temp = nullptr;
}


void ARPGCharacter::TradePickUpLegsArmour()
{
	ARPGArmour* Temp = LegsArmour;
	LegsArmour = nullptr;
	ArmourPickUp(TargetNPC->GetNPCLegsArmour());
	TargetNPC->NPCSetLegsArmourNull();
	TargetNPC->NPCArmourPickUp(Temp);
	Temp = nullptr;
}

void ARPGCharacter::TradePickUpItem(uint8 Pos)
{
	ARPGItem* Temp = SelectedItem;
	GetCharacterInventory()->SetItemInventory(nullptr, SelectedItemID);
	ItemPickUp(TargetNPC->GetNPCSelectedItem());
	TargetNPC->GetNPCInventory()->SetItemInventory(nullptr, TargetNPC->GetNPCSelectedItemID());
	TargetNPC->NPCItemPickUp(Temp);
	SelectedItem = nullptr;
	TargetNPC->SetNPCSelectedItem(nullptr);
	Temp = nullptr;
}

void ARPGCharacter::ReloadCurrentWeaponTimer()
{
	if (!CurrentWeapon) return;
	CurrentWeapon->ResetCurrentBullets();
}

void ARPGCharacter::CharacterReload()
{
	if (GetCharacterState() != ERPGCharacterState::Normal && GetCharacterState() != ERPGCharacterState::Combat) return;
	if (!CurrentWeapon) return;
	if (CurrentWeapon->GetWeaponType() == ERPGWeaponType::Melee) return;
	if (CurrentWeapon->GetWeaponCurrentBulletAmount() == CurrentWeapon->GetWeaponTotalBulletAmount()) return;
	ChangeCharacterStateReloading();
}

void ARPGCharacter::PauseGame()
{
	if (!PauseMenu->IsInViewport())
	{
		PauseMenu->AddToViewport();
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		return;
	}
	PauseMenu->RemoveFromViewport();
	if (GetCharacterState() != ERPGCharacterState::Inventory && GetCharacterState() != ERPGCharacterState::Rest) UGameplayStatics::SetGamePaused(GetWorld(), false);
}

void ARPGCharacter::CharacterSetBound(float Duration)
{
	CharacterBound = true;
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	FTimerHandle BoundHandle;
	GetWorldTimerManager().SetTimer(BoundHandle, this, &ARPGCharacter::SetCharacterBoundFalse, Duration, false);
}

void ARPGCharacter::SetCharacterBoundFalse()
{
	CharacterBound = false;
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}







void ARPGCharacter::TestFunction()
{
	// CharacterAttack(1);
	
	UE_LOG(LogTemp, Warning, TEXT("test"));

	// if (TargetNPC) TargetNPC->NPCTakeDamage(1, 15);

	// WeaponDrop();
	
	// if (TargetNPC && CurrentWeapon) CurrentWeapon->WeaponShoot(TargetNPC, 0);

	// CharacterAttack(1);

	// CharacterTakeDamage(4, 15);
	// UE_LOG(LogTemp, Warning, TEXT("larm: %f"), GetCharacterLArmCurrentHealth());
	// UE_LOG(LogTemp, Warning, TEXT("health: %f"), GetCharacterCurrentHealth());

	

	// if (TargetNPC)
	// {
	// 	PlayerFaceTargetNPC();
	// 	
	// 	TargetLineOfSight(6);
	// }

	// CharacterGetExp(10.f);

	// CharacterInventory->IncreaseInventorySize();

	// for (uint8 i = 0; i < CharacterInventory->GetInventorySize(); i++)
	// {
	// 	if (CharacterInventory->GetItemInventory()[i])
	// 	{
	// 		UE_LOG(LogTemp, Warning, TEXT("Item %d: %s"), i,*CharacterInventory->GetItemInventory()[i]->GetItemName().ToString());
	// 	}
	// 	else
	// 	{
	// 		UE_LOG(LogTemp, Warning, TEXT("Item %d: null"), i);
	// 	}
	// }

	// if (CharacterInventory->GetItemInventory()[0]) ItemDrop(0);

	// add repair item and make check if after destroy it gets replaced with nullptr

	// if (TargetNPC) UE_LOG(LogTemp, Warning, TEXT("DISTANCE: %f"), GetDistanceTo(TargetNPC));

	// CharacterTakeDamage(3, GetCharacterLArmMaxHealth());

	// if (TargetNPC) TargetNPC->NPCSetBound(5.f);

	// if (ThrowableWeapon && TargetNPC) ThrowableWeapon->ThrowableThrow(this, TargetNPC);

	// // FVector testloc = GetRootComponent()->GetComponentLocation() + GetRootComponent()->GetForwardVector() * 250;
	// FVector testloc = GetRootComponent()->GetComponentLocation() + GetRootComponent()->GetForwardVector() + FRotator(0.f, 90.f, 0.f).Vector()  * 250;
	// FVector testloc2 = GetRootComponent()->GetComponentLocation() + GetRootComponent()->GetForwardVector() + FRotator(0.f, 90.f + 180.f, 0.f).Vector()  * 250;
	//
	//
	// UKismetSystemLibrary::DrawDebugSphere(GetWorld(), testloc, 50, 12, FColor::White, 2.f);
	// UKismetSystemLibrary::DrawDebugSphere(GetWorld(), testloc2, 50, 12, FColor::White, 2.f);

	// float TrapMinDistance = 150.f;
	//
	// float TrapMaxDistance = 650.f;
	//
	// const float Trap0Angle = FMath::RandRange(0.f, 360.f);
	//
	// FVector Trap0Pos = GetActorLocation() + FRotator(0.f, Trap0Angle, 0.f).Vector() * FMath::RandRange(TrapMaxDistance - TrapMinDistance, TrapMaxDistance);
	//
	// const float Trap1Value = Trap0Angle + 180.f;
	//
	// const float Trap1Angle = FMath::RandRange(Trap1Value - 90.f, Trap1Value + 90.f);
	//
	// FVector Trap1Pos = GetActorLocation() + FRotator(0.f, Trap1Angle, 0.f).Vector() * FMath::RandRange(TrapMinDistance, TrapMaxDistance - TrapMinDistance);
	//
	// UKismetSystemLibrary::DrawDebugSphere(GetWorld(), Trap0Pos, 50, 12, FColor::White, 2.f);
	// UKismetSystemLibrary::DrawDebugSphere(GetWorld(), Trap1Pos, 50, 12, FColor::White, 2.f);
}
