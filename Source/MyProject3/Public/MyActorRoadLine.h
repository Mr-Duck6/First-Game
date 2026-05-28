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

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


    //Road
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        UStaticMeshComponent* RoadMesh;

    TArray<FVector> LinePoints;

    void GenerateCells();
    FVector GetCellLocation(int32 Index);
    void InitializeRoadLine(int32 InLineIndex);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Road Settings")
        int32 LineIndex = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Road Settings")
        int32 LineLength = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Road Settings")
    float StartLocationY;

private:
    UPROPERTY()
        float CellSize;



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

    int32 DriveDistance;
    FTimerHandle CarTimer;
    int32 CarTimeToRespawn;
    FVector MainRoadDirection;

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