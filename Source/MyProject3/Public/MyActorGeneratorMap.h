#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActorGeneratorMap.generated.h"

class AMyActorRoadLine;
class AMyPawnPlayer;

UCLASS()
class MYPROJECT3_API AMyActorGeneratorMap : public AActor
{
	GENERATED_BODY()

public:

	AMyActorGeneratorMap();

	UPROPERTY(VisibleAnywhere)
		USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "Road")
		TArray<TSubclassOf<AMyActorRoadLine>> RoadLines;

	UPROPERTY()
		TArray<AMyActorRoadLine*> SpawnedLines;

	UPROPERTY(EditAnywhere, Category = "Lamp")
		TSubclassOf<AActor> LampBlueprintClass;

	UPROPERTY()
		AMyPawnPlayer* Player;

	UPROPERTY(EditAnywhere)
		int32 MaxLines = 20;

	UPROPERTY(EditAnywhere)
		int32 StartLinesCount = 20;

	UPROPERTY(EditAnywhere)
		float DistanceBetweenLines = 100.f;

	UPROPERTY(EditAnywhere)
		float LampOffsetX = 493.f;

	UPROPERTY(EditAnywhere)
		float LampZ = 320.f;

	float CurrentY = 0.f;

	float NextSpawnTrigger = 100.f;

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void SpawnLine();

	void CheckPlayerProgress();

	void RemoveOldLine();
};