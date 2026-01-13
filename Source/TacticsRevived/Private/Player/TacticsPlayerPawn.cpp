#include "Player/TacticsPlayerPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Game/TacticsGameState.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Core/Board.h"
#include "Core/Tile.h"
#include "Core/Unit/Unit.h"
#include "Camera/CameraComponent.h"
#include "Game/TacticsGameMode.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TacticsRevived/Constants/Colors.h"
#include "TacticsRevived/Macros/DebugMacros.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Player/TacticsPlayerController.h"
#include "UI/HUD/TacticsHUD.h"
#include "Player/TacticsPlayerState.h"
#include "Player/TacticsPlayerStart.h"
#include "GameFramework/SpringArmComponent.h"

ATacticsPlayerPawn::ATacticsPlayerPawn()
{

    PrimaryActorTick.bCanEverTick = true;
	SelectionType = ETacticsSelectionMode::Move;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	SceneComponent->SetupAttachment(RootComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(SceneComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

}

void ATacticsPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, TargetZoom, DeltaTime, ZoomSpeed);
}

void ATacticsPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &ATacticsPlayerPawn::ZoomCamera);
	}
}


void ATacticsPlayerPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitMouse();
	InitPlayerState();
}

void ATacticsPlayerPawn::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitPlayerState();
}

void ATacticsPlayerPawn::OnRep_Controller()
{
	Super::OnRep_Controller();
	InitMouse();
}

void ATacticsPlayerPawn::InitPlayerState()
{
	ATacticsPlayerController* PC = GetTacticsPlayerController();
	ATacticsPlayerState* PS = GetTacticsPlayerState();
	ATacticsGameState* GS = GetTacticsGameState();

	if (PC && PS)
	{
		if (ATacticsHUD* HUD = PC->GetHUD<ATacticsHUD>())
		{
			HUD->InitOverlay(GS, PC, PS);
		}
	}
}

void ATacticsPlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	InitPlayerState();

	BindTileEvents();
	
}

void ATacticsPlayerPawn::BindTileEvents()
{
	if (ABoard* Board = Cast<ABoard>(UGameplayStatics::GetActorOfClass(GetWorld(), ABoard::StaticClass())))
	{
		for (ATile* Tile : Board->Tiles)
		{
			if (!Tile) continue;
			Tile->OnBeginCursorOver.AddDynamic(this, &ATacticsPlayerPawn::BeginCursorOver);
			Tile->OnEndCursorOver.AddDynamic(this, &ATacticsPlayerPawn::EndCursorOver);
			Tile->OnReleased.AddDynamic(this, &ATacticsPlayerPawn::Click);
		}
	}
}

void ATacticsPlayerPawn::InitMouse()
{
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		PC->bShowMouseCursor = true;
		PC->bEnableClickEvents = true;
		PC->bEnableMouseOverEvents = true;
	}
}

void ATacticsPlayerPawn::ZoomCamera(const FInputActionValue& Value)
{
	const float Val = Value.Get<float>();
	if (FMath::Abs(Val) > KINDA_SMALL_NUMBER)
	{
		TargetZoom = FMath::Clamp(TargetZoom - (Val * ZoomStep), MinZoom, MaxZoom);
	}
}

void ATacticsPlayerPawn::SetHighlights(TArray<ATile*> Tiles, FLinearColor Color)
{
	for (ATile* Tile : Tiles)
	{
		Tile->SetHighlight(Color);
	}
}

void ATacticsPlayerPawn::BeginCursorOver(AActor* Actor)
{
	if (ATile* Tile = Cast<ATile>(Actor))
	{

		switch (SelectionType)
		{
			case ETacticsSelectionMode::Move:
			{
				AUnit* Unit = Tile->Unit.Get();

				if (Unit)
				{
					Tile->SetHighlight(Colors::WhiteOpacity);
					
					if (Unit->PlayerId == TacticsPlayerState->PlayerId)
					{
						Unit->Highlight();
					}
				}
				else if (SelectedUnit && ActiveTiles.Contains(Tile))
				{
					Tile->SetHighlight(Colors::DarkBlueOpacity);
				}
			}
		}
		
	}
}

void ATacticsPlayerPawn::EndCursorOver(AActor* Actor)
{

	if (ATile* Tile = Cast<ATile>(Actor))
	{
		switch (SelectionType)
		{
			case ETacticsSelectionMode::Move:
			{
				if (AUnit* Unit = Tile->Unit.Get())
				{

					Tile->SetHighlight(Colors::Transparent);
					
					if (Unit != SelectedUnit)
					{
						Unit->UnHighlight();
					}
				}
				else if (SelectedUnit && ActiveTiles.Contains(Tile))
				{
					Tile->SetHighlight(Colors::BlueOpacity);
				}
			}
		}
	}
}

void ATacticsPlayerPawn::Click(AActor* Actor, FKey ButtonPressed)
{
	
	if (ATile* Tile = Cast<ATile>(Actor))
	{

		switch (SelectionType)
		{
			case ETacticsSelectionMode::Move:
			{
				
				if (AUnit* Unit = Tile->Unit.Get())
				{
					if (SelectedUnit && SelectedUnit == Unit)
					{
						// Deselect Unit
						SelectedUnit = nullptr;
						SetHighlights(ActiveTiles, Colors::Transparent);
						ActiveTiles.Empty();
						OnUnitDeselected.Broadcast();

					}
					else
					{
						// Deselect last unit
						if (SelectedUnit) SelectedUnit->UnHighlight();
						// Select the Unit
						SetHighlights(ActiveTiles, Colors::Transparent);
						SelectedUnit = Unit;
						ActiveTiles = Unit->GetMovementTiles();
						SetHighlights(ActiveTiles, Colors::BlueOpacity);
						OnUnitSelected.Broadcast(SelectedUnit);
					}
					
				}
				else
				{
					if (SelectedUnit)
					{
						SelectedUnit->UnHighlight();

						if (ActiveTiles.Contains(Tile))
						{
							// Initiate Move
							SetHighlights(ActiveTiles, Colors::Transparent);
							ActiveTiles.Empty();
							SelectedUnit->ServerWalkPath(Tile);
						}
						else
						{
							// Deselect Unit
							SetHighlights(ActiveTiles, Colors::Transparent);
							ActiveTiles.Empty();
							SelectedUnit = nullptr;
							OnUnitDeselected.Broadcast();
						}
					}
					
				}

			}
		}
		
	}
}

ATacticsPlayerController* ATacticsPlayerPawn::GetTacticsPlayerController()
{
	if (TacticsPlayerController == nullptr)
	{
		TacticsPlayerController = GetController<ATacticsPlayerController>();
	}

	return TacticsPlayerController;
}

ATacticsPlayerState* ATacticsPlayerPawn::GetTacticsPlayerState()
{
	if (TacticsPlayerState == nullptr)
	{
		TacticsPlayerState = GetPlayerState<ATacticsPlayerState>();
	}

	return TacticsPlayerState;
}

ATacticsGameState* ATacticsPlayerPawn::GetTacticsGameState()
{
	if (TacticsGameState == nullptr)
	{
		TacticsGameState = Cast<ATacticsGameState>(UGameplayStatics::GetGameState(this));
	}

	return TacticsGameState;
}
