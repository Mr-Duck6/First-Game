
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActorGeneratorMap.generated.h"

class AMyActorRoadLine;

UCLASS()
class MYPROJECT3_API AMyActorGeneratorMap : public AActor
{
	GENERATED_BODY()

public:

	AMyActorGeneratorMap();

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UStaticMeshComponent* Mesh;


protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<TSubclassOf<AMyActorRoadLine>> RoadLines;

	UPROPERTY()
		TArray<AMyActorRoadLine*> SpawnedLines;

	UPROPERTY(EditAnywhere)
		int32 StartLinesCount = 10;

	UPROPERTY(EditAnywhere)
		float DistanceBetweenLines = 100.f;

	float CurrentY = 0.f;

	void SpawnLine();

	AMyActorRoadLine* GetLine(int32 Index);
};

