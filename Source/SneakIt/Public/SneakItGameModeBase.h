// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SneakItGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKIT_API ASneakItGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	virtual void StartPlay() override;

private:
	bool bIsGameOver;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	int TotalRelics;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	int RelicsStolen;

public:
	void RelicSuccessfullyStolen(APawn* InstigatorPawn, bool bRelicsStealSuccess);

	void CompletedStealingRelics(APawn* InstigatorPawn, bool bRelicsStealSuccess);

	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void OnCompletedStealingRelics(bool bRelicsStealSuccess);

	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void OnRelicSuccessfullyStolen(int NumberOfStolenRelics);
};
