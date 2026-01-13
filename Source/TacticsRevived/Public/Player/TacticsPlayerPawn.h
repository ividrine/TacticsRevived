// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "TacticsRevived/Enums/TacticsEnums.h"
#include "TacticsPlayerPawn.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnitSelected, AUnit*, Unit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnitDeselected);

class AUnit;
class ATile;
class IOnlineSession;

class ATacticsGameState;
class ATacticsPlayerController;
class ATacticsPlayerState;

UCLASS()
class TACTICSREVIVED_API ATacticsPlayerPawn : public APawn
{
	GENERATED_BODY()

	public:

		ATacticsPlayerPawn();

		virtual void Tick(float DeltaTime) override;
		virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
		virtual void PossessedBy(AController* NewController) override;

		void InitPlayerState();

		void BindTileEvents();

		ETacticsSelectionMode GetSelectionType() { return SelectionType; }
		void SetSelectionType(ETacticsSelectionMode Selection) { SelectionType = Selection; }

		AUnit* GetSelectedUnit() { return SelectedUnit; }
		void SetSelectedUnit(AUnit* Unit) { SelectedUnit = Unit; }

		ATacticsPlayerController* GetTacticsPlayerController();
		ATacticsPlayerState* GetTacticsPlayerState();
		ATacticsGameState* GetTacticsGameState();

		UPROPERTY(BlueprintAssignable)
		FOnUnitSelected OnUnitSelected;

		UPROPERTY(BlueprintAssignable)
		FOnUnitDeselected OnUnitDeselected;

		TArray<ATile*> ActiveTiles;
		TArray<ATile*> HitAreaTiles;
		

	protected:
		virtual void BeginPlay() override;
		virtual void OnRep_Controller() override;
		void OnRep_PlayerState() override;

		void SetHighlights(TArray<ATile*> Tiles, FLinearColor Color);
		

		UPROPERTY(VisibleAnywhere)
		class USceneComponent* SceneComponent;

		UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;

		UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		class UInputAction* ZoomAction;


		

	private:

		void InitMouse();

		void ZoomCamera(const FInputActionValue& Value);

		UFUNCTION()
		void BeginCursorOver(AActor* Actor);

		UFUNCTION()
		void EndCursorOver(AActor* Actor);

		UFUNCTION()
		void Click(AActor* Actor, FKey ButtonPressed);

		UPROPERTY()
		ATacticsGameState* TacticsGameState;

		UPROPERTY()
		ATacticsPlayerController* TacticsPlayerController;

		UPROPERTY()
		ATacticsPlayerState* TacticsPlayerState;

		UPROPERTY(EditAnywhere, Category = "Camera")
		float MinZoom = 0.f;

		UPROPERTY(EditAnywhere, Category = "Camera")
		float MaxZoom = 2000.f;

		UPROPERTY(EditAnywhere, Category = "Camera")
		float ZoomStep = 100.f;

		UPROPERTY(EditAnywhere, Category = "Camera")
		float ZoomSpeed = 10.f; // Lower values = smoother

		float TargetZoom = 0.f;
		
		ETacticsSelectionMode SelectionType;
		AUnit* SelectedUnit = nullptr;
		AUnit* ViewedUnit = nullptr;
		ATile* LastHoveredTile = nullptr;
	
};
