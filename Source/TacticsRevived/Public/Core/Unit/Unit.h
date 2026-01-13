#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "TacticsRevived/Enums/TacticsEnums.h"
#include "Unit.generated.h"

class ABoard;
class ATile;
class UCapsuleComponent;
class USkeletalMeshCompoonent;
class UArrowComponent;
class UWidgetComponent;
class UUnitMovementComponent;
class UAbilitySystemComponent;
class UAttributeSet;
class UUnitAnimDataAsset;

UCLASS()
class TACTICSREVIVED_API AUnit : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	AUnit();

	// Overrides

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Tick(float DeltaTime) override;
	
	// Component Getters

	UUnitMovementComponent* GetMovementComponent() const { return MovementComponent; }
	UCapsuleComponent* GetCapsuleComponent() const { return CapsuleComponent; }
	USkeletalMeshComponent* GetMeshComponent() const { return MeshComponent; }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	// Movement State

	UFUNCTION(BlueprintCallable)
	bool IsMovingPath() const { return MovementState == EUnitMovementState::MovingPath; }

	UFUNCTION(BlueprintCallable)
	bool IsMovingWay() const { return MovementState == EUnitMovementState::MovingWay; }

	UFUNCTION(BlueprintCallable)
	bool IsMovingBack() const { return MovementState == EUnitMovementState::MovingBack; }

	UFUNCTION(BlueprintCallable)
	bool IsIdle() const { return MovementState == EUnitMovementState::Idle; }

	// RPC

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerWalkPath(ATile* Target);

	// Utility

	void Highlight();
	void UnHighlight();
	float GetBoardRotation();
	TArray<ATile*> GetMovementTiles();

public:

	// BP Edit

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TObjectPtr<UTexture2D> UnitIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	int32 TileRange = 4;

	// Replicated

	UPROPERTY(Replicated)
	TWeakObjectPtr<ATile> Tile;

	UPROPERTY(Replicated)
	ABoard* Board;

	UPROPERTY(Replicated)
	int32 PlayerId;

	UPROPERTY(Replicated)
	FVector ServerLocation;

	UPROPERTY(Replicated)
	FRotator ServerRotation;

	UPROPERTY(Replicated)
	EUnitMovementState MovementState = EUnitMovementState::Idle;

	UPROPERTY(Replicated)
	EUnitMovementAnimType MovementType = EUnitMovementAnimType::Idle;

	UPROPERTY(Replicated)
	bool bHasMovedWay = false;

	UPROPERTY(Replicated)
	bool bHasMovedBack = false;

	// Server only

	ETacticsUnit UnitID;
	ETacticsDirection Direction;

protected:

	virtual void BeginPlay() override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* ArrowComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UUnitMovementComponent* MovementComponent;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;


};
