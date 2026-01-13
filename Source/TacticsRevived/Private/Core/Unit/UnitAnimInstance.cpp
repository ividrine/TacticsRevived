// UnitAnimInstance.cpp

#include "Core/Unit/UnitAnimInstance.h"
#include "TacticsRevived/Macros/DebugMacros.h"
#include "Core/Unit/Unit.h"
#include "Animation/AnimNode_StateMachine.h"

void UUnitAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	OwnerUnit = Cast<AUnit>(GetOwningActor());
}

void UUnitAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (OwnerUnit)
	{
		bIsMovingPath = OwnerUnit->IsMovingPath();
		bIsMovingWay = OwnerUnit->IsMovingWay();
		bIsMovingBack = OwnerUnit->IsMovingBack();
		bHasMovedWay = OwnerUnit->bHasMovedWay;
		bHasMovedBack = OwnerUnit->bHasMovedBack;
		MovementType = OwnerUnit->MovementType;
	}

}
