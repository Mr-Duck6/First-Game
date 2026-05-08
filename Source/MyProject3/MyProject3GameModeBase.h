

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyProject3GameModeBase.generated.h"


UCLASS()
class MYPROJECT3_API AMyProject3GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	void RestartLevel();
	
};
