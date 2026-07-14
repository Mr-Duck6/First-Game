#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CarsParametrsDA.h"
#include "MyActorRoadLine.generated.h"

class AMyActorBaseCar;
class AMyActorLamp;
class AMyActorTrain;

DECLARE_LOG_CATEGORY_EXTERN(RoadLineLog, Log, All);

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

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        UStaticMeshComponent* RoadMesh;

    TArray<FVector> LinePoints;

    void GenerateCells();
    FVector GetCellLocation(int32 Index);
    void InitializeRoadLine(int32 InLineIndex);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Road Settings")
        int32 LineIndex;

private:
    UPROPERTY()
        float CellSize;
    UPROPERTY()
        float StartLocationY;
    UPROPERTY()
        int32 LineLength;

public:

    //Lamp
    UPROPERTY(VisibleAnywhere, Category = "Lamp")
        TArray<AMyActorLamp*> SpawnedLamps;

    UPROPERTY(EditAnywhere, Category = "Lamp")
        TArray<TSubclassOf<AMyActorLamp>> BluePrintToSpawnLamp;

    void SpawnLamp(int32 InLineIndex);


    //Cars
    UPROPERTY(VisibleAnywhere, Category = "Car")
        TArray<AMyActorBaseCar*> SpawnedCars;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Car")
        TArray<TSubclassOf<AMyActorBaseCar>> BluePrintToSpawnCar;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Car")
        TArray<UCarsParametrsDA*> CarsCases;

    FTimerHandle CarTimer;
    FVector MainRoadDirection;
    int32 YSpawn;
    float StartYCarLocation;
    float StartCarYLocation;

    void SpawnCar();
    void DeleteCars();

    //Train
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Car")
        TSubclassOf<AMyActorTrain> TrainClass;

    UPROPERTY()
        AMyActorTrain* SpawnedTrain;

    FTimerHandle TrainCycleTimer;
    FTimerHandle WarningLightTimer;

    void PlanTrainAttack();
    void TriggerTrainMove();
};