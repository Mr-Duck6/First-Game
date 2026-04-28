

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
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
	virtual void DrawSuareGrid(int32 j);

	void SpawnLines();
	void SpawnProps();

	FVector LinePoint;//Center of line
	FVector GridPoint;//Points in grid

	int32 GridSizeY=10;
	int32 GridSizeX = 10;

	UPROPERTY(EditAnywhere, Category = "Data")
		UDataTable* MyTable;

};