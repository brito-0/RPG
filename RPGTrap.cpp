// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGTrap.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "RPGCharacter.h"
#include "Engine/DecalActor.h"
#include "RPGGameModeBase.h"
#include "RPGNPCTrapper.h"

ARPGTrap::ARPGTrap()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SetItemName("Trap");
	SetItemType(ERPGItemType::Trap);

	SetActorTickInterval(.5f);
	
	this->Tags.Add("Trap");
}

void ARPGTrap::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<ARPGGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	PlayerChar = Cast<ARPGCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	ChangeTrapStateActivated();
	
	// GameTraps.Empty();
}

void ARPGTrap::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// if (GetTrapState() == ERPGTrapState::Deactivated && GetDistanceTo(PlayerChar) <= 4050.f) ChangeTrapStateActivated();
	// if (GetTrapState() == ERPGTrapState::Activated)
	// {
	// 	if (GetDistanceTo(PlayerChar) > 4050.f) ChangeTrapStateDeactivated();
	// }
	//
	// if (Activated)
	// {
	// 	TrapCheckRange();
	// }

	if (Activated)
	{
		if (GetTrapState() == ERPGTrapState::Deactivated && GetDistanceTo(PlayerChar) <= 4050.f) ChangeTrapStateActivated();
		if (GetTrapState() == ERPGTrapState::Activated)
		{
			if (GetDistanceTo(PlayerChar) > 4050.f) ChangeTrapStateDeactivated();
		}

		TrapCheckRange();
	}

	// CurrentGameTrapsSize = GameTraps.Num();
}

void ARPGTrap::UseItem()
{
	TrapPos = PlayerChar->GetActorLocation();

	PlayerChar->DestroySelectedItem();

	GameMode->AddGameTrap(this);

	if (TrapDecalObject) TrapDecal = GetWorld()->SpawnActor<ADecalActor>(TrapDecalObject,
		FVector(TrapPos.X, TrapPos.Y, TrapPos.Z - 93.f), FRotator(0.f, 0.f, 0.f));

	FTimerHandle ActivatedHandle;
	GetWorldTimerManager().SetTimer(ActivatedHandle, this, &ARPGTrap::SetActivatedTrue, GetTrapDelay(), false);
}

void ARPGTrap::NPCUseItem(ARPGNPCTrapper* Trapper, FVector Position)
{
	NPCTrapper = Trapper;

	TrapPos = Position;

	if (NPCTrapDecalObject) TrapDecal = GetWorld()->SpawnActor<ADecalActor>(TrapDecalObject,
		FVector(TrapPos.X, TrapPos.Y, TrapPos.Z - 93.f), FRotator(0.f, 0.f, 0.f));

	this->SetActorEnableCollision(false);
	this->SetActorHiddenInGame(true);
	
	SetActivatedTrue();

	//
	//
	//
	// check position
	
}


void ARPGTrap::ItemUsed()
{	
	// GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &ARPGTrap::DestroyItem, GetTrapDuration(), false);

	this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	this->SetActorEnableCollision(false);
	
	SetActorLocation(GetTrapPos());
	
}

void ARPGTrap::TrapEffect(TArray<AActor*> Targets)
{
	//
}

void ARPGTrap::TrapCheckRange()
{
	if (GetTrapState() != ERPGTrapState::Activated) return;
	
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType((ECollisionChannel::ECC_WorldDynamic)));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType((ECollisionChannel::ECC_PhysicsBody)));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType((ECollisionChannel::ECC_Pawn)));
	const TArray<AActor*> IgnoreActor = {this};
	// const TArray<AActor*> IgnoreActor = {this, ARPGNPCTrapper};
	TArray<AActor*> OutActors;
		
	UKismetSystemLibrary::DrawDebugSphere(GetWorld(), TrapPos, GetTrapRange());
			
	if (UKismetSystemLibrary::SphereOverlapActors(GetWorld(), TrapPos, GetTrapRange(), ObjectTypes, ACharacter::StaticClass(), IgnoreActor, OutActors))
	{
		if (OutActors.Num() == 1)
		{
			if (OutActors[0]->ActorHasTag("Trapper")) return;
		}

		// alert npc
		if (NPCTrapper) NPCTrapper->TrapDetonationAlert(OutActors[0]);
		
		Activated = false;
		ActivateTrap();
	}
}

void ARPGTrap::ActivateTrap()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType((ECollisionChannel::ECC_WorldDynamic)));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType((ECollisionChannel::ECC_PhysicsBody)));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType((ECollisionChannel::ECC_Pawn)));
	const TArray<AActor*> IgnoreActor = {this};
	TArray<AActor*> OutActors;
		
	UKismetSystemLibrary::DrawDebugSphere(GetWorld(), TrapPos, GetTrapEffectRange());
			
	if (UKismetSystemLibrary::SphereOverlapActors(GetWorld(), TrapPos, GetTrapEffectRange(), ObjectTypes, ACharacter::StaticClass(), IgnoreActor, OutActors))
	{
		Activated = false;
		TrapEffect(OutActors);
	}
}

void ARPGTrap::DetonateTrap()
{
	ActivateTrap();
}

void ARPGTrap::DestroyDecal()
{
	if (TrapDecal) TrapDecal->Destroy();
}


void ARPGTrap::DestroyItem()
{
	GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
	if (TrapDecal) TrapDecal->Destroy();

	GameMode->RemoveGameTrap(this);
	
	this->Destroy();
}


void ARPGTrap::SetTrapState(ERPGTrapState State)
{
	if (GetTrapState() == State) return;
	
	switch (State)
	{
	case ERPGTrapState::Activated:
		{
			CurrentTrapState = ERPGTrapState::Activated;
			
			SetActorTickInterval(.5f);

			break;
		}
	case ERPGTrapState::Deactivated:
		{
			CurrentTrapState = ERPGTrapState::Deactivated;

			SetActorTickInterval(2.5f);
			
			break;
		}
	}
}
