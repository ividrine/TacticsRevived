

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Tile.generated.h"

class AUnit;

UCLASS()
class TACTICSREVIVED_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
    ATile();

	void SetHighlight(FLinearColor Color);

    UPROPERTY(Replicated)
    TWeakObjectPtr<AUnit> Unit;

    // Position in the grid
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visual")
    FIntPoint BoardIndex;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visual")
    UStaticMeshComponent* ComponentMesh;

    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Visual")
    TWeakObjectPtr<ATile> N;
    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Visual")
    TWeakObjectPtr<ATile> S;
    UPROPERTY(Replicated,  VisibleAnywhere, BlueprintReadOnly, Category = "Visual")
    TWeakObjectPtr<ATile> E;
    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Visual")
    TWeakObjectPtr<ATile> W;

    protected:
	    virtual void BeginPlay() override;

    private:
        
        UMaterialInstanceDynamic* DynamicMaterialInstance;
		FName HighlightParameterName = "HighlightColor";

};
