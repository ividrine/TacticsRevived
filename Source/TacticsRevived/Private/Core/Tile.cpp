


#include "Core/Tile.h"
#include "Core/Unit/Unit.h"
#include "Net/UnrealNetwork.h"

ATile::ATile()
{
	bReplicates = true;
	ComponentMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CellMesh"));
}

void ATile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATile, Unit);
	DOREPLIFETIME(ATile, N);
	DOREPLIFETIME(ATile, S);
	DOREPLIFETIME(ATile, E);
	DOREPLIFETIME(ATile, W);
}

void ATile::BeginPlay()
{
	Super::BeginPlay();

	if (!ComponentMesh) return;

	UMaterialInterface* OriginalMaterial = ComponentMesh->GetMaterial(0);

	if (!OriginalMaterial) return;

	DynamicMaterialInstance = UMaterialInstanceDynamic::Create(OriginalMaterial, this);

	if (DynamicMaterialInstance)
	{
		ComponentMesh->SetMaterial(0, DynamicMaterialInstance);
	}

}

void ATile::SetHighlight(FLinearColor Color)
{
	if (!DynamicMaterialInstance) return;
	DynamicMaterialInstance->SetVectorParameterValue(HighlightParameterName, Color);
}
