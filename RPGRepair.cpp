// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGRepair.h"
#include "Blueprint/UserWidgetBlueprint.h"
#include "Blueprint/UserWidget.h"


ARPGRepair::ARPGRepair()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SetItemName("Repair Kit");
	SetItemType(ERPGItemType::Repair);

	// static ConstructorHelpers::FObjectFinder<UUserWidgetBlueprint>RepairMenuAsset(TEXT("WidgetBlueprint'/Game/Blueprints/HUD/RPGRepairMenu_BP.RPGRepairMenu_BP'"));
	// if (RepairMenuAsset.Succeeded()) { RepairMenuWidget = RepairMenuAsset.Object->GeneratedClass; }

	this->Tags.Add("Repair");
}

void ARPGRepair::BeginPlay()
{
	Super::BeginPlay();
	
	if (RepairMenuWidget) RepairMenu = CreateWidget<UUserWidget>(GetWorld(),  RepairMenuWidget);
}



void ARPGRepair::UseItem()
{
	// repair selected armour
	// delete item

	StartScreen();
}

void ARPGRepair::StartScreen()
{
	if (!RepairMenu->IsInViewport()) RepairMenu->AddToViewport();
}

void ARPGRepair::StopScreen()
{
	if (RepairMenu->IsInViewport()) RepairMenu->RemoveFromViewport();
}

void ARPGRepair::ItemUsed()
{
	StopScreen();

	DestroyItem();
}
