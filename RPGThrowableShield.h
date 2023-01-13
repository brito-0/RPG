// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGThrowable.h"
#include "RPGThrowableShield.generated.h"

class ARPGNPCShield;

UCLASS()
class RPG_API ARPGThrowableShield : public ARPGThrowable
{
	GENERATED_BODY()

public:
	ARPGThrowableShield();
	
	void ThrowableThrowShield(FVector Position, FVector Direction);
	
protected:
	void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = Shield)
	TSubclassOf<ARPGNPCShield> NPCShieldObject = nullptr;
	UPROPERTY(VisibleAnywhere, Category = Shield)
	ARPGNPCShield* NPCShield = nullptr;
	
};
