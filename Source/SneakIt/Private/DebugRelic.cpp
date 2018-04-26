// Fill out your copyright notice in the Description page of Project Settings.

#include "DebugRelic.h"
#include "Thief.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SpotLightComponent.h"

// Sets default values
ADebugRelic::ADebugRelic()
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

	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &ADebugRelic::HandleBeginOverlap);
	//OverlapComp->OnComponentEndOverlap.AddDynamic(this, &ARelicZone::HandleEndOverlap);
}

// Called when the game starts or when spawned
void ADebugRelic::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADebugRelic::HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
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

