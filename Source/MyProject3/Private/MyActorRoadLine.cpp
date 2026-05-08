#include "MyActorRoadLine.h"
#include "DrawDebugHelpers.h"

AMyActorRoadLine::AMyActorRoadLine()
{
    PrimaryActorTick.bCanEverTick = true;

    RoadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RoadMesh"));

    RootComponent = RoadMesh;
}

void AMyActorRoadLine::BeginPlay()
{
    Super::BeginPlay();

    GenerateCells();
}

void AMyActorRoadLine::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AMyActorRoadLine::GenerateCells()
{

    LinePoints.Empty();

    float StartX = -400.f;

    for (int i = 0; i < LineLength; i++)
    {
    FVector CellLocation=GetActorLocation() +FVector(StartX + i * CellSize, 0, 0);
    LinePoints.Add(CellLocation);

    if (GetWorld())
    {
        DrawDebugPoint(GetWorld(), CellLocation, 20.f, FColor::Red, true);
    }
    }
}

FVector AMyActorRoadLine::GetCellLocation(int32 Index)//Get location
{
    if (!LinePoints.IsValidIndex(Index))
    {
        return FVector::ZeroVector;
    }

    return LinePoints[Index];
}