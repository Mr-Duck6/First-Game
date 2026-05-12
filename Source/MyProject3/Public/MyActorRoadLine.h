#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActorRoadLine.generated.h"

UCLASS()
class MYPROJECT3_API AMyActorRoadLine : public AActor
{
    GENERATED_BODY()

public:

    AMyActorRoadLine();

protected:

    virtual void BeginPlay() override;

public:

    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UStaticMeshComponent* RoadMesh;

    UPROPERTY(EditAnywhere)
    int32 LineLength = 19;

    UPROPERTY(EditAnywhere)
    float CellSize = 100.f;

    TArray<FVector> LinePoints;

    UPROPERTY()
        TArray<AActor*> SpawnedLamps;

    void GenerateCells();

    FVector GetCellLocation(int32 Index);


};