#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

    void InitializeRoadLine(int32 InLineIndex);

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;//Clear all old objects

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        UStaticMeshComponent* RoadMesh;

    TArray<FVector> LinePoints;

    UPROPERTY(VisibleAnywhere, Category = "Lamp")
        TArray<AMyActorLamp*> SpawnedLamps;

    UPROPERTY(EditAnywhere, Category = "Lamp")
        TArray<TSubclassOf<AMyActorLamp>> BluePrintToSpawnLamp;

    void GenerateCells();
    FVector GetCellLocation(int32 Index);
    void SpawnLamp(int32 InLineIndex);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Road Settings")
        int32 LineIndex = 0;

    UPROPERTY()
        int32 LineLength = 10;

    UPROPERTY()
        float CellSize = 100.f;


    //Car
    UPROPERTY(VisibleAnywhere, Category = "Car")
        TArray<AMyActorBaseCar*> SpawnedCars;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Car")
        TArray<TSubclassOf<AMyActorBaseCar>> BluePrintToSpawnCar;

    int32 DriveDistance;
    FTimerHandle CarTimer;
    int32 CarTimeToRespawn;

    void SpawnCar();
    void DeleteCars();

    //Train
    UPROPERTY(EditDefaultsOnly, Category = "Train Settings")
        TSubclassOf<AMyActorTrain> TrainClass;

    UPROPERTY()
        AMyActorTrain* SpawnedTrain; 

    FTimerHandle TrainCycleTimer;  
    FTimerHandle WarningLightTimer; 

    void PlanTrainAttack(); 
    void TriggerTrainMove();
};