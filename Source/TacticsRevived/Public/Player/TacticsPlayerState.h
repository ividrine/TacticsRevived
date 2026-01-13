

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TacticsRevived/Enums/TacticsEnums.h"
#include "TacticsPlayerState.generated.h"

class AUnit;

UCLASS()
class TACTICSREVIVED_API ATacticsPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	TMap<FIntPoint, ETacticsUnit> UnitLayout =
	{
		//{{2, 1}, ETacticsUnit::Sparrow},
		{{1, 8}, ETacticsUnit::Greystone},
		//{{2, 9}, ETacticsUnit::Countess},
		{{2, 5}, ETacticsUnit::Greystone}
		//{{1, 3}, ETacticsUnit::Morigesh}
	};
	
};
