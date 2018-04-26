// Fill out your copyright notice in the Description page of Project Settings.
#include "SneakItGameModeBase.h"

#include "EngineUtils.h"
#include "RelicZone.h"
#include "Kismet/GameplayStatics.h"

void ASneakItGameModeBase::StartPlay()
{
	//TODO: Research why GetWorld() no longer works with GetAllActorsOfClass
	//TArray<AActor*> Relics;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARelicZone::StaticClass(), Relics);
	for (TActorIterator<ARelicZone> RelicItr(GetWorld()); RelicItr; ++RelicItr)
	{
		TotalRelics += 1;
	}

	bIsGameOver = false;

	Super::StartPlay();
}

void ASneakItGameModeBase::RelicSuccessfullyStolen(APawn* InstigatorPawn, bool bRelicsStealSuccess)
{
	RelicsStolen += 1;

	OnRelicSuccessfullyStolen(RelicsStolen);

	if (RelicsStolen == TotalRelics)
	{
		CompletedStealingRelics(InstigatorPawn, bRelicsStealSuccess);
	}
}

void ASneakItGameModeBase::CompletedStealingRelics(APawn* InstigatorPawn, bool bRelicsStealSuccess)
{
	if (!bIsGameOver)
	{
		if (InstigatorPawn)
		{
			InstigatorPawn->DisableInput(nullptr);
		}

		OnCompletedStealingRelics(bRelicsStealSuccess);

		bIsGameOver = true;
	}
}