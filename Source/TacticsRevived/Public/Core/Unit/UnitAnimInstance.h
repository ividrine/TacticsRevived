// UnitAnimInstance.h

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TacticsRevived/Enums/TacticsEnums.h"
#include "UnitAnimInstance.generated.h"

class AUnit;

UCLASS()
class TACTICSREVIVED_API UUnitAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:

    virtual void NativeInitializeAnimation() override;
    virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, Category = "Unit")
	AUnit* OwnerUnit = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bIsMovingPath = false;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bIsMovingWay = false;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bIsMovingBack = false;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bHasMovedWay = false;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bHasMovedBack = false;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	EUnitMovementAnimType MovementType = EUnitMovementAnimType::Idle;

    
};