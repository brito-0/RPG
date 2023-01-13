// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGThrowable.h"
#include "RPGThrowableSlow.generated.h"


UCLASS()
class RPG_API ARPGThrowableSlow : public ARPGThrowable
{
	GENERATED_BODY()
			
public:
	ARPGThrowableSlow();

	virtual void Tick(float DeltaTime) override;
	
	virtual void ThrowableThrow(ACharacter* Target, ACharacter* Attacker) override;

	float GetSlowDuration() { return SlowDuration; }
	
protected:
	void BeginPlay() override;

private:
	float SlowDuration = 5.f;
	
};
