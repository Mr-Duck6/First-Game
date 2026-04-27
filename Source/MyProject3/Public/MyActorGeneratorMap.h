

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActorGeneratorMap.generated.h"

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

	void DrawLinesGrid(int32 j);
	void DrawSuareGrid(int32 j);

	void SpawnLines();
	void SpawnProps();

	FVector LinePoint;
	FVector GridPoint;

	int32 GridSizeY=10;
	int32 GridSizeX = 10;

};