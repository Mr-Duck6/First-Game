
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"


UCLASS()
class MYPROJECT3_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()

public:
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
		float MaxLifeTime;
	
};
