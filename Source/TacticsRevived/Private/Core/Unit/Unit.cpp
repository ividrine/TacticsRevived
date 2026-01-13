#include "Core/Unit/Unit.h"
#include "Net/UnrealNetwork.h"
#include "TacticsRevived/Macros/DebugMacros.h"
#include "RenderingThread.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Core/Unit/UnitMovementComponent.h"
#include "Core/Unit/UnitAbilitySystemComponent.h"
#include "Core/Unit/UnitAttributeSet.h"
#include "Core/Board.h"
#include "Core/Tile.h"

AUnit::AUnit()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(false);

	NetUpdateFrequency = 66.f;
	MinNetUpdateFrequency = 33.f;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComponent;

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(CapsuleComponent);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->SetupAttachment(RootComponent);

	MovementComponent = CreateDefaultSubobject<UUnitMovementComponent>(TEXT("MovementComponent"));

	AbilitySystemComponent = CreateDefaultSubobject<UUnitAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UUnitAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void AUnit::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUnit, Board);
	DOREPLIFETIME(AUnit, Tile);
	DOREPLIFETIME(AUnit, PlayerId);
	DOREPLIFETIME(AUnit, MovementState);
	DOREPLIFETIME(AUnit, MovementType);
	DOREPLIFETIME(AUnit, bHasMovedWay);
	DOREPLIFETIME(AUnit, bHasMovedBack);
	DOREPLIFETIME(AUnit, ServerLocation);
	DOREPLIFETIME(AUnit, ServerRotation);
}

UAbilitySystemComponent* AUnit::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}


void AUnit::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void AUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUnit::Highlight()
{
	MeshComponent->SetRenderCustomDepth(true);
	MeshComponent->SetCustomDepthStencilValue(250);
}

void AUnit::UnHighlight()
{
	MeshComponent->SetRenderCustomDepth(false);
	MeshComponent->SetCustomDepthStencilValue(0);
}

void AUnit::ServerWalkPath_Implementation(ATile* Target)
{
	if (!Board || !Target) return;
	TArray<ATile*> TilePath = Board->GetPath(this, Target);
	GetMovementComponent()->MovePath(TilePath);
}

bool AUnit::ServerWalkPath_Validate(ATile* Target)
{
	// TODO: Make sure unit is actually allowed to move to this target
	return true;
}

float AUnit::GetBoardRotation()
{
	switch (Direction)
	{
		case ETacticsDirection::N: return 0; break;
		case ETacticsDirection::S: return 180; break;
		case ETacticsDirection::E: return 90; break;
		case ETacticsDirection::W: return -90; break;
		default: return 0;
	}
}

TArray<ATile*> AUnit::GetMovementTiles()
{
	if (!Board) return {};
	return Board->GetUnitPathRange(this);
}

