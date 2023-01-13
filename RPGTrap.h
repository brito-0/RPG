// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGItem.h"
#include "RPGNPCTrapper.h"
#include "RPGTrap.generated.h"

class ADecalActor;
class ARPGGameModeBase;

UENUM()
enum class ERPGTrapType : uint8
{
	Bind,
	Explosive,
	Incendiary,
	Slow
};

UENUM()
enum class ERPGTrapState : uint8
{
	Activated,
	Deactivated
};

UCLASS()
class RPG_API ARPGTrap : public ARPGItem
{
	GENERATED_BODY()

public:
	ARPGTrap();

	virtual void Tick(float DeltaSeconds) override;

	virtual void UseItem() override;

	void NPCUseItem(ARPGNPCTrapper* Trapper, FVector Position);

	virtual void ItemUsed() override;

	float GetTrapDamage() const { return Damage; }
	
	float GetTrapRange() const { return Range; }

	float GetTrapEffectRange() const { return EffectRange; }

	float GetTrapEffectDuration() const { return EffectDuration; }
	
	float GetTrapDuration() const { return Duration; }

	float GetTrapDelay() const { return Delay; }

	bool GetTrapActivated() const { return Activated; }

	FVector GetTrapPos() const { return TrapPos; }

	virtual void DestroyItem() override;

	void DetonateTrap();

	ERPGTrapState GetTrapState() { return CurrentTrapState; }

protected:
	virtual void BeginPlay() override;

	void SetTrapDamage(float Value) { Damage = Value; }

	void SetTrapRange(float Value) { Range = Value; }

	void SetTrapEffectRange(float Value) { EffectRange = Value; }

	void SetTrapEffectDuration(float Value) { EffectDuration = Value; }

	void SetTrapDuration(float Value) { Duration = Value; }

	void SetTrapDelay(float Value) { Delay = Value; }

	void SetTrapType(ERPGTrapType Type) { TrapType = Type; }
	

	virtual void TrapEffect(TArray<AActor*> Targets);

	void DestroyDecal();


	void SetTrapState(ERPGTrapState State);

	void ChangeTrapStateActivated() { SetTrapState(ERPGTrapState::Activated); }
	void ChangeTrapStateDeactivated() { SetTrapState(ERPGTrapState::Deactivated); }
	
private:
	UPROPERTY(VisibleAnywhere)
	float Damage = 0.f;

	UPROPERTY(VisibleAnywhere)
	float Range = 75.f;

	UPROPERTY(VisibleAnywhere)
	float EffectRange = 150.f;

	UPROPERTY(VisibleAnywhere)
	float EffectDuration = 0.f;

	UPROPERTY(VisibleAnywhere)
	float Duration = 62.f;

	UPROPERTY(VisibleAnywhere)
	float Delay = 2.f;

	UPROPERTY(VisibleAnywhere)
	bool Activated = false;

	void SetActivatedTrue() { Activated = true; }

	FVector TrapPos;

	void TrapCheckRange();

	void ActivateTrap();

	UPROPERTY(VisibleAnywhere)
	ERPGTrapType TrapType;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ADecalActor> TrapDecalObject;
	UPROPERTY(EditAnywhere)
	TSubclassOf<ADecalActor> NPCTrapDecalObject;
	UPROPERTY()
	ADecalActor* TrapDecal;

	FTimerHandle DestroyTimerHandle;

	
	// inline static TArray<ARPGTrap*> GameTraps;// = {nullptr, nullptr, nullptr};
	// inline static  uint8 GameTrapsSize = 0;
	//
	// UPROPERTY(VisibleAnywhere)
	// uint8 GameTrapsMaxSize = 3;


	// UPROPERTY(VisibleAnywhere)
	// uint8 CurrentGameTrapsSize = 0;

	UPROPERTY()
	ARPGGameModeBase* GameMode;

	UPROPERTY(VisibleAnywhere)
	ERPGTrapState CurrentTrapState = ERPGTrapState::Deactivated;

	UPROPERTY(VisibleAnywhere)
	ARPGCharacter* PlayerChar;

	UPROPERTY(VisibleAnywhere)
	ARPGNPCTrapper* NPCTrapper = nullptr;
	
};
