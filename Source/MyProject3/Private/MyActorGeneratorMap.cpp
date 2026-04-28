

#include "MyActorGeneratorMap.h"
#include "DrawDebugHelpers.h"
#include "MyBaseTypes.h"
#include "Logging/LogMacros.h"


AMyActorGeneratorMap::AMyActorGeneratorMap()
{

	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Scene);

}


void AMyActorGeneratorMap::BeginPlay()
{
	Super::BeginPlay();

		DrawLinesGrid(10);
		DrawSuareGrid(10);
}


void AMyActorGeneratorMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyActorGeneratorMap::DrawLinesGrid(int32 j)//Drawing line and createing point of center lines
{
	UE_LOG(LogTemp, Display, TEXT("Draw lines and points for road"));
	FVector Start(0, -500, 0);
	FVector End(0, 500, 0);
	LinePoint = FVector(50, 0, 0);

	for (int i = 0; i < j; i++)
	{
		DrawDebugPoint(GetWorld(), LinePoint, 5, FColor::Red, true);
		LinePoint += FVector(100, 0, 0);

		DrawDebugLine(GetWorld(), Start, End, FColor::Red, true);
		Start += FVector(100, 0, 0);
		End += FVector(100, 0, 0);

	}

}


void AMyActorGeneratorMap::DrawSuareGrid(int32 j)//Displays a grid of item locations and player movement.
{
	UE_LOG(LogTemp, Display, TEXT("Draw suare and points for road"));
	GridPoint = FVector(50, -450, 0);
	for (int i = 0; i < j; i++)
	{
		for (int k = 0; k < GridSizeY; k++)
		{
			DrawDebugPoint(GetWorld(), GridPoint, 4, FColor::Orange, true);
			GridPoint += FVector(0, 100, 0);
		}
		UE_LOG(LogTemp, Log, TEXT("Point cordinate: %s"), *GridPoint.ToString());
		GridPoint += FVector(100, -1000, 0);
	}
}

void AMyActorGeneratorMap::SpawnLines()//Spawn mesh line
{
	

	

}

void AMyActorGeneratorMap::SpawnProps()//Spawn mesh other objects
{



}

