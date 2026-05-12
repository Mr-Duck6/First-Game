#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActorGeneratorMap.generated.h"

class AMyActorRoadLine;
class AMyPawnPlayer;
class AMyActorBaseStaticObject;
class AMyActorCharger;
class AMyActorBaseCar;

UCLASS()
class MYPROJECT3_API AMyActorGeneratorMap : public AActor
{
	GENERATED_BODY()

public:

	AMyActorGeneratorMap();
	//Components
	UPROPERTY(VisibleAnywhere)
		USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* Mesh;

	//Reference
	UPROPERTY(EditAnywhere, Category = "Lamp")
		TSubclassOf<AActor> LampBlueprintClass;

	UPROPERTY(EditAnywhere, Category = "Barrel")
		TSubclassOf<AMyActorBaseStaticObject> BlueprintToSpawnBarrel;
	UPROPERTY(EditAnywhere, Category = "Charger")
		TSubclassOf<AMyActorCharger> BlueprintToSpawnCharger;
	UPROPERTY(EditAnywhere, Category = "Car")
		TSubclassOf<AMyActorBaseCar> BlueprintToSpawnCar;

	UPROPERTY()
		AMyPawnPlayer* Player;

	//Road
	UPROPERTY(EditAnywhere)
		int32 MaxLines;

	UPROPERTY(EditAnywhere)
		int32 StartLinesCount;

	UPROPERTY(EditAnywhere)
		float DistanceBetweenLines;

	UPROPERTY(EditAnywhere)
		float LampOffsetX;

	UPROPERTY(EditAnywhere)
		float LampZ;

	float CurrentY;

	float NextSpawnTrigger;

	int32 PassedLines;

	//Functions
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void SpawnLine();

	void CheckPlayerProgress();

	void RemoveOldLine();

	void SpawnOnbjects(int32 LineIndex);

	void DeleteOldOnbjects(float LineY);

	bool CheckSafeRoad(int32 LineIndex);

	bool CheckDangerRoad(int32 LineIndex);

	void SpawnBarrel(int32 LineIndex);

	void SpawnCharger(int32 LineIndex);

	void SpawnCar(int32 LineIndex);


	//Array
	UPROPERTY()
		TArray<AActor*> SpawnedCharger;
	UPROPERTY()
		TArray<AActor*> SpawnedLamps;
	UPROPERTY()
		TArray<AMyActorBaseStaticObject*> SpawnedBarrel;
	UPROPERTY(EditAnywhere, Category = "Road")
		TArray<TSubclassOf<AMyActorRoadLine>> RoadLines;
	UPROPERTY()
		TArray<AMyActorRoadLine*> SpawnedLines;
	UPROPERTY()
		TArray<AMyActorBaseCar*>SpawnedCar;

	//Objects
	int32 MaxBarrelInRow;
	int32 BarrelCouner;
	int32 ChargerCounterInLine;
	int32 ChargerDistance;

	int32 TrainCounter;
	int32 SafeCounter;
	int32 DangerCounter;

	UPROPERTY(EditAnywhere, Category = "Streaming")
		int32 LinesBehindToKeep;

};