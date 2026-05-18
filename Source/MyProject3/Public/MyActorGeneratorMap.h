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
        UStaticMeshComponent* Mesh;

    //Objects ref
    UPROPERTY(EditAnywhere, Category = "Prefabs")
        TSubclassOf<AMyActorBaseStaticObject> BlueprintToSpawnBarrel;

    UPROPERTY(EditAnywhere, Category = "Prefabs")
        TSubclassOf<AMyActorCharger> BlueprintToSpawnCharger;

    UPROPERTY()
        AMyPawnPlayer* Player;

    UPROPERTY()
        AMyActorFogKiller* Fog;

    //Map settings
    UPROPERTY(EditAnywhere, Category = "Map Settings")
        int32 MaxLines;

    UPROPERTY(EditAnywhere, Category = "Map Settings")
    int32 MaxBarrelInLine;

    UPROPERTY(EditAnywhere, Category = "Map Settings")
        int32 StartLinesCount;

    UPROPERTY(EditAnywhere, Category = "Map Settings")
        float DistanceBetweenLines;

    float CurrentX;
    float NextSpawnTrigger;
    UPROPERTY(EditAnywhere, Category = "Map Settings")
        int32 LinesBehindToKeep;

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;


    //Functions
    void SpawnLine();

    void CheckPlayerProgress();

    void RemoveOldLine();

    void SpawnOnbjects(int32 LineIndex);

    void DeleteOldOnbjects(float LineX);

    bool CheckSafeRoad(AMyActorRoadLine* Line);

    bool CheckDangerRoad(AMyActorRoadLine* Line);

    void SpawnBarrel(AMyActorRoadLine* TargetLine);

    void SpawnCharger(AMyActorRoadLine* TargetLine);



    UPROPERTY(EditAnywhere, Category = "Map Settings")
        TArray<TSubclassOf<AMyActorRoadLine>> RoadLines;


    UPROPERTY()
        TArray<AActor*> SpawnedCharger;

    UPROPERTY()
        TArray<AMyActorBaseStaticObject*> SpawnedBarrel;
    UPROPERTY()
        TArray<AMyActorRoadLine*> SpawnedLines;

    //Counters
    int32 TrainCounter;
    int32 SafeCounter;
    int32 DangerCounter;

    int32 BarrelCouner;
    int32 ChargerCounterInLine;

    int32 LinesPassed;

};