#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" 
#include "MyBaseTypes.generated.h" 

USTRUCT(BlueprintType)
struct FRoadType : public FTableRowBase
{
    GENERATED_BODY()

        UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TSoftObjectPtr<UStaticMesh> Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        int32 ChanceToSpawn;
};
