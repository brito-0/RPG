// Copyright Epic Games, Inc. All Rights Reserved.


#include "RPGGameModeBase.h"

#include "RPGTrap.h"
#include "Kismet/GameplayStatics.h"

ARPGGameModeBase::ARPGGameModeBase()
{
	PrimaryActorTick.bCanEverTick =	true;
}

void ARPGGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(true);

	// GameTraps.Init(nullptr, MaxGameTraps);
}

void ARPGGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}


void ARPGGameModeBase::AddGameTrap(ARPGTrap* Trap)
{
	if (GameTraps.Num() < MaxGameTraps) GameTraps.Add(Trap);
	else
	{
		GameTraps[0]->DestroyItem();
		GameTraps.Add(Trap);
	}
}

void ARPGGameModeBase::RemoveGameTrap(ARPGTrap* Trap)
{
	GameTraps.Remove(Trap);
	// GameTraps.Shrink();
}
