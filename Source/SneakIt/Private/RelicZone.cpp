// Fill out your copyright notice in the Description page of Project Settings.

#include "RelicZone.h"
#include "Thief.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SpotLightComponent.h"

// Sets default values
ARelicZone::ARelicZone()
{
	bIsRelicStolen = false;

	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapCheckArea"));
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapComp->SetBoxExtent(FVector(140.0f));

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RelicMesh"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(GetRootComponent());

	SpotlightComp = CreateDefaultSubobject<USpotLightComponent>(TEXT("RelicSpotLight"));
	SpotlightComp->SetupAttachment(GetRootComponent());

	RootComponent = OverlapComp;

	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &ARelicZone::HandleBeginOverlap);
	OverlapComp->OnComponentEndOverlap.AddDynamic(this, &ARelicZone::HandleEndOverlap);
}

void ARelicZone::BeginPlay()
{
	Super::BeginPlay();
}

void ARelicZone::HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AThief* MyPawn = Cast<AThief>(OtherActor);
	if (MyPawn == nullptr)
		return;

	if (!bIsRelicStolen)
	{
		MyPawn->BeginStealingRelic(MeshComp);
	}
}

void ARelicZone::HandleEndOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp,
	int32 OtherBodyIndex)
{
	AThief* MyPawn = Cast<AThief>(OtherActor);
	if (MyPawn == nullptr)
	return;

	if (!bIsRelicStolen && !MeshComp->IsPendingKill())
	{
		MyPawn->EndStealingRelic();
	}
	else
	{
		bIsRelicStolen = true;
	}
}