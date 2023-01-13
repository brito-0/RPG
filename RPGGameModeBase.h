// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RPGGameModeBase.generated.h"

class ARPGTrap;

UCLASS()
class RPG_API ARPGGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARPGGameModeBase();
	virtual void Tick(float DeltaSeconds) override;


	void AddGameTrap(ARPGTrap* Trap);

	void RemoveGameTrap(ARPGTrap* Trap);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	TArray<ARPGTrap*> GameTraps;

	UPROPERTY(EditAnywhere)
	uint8 MaxGameTraps = 3;
	
};
