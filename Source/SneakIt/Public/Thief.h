// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Thief.generated.h"

class UCameraComponent;
//class USpotLightComponent;

UCLASS()
class SNEAKIT_API AThief : public ACharacter
{
	GENERATED_BODY()

private:
	UFUNCTION()
	void MoveForward(float Value);

	UFUNCTION()
	void MoveRight(float Value);

	//UFUNCTION()
	//void SwitchFlashlight();

	UFUNCTION()
	void RelicStealCompleted();

	UFUNCTION()
	void TimerToStealRelic(float DeltaTime);
	
	UPROPERTY()
	USkeletalMeshComponent* RelicBeingStolen;

public:
	AThief();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;

	//UPROPERTY(VisibleAnywhere, Category="Components")
	//USpotLightComponent* SpotlightComp;

	//A variable associated with the progress bar to check time left before steal is complete. Goes up to 1.0
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay Properties")
	float TotalTimeForSteal;

	//Amount the TotalTimeForSteal gets incremented by when stealing a relic
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Properties")
	float RelicStealSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay Properties")
	bool bIsFlashlightOn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay Properties")
	bool bIsStealingRelic;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay Properties")
	bool bIsStealComplete;

	UFUNCTION(BlueprintImplementableEvent, Category = "Gameplay Events")
	void OnEnterRelicZone();

	UFUNCTION(BlueprintImplementableEvent, Category = "Gameplay Events")
	void OnExitRelicZone();

	UFUNCTION(BlueprintImplementableEvent, Category = "Gameplay Events")
	void OnRelicStealCompleted();

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void BeginStealingRelic(USkeletalMeshComponent* relic);

	UFUNCTION()
	void EndStealingRelic();
};
