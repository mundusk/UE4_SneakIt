#include "AIGuard.h"
#include "DrawDebugHelpers.h"
#include "SneakItGameModeBase.h"
#include "TimerManager.h"
#include "Thief.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/SpotLightComponent.h"
#include "Perception/PawnSensingComponent.h"

AAIGuard::AAIGuard()
{
	PrimaryActorTick.bCanEverTick = true;
	bThiefIsSeen = false;
	bIsPatrolling = false;
	bIsSearchingForNextPatrolPoint = false;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	SpotLightComp = CreateDefaultSubobject<USpotLightComponent>(TEXT("Flashlight"));
	SpotLightComp->SetupAttachment(GetRootComponent());

	OverlapComp = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapPlayerComp"));
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapComp->SetSphereRadius(150.0f);

	PawnSensingComp->OnSeePawn.AddDynamic(this, &AAIGuard::OnThiefSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AAIGuard::OnThiefHeard);
	
	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AAIGuard::HandleBeginOverlap);
}

void AAIGuard::BeginPlay()
{
	Super::BeginPlay();

	AIGuardBlackboard = UAIBlueprintHelperLibrary::GetBlackboard(this);

	if (AIGuardBlackboard != nullptr)
	{
		AIGuardBlackboard->SetValueAsBool("ThiefIsSeen", bThiefIsSeen);
	}

	SearchForPatrolPoint();
}

void AAIGuard::HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AThief* MyPawn = Cast<AThief>(OtherActor);
	if (MyPawn == nullptr)
		return;

	ASneakItGameModeBase* GM = Cast<ASneakItGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->CompletedStealingRelics(MyPawn, false);
	}
}

void AAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsPatrolling)
	{
		FVector Delta = GetActorLocation() - CurrentPatrolPoint->GetActorLocation();
		float DistanceToTargetDestination = Delta.Size();

		if (DistanceToTargetDestination < 50 && !bIsSearchingForNextPatrolPoint)
		{
			GetWorldTimerManager().ClearTimer(TimerHandle_FindNextPatrolPoint);
			GetWorldTimerManager().SetTimer(TimerHandle_FindNextPatrolPoint, this, &AAIGuard::FindNextPatrolPoint, 3.5f);

			bIsSearchingForNextPatrolPoint = true;
		}
	}
}

void AAIGuard::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AAIGuard::OnThiefSeen(APawn* SeenPawn)
{
	if (SeenPawn == nullptr)
	{
		return;
	}

	//Uncomment to test that the AIGuard is using sight sense to keep track of the Thief
	//DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.0f, 12, FColor::Red, false, 10.0f);

	bThiefIsSeen = true;
	bIsPatrolling = false;

	AIGuardBlackboard->SetValueAsBool("ThiefIsSeen", bThiefIsSeen);
	AIGuardBlackboard->SetValueAsVector("TargetDestination", SeenPawn->GetActorLocation());

	GetWorldTimerManager().ClearTimer(TimerHandle_ResetGuardSenses);
	GetWorldTimerManager().SetTimer(TimerHandle_ResetGuardSenses, this, &AAIGuard::OnThiefNoLongerSensed, 5.0f);
}

void AAIGuard::OnThiefHeard(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Green, false, 10.0f);
}

void AAIGuard::OnThiefNoLongerSensed()
{
	bThiefIsSeen = false;

	AIGuardBlackboard->SetValueAsBool("ThiefIsSeen", bThiefIsSeen);

	SearchForPatrolPoint();
}

void AAIGuard::SearchForPatrolPoint()
{
	//This type of AI Guard rotates between 3 patrol points
	int GeneratedPatrolPoint = FMath::RandRange(1, 3);

	switch (GeneratedPatrolPoint)
	{
	case 1:
		PatrolPointDestination = FirstPatrolPoint->GetActorLocation();
		CurrentPatrolPoint = FirstPatrolPoint;
		break;
	case 2:
		PatrolPointDestination = SecondPatrolPoint->GetActorLocation();
		CurrentPatrolPoint = SecondPatrolPoint;
		break;
	case 3:
		PatrolPointDestination = ThirdPatrolPoint->GetActorLocation();
		CurrentPatrolPoint = ThirdPatrolPoint;
		break;
	default:
		break;
	}

	AIGuardBlackboard->SetValueAsVector("TargetDestination", PatrolPointDestination);
	
	CurrentPatrolPointNumber = GeneratedPatrolPoint;
	bIsPatrolling = true;
}

void AAIGuard::FindNextPatrolPoint()
{
	int GeneratedPatrolPoint = 1;

	do
	{
		GeneratedPatrolPoint = FMath::RandRange(1, 3);
	}
	while (GeneratedPatrolPoint == CurrentPatrolPointNumber);

	switch (GeneratedPatrolPoint)
	{
	case 1:
		PatrolPointDestination = FirstPatrolPoint->GetActorLocation();
		CurrentPatrolPoint = FirstPatrolPoint;
		break;
	case 2:
		PatrolPointDestination = SecondPatrolPoint->GetActorLocation();
		CurrentPatrolPoint = SecondPatrolPoint;
		break;
	case 3:
		PatrolPointDestination = ThirdPatrolPoint->GetActorLocation();
		CurrentPatrolPoint = ThirdPatrolPoint;
		break;
	default:
		break;
	}

	AIGuardBlackboard->SetValueAsVector("TargetDestination", PatrolPointDestination);

	CurrentPatrolPointNumber = GeneratedPatrolPoint;

	bIsSearchingForNextPatrolPoint = false;
}