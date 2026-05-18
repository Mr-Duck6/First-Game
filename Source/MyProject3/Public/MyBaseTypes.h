#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" 
#include "MyBaseTypes.generated.h" 


UENUM(BlueprintType)
enum class EMyBaseTypes : uint8
{
    SafeRoad    UMETA(DisplayName = "Safe Road"),
    DangerRoad  UMETA(DisplayName = "Danger Road"),
    TrainRoad   UMETA(DisplayName = "Train Road")
};


USTRUCT(BlueprintType)
struct FRoadType : public FTableRowBase
{
    GENERATED_BODY()

        UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TSoftObjectPtr<UStaticMesh> Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TArray<FVector> GridPoints;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        int32 LineID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        bool IsBusy = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        EMyBaseTypes LineType;
};


