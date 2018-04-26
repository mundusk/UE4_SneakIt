#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIGuard.generated.h"

class UBlackboardComponent;
class UPawnSensingComponent;
class USphereComponent;
class USpotLightComponent;

UCLASS()
class SNEAKIT_API AAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAIGuard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnThiefSeen(APawn* SeenPawn);

	UFUNCTION()
	void OnThiefHeard(APawn* NoiseInstigator, const FVector& Location, float Volume);

	UFUNCTION()
	void OnThiefNoLongerSensed();

	UFUNCTION()
	void SearchForPatrolPoint();

	UFUNCTION()
	void FindNextPatrolPoint();

	UPROPERTY()
	UBlackboardComponent* AIGuardBlackboard;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpotLightComponent* SpotLightComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* OverlapComp;

	UPROPERTY(EditInstanceOnly, Category = "AI")
	AActor* FirstPatrolPoint;

	UPROPERTY(EditInstanceOnly, Category = "AI")
	AActor* SecondPatrolPoint;

	UPROPERTY(EditInstanceOnly, Category = "AI")
	AActor* ThirdPatrolPoint;

	UPROPERTY(EditInstanceOnly, Category = "AI")
	AActor* CurrentPatrolPoint;

	FTimerHandle TimerHandle_ResetGuardSenses;
	FTimerHandle TimerHandle_FindNextPatrolPoint;
	FVector PatrolPointDestination;

	UPROPERTY(VisibleAnywhere, Category="Gameplay")
	bool bThiefIsSeen;

	UPROPERTY(VisibleAnywhere, Category = "Gameplay")
	bool bIsPatrolling;

	UPROPERTY(VisibleAnywhere, Category = "Gameplay")
	bool bIsSearchingForNextPatrolPoint;

	UPROPERTY(VisibleAnywhere, Category = "Gameplay")
	int CurrentPatrolPointNumber;

	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult & SweepResult);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
