// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGHeal.h"
#include "Blueprint/UserWidgetBlueprint.h"
#include "Blueprint/UserWidget.h"

ARPGHeal::ARPGHeal()
{
	PrimaryActorTick.bCanEverTick = false;

	SetItemName("Heal Kit");
	SetItemType(ERPGItemType::Heal);

	// static ConstructorHelpers::FObjectFinder<UUserWidgetBlueprint>HealMenuAsset(TEXT("WidgetBlueprint'/Game/Blueprints/HUD/RPGHealMenu_BP.RPGHealMenu_BP'"));
	// if (HealMenuAsset.Succeeded()) { HealMenuWidget = HealMenuAsset.Object->GeneratedClass; }

	this->Tags.Add("Heal");
}

void ARPGHeal::BeginPlay()
{
	Super::BeginPlay();

	if (HealMenuWidget) HealMenu = CreateWidget<UUserWidget>(GetWorld(),  HealMenuWidget);
}


void ARPGHeal::UseItem()
{
	// do nothing
	// or bring up normal menu - instant heal when not in combat???

	StartScreen();
}

void ARPGHeal::StartScreen()
{
	if (!HealMenu->IsInViewport()) HealMenu->AddToViewport();
}

void ARPGHeal::StopScreen()
{
	if (HealMenu->IsInViewport()) HealMenu->RemoveFromViewport();
}

void ARPGHeal::ItemUsed()
{
	StopScreen();

	DestroyItem();
}
