// Fill out your copyright notice in the Description page of Project Settings.
#include "Thief.h"

#include "SneakItGameModeBase.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Controller.h"

AThief::AThief()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("ThiefCamera"));
	CameraComp->SetupAttachment(GetRootComponent());
	CameraComp->RelativeLocation = FVector(0, 0, BaseEyeHeight);
	CameraComp->bUsePawnControlRotation = true;

	bIsStealingRelic = false;
	bIsStealComplete = false;
	TotalTimeForSteal = 0.0f;
	RelicStealSpeed = 0.2f;
}

void AThief::BeginPlay()
{
	Super::BeginPlay();
}

void AThief::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsStealingRelic && !bIsStealComplete)
	{
		TimerToStealRelic(DeltaTime);
	}
	else if (bIsStealingRelic && bIsStealComplete)
	{
		RelicStealCompleted();
	}

	FRotator ThiefRotation = GetActorRotation();
	ThiefRotation.Pitch = 0.0f;
	ThiefRotation.Roll = 0.0f;
	SetActorRotation(ThiefRotation);
}

void AThief::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AThief::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AThief::MoveRight);
}

void AThief::MoveForward(float Value)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);

	MakeNoise(1.0f, Instigator);
}

void AThief::MoveRight(float Value)
{
	FRotator ControlRotation = GetControlRotation();

	if (Value > 0.5f)
	{
		ControlRotation.Yaw += 2.5f;
	}
	else if (Value < -0.5f)
	{
		ControlRotation.Yaw -= 2.5f;
	}
	
	Controller->SetControlRotation(ControlRotation);
}

void AThief::BeginStealingRelic(USkeletalMeshComponent* relic)
{
	bIsStealingRelic = true;
	RelicBeingStolen = relic;

	OnEnterRelicZone();
}

void AThief::EndStealingRelic()
{
	bIsStealingRelic = false;
	TotalTimeForSteal = 0.0f;

	OnExitRelicZone();
}

void AThief::RelicStealCompleted()
{
	bIsStealingRelic = false;
	bIsStealComplete = false;
	TotalTimeForSteal = 0.0f;
	RelicBeingStolen->DestroyComponent();

	OnRelicStealCompleted();

	ASneakItGameModeBase* GM = Cast<ASneakItGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->RelicSuccessfullyStolen(this, true);
	}
}

void AThief::TimerToStealRelic(float DeltaTime)
{
	TotalTimeForSteal += RelicStealSpeed * DeltaTime;

	if (TotalTimeForSteal >= 1.0f)
	{
		bIsStealComplete = true;
	}
}