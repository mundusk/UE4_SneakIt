// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DebugRelic.generated.h"

class UBoxComponent;
class USpotLightComponent;

UCLASS()
class SNEAKIT_API ADebugRelic : public AActor
{
	GENERATED_BODY()
	
private:
	UFUNCTION()
		void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult & SweepResult);

	//UFUNCTION()
	//void HandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	ADebugRelic();

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USkeletalMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* OverlapComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpotLightComponent* SpotlightComp;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* ObjectiveMissingSound;

	UPROPERTY(BlueprintReadOnly, Category = "Gameplay Properties")
	bool bIsRelicStolen;

protected:
	virtual void BeginPlay() override;
};
