#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "MyActorRoadLine.h" 
#include "MyActorGeneratorMap.generated.h"

class AMyPawnPlayer;
class AMyActorBaseStaticObject;
class AMyActorCharger;
class AMyActorFogKiller;

DECLARE_LOG_CATEGORY_EXTERN(GeneratorMapLog, Log, All);

UCLASS()
class MYPROJECT3_API AMyActorGeneratorMap : public AActor
{
    GENERATED_BODY()

public:
    AMyActorGeneratorMap();

    //Components
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
        USceneComponent* Scene;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
        UStaticMeshComponent* DevMesh;

    //Objects ref
    UPROPERTY(EditAnywhere, Category = "StaticObject")
        TSubclassOf<AMyActorBaseStaticObject> BlueprintToSpawnStaticObject;

    UPROPERTY(EditAnywhere, Category = "Charger")
        TSubclassOf<AMyActorCharger> BlueprintToSpawnCharger;

    UPROPERTY()
        AMyPawnPlayer* Player;

    UPROPERTY()
        AMyActorFogKiller* Fog;


    //Map generation settings
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Settings")
        int32 MaxLines;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Settings")
    int32 MaxBarrelInLine;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Settings")
        int32 StartLinesCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Settings")
        float DistanceBetweenLines;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Settings")
        int32 NextSpawnTrigger;


    float CurrentXLine;


    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;


    //Functions
    void CheckPlayerProgress();
    void SpawnLine();

    void RemoveOldLine();


    void SpawnOnbjects(int32 LineIndex);

    bool CheckSafeRoad(AMyActorRoadLine* Line);
    bool CheckDangerRoad(AMyActorRoadLine* Line);

    void SpawnBarrel(AMyActorRoadLine* TargetLine);
    void SpawnCharger(AMyActorRoadLine* TargetLine);

    void DeleteOldOnbjects(float LineX);


    UPROPERTY(EditAnywhere, Category = "Map Settings")
        TArray<TSubclassOf<AMyActorRoadLine>> RoadLines;

    UPROPERTY()
        TArray<AMyActorCharger*> SpawnedCharger;

    //Created objects arrays
    UPROPERTY()
        TArray<AMyActorBaseStaticObject*> SpawnedBarrel;

    UPROPERTY()
        TArray<AMyActorRoadLine*> SpawnedLines;

    //Counters
    int32 TrainCounter;
    int32 SafeCounter;
    int32 DangerCounter;
    int32 BarrelCouner;
    int32 ChargerCounter;

    int32 FirstThreeRoad;

};