
#include "MyActorGeneratorMap.h"
#include "MyBaseTypes.h"
#include "MyActorRoadLine.h"


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
    for (int i = 0; i < StartLinesCount; i++)
    {
        SpawnLine();
    }
   
}


void AMyActorGeneratorMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void  AMyActorGeneratorMap::SpawnLine()
{

    if (RoadLines.Num() == 0) return;

    int32 RandomIndex = FMath::RandRange(0, RoadLines.Num() - 1);

    TSubclassOf<AMyActorRoadLine> RandomClass =RoadLines[RandomIndex];

    FVector SpawnLocation =GetActorLocation() +FVector(0, CurrentY, 0);

    AMyActorRoadLine* NewLine =GetWorld()->SpawnActor<AMyActorRoadLine>(RandomClass,SpawnLocation, FRotator(0,90,0));

    if (NewLine)
    {
        SpawnedLines.Add(NewLine);
        CurrentY += DistanceBetweenLines;
    }
}

AMyActorRoadLine* AMyActorGeneratorMap::GetLine(int32 Index)
{
    if (SpawnedLines.IsValidIndex(Index))
    {
        return SpawnedLines[Index];
    }

    return nullptr;
}
