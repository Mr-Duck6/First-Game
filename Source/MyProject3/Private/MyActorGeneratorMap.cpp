#include "MyActorGeneratorMap.h"
#include "MyActorRoadLine.h"
#include "MyPawnPlayer.h"
#include "Kismet/GameplayStatics.h"

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

	Player = Cast<AMyPawnPlayer>(
		GetWorld()->GetFirstPlayerController()->GetPawn()
		);

	for (int i = 0; i < StartLinesCount; i++)
	{
		SpawnLine();
	}
}

void AMyActorGeneratorMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckPlayerProgress();
}

void AMyActorGeneratorMap::SpawnLine()
{
	if (RoadLines.Num() == 0) return;

	int32 RandomIndex = FMath::RandRange(0, RoadLines.Num() - 1);

	TSubclassOf<AMyActorRoadLine> RandomClass =
		RoadLines[RandomIndex];

	FVector SpawnLocation =
		GetActorLocation() + FVector(0.f, CurrentY, 0.f);

	AMyActorRoadLine* NewLine =
		GetWorld()->SpawnActor<AMyActorRoadLine>(
			RandomClass,
			SpawnLocation,
			FRotator(0.f, 90.f, 0.f)
			);

	if (!NewLine) return;

	SpawnedLines.Add(NewLine);

	// Левая лампа
	if (LampBlueprintClass)
	{
		GetWorld()->SpawnActor<AActor>(
			LampBlueprintClass,
			FVector(LampOffsetX, CurrentY, LampZ),
			FRotator::ZeroRotator
			);

		// Правая лампа
		GetWorld()->SpawnActor<AActor>(
			LampBlueprintClass,
			FVector(-LampOffsetX, CurrentY, LampZ),
			FRotator::ZeroRotator
			);
	}

	CurrentY += DistanceBetweenLines;
}

void AMyActorGeneratorMap::CheckPlayerProgress()
{
	if (!Player) return;

	float PlayerY = Player->GetActorLocation().Y;

	if (PlayerY > NextSpawnTrigger)
	{
		SpawnLine();

		if (SpawnedLines.Num() > MaxLines)
		{
			RemoveOldLine();
		}

		NextSpawnTrigger += DistanceBetweenLines;
	}
}

void AMyActorGeneratorMap::RemoveOldLine()
{
	if (SpawnedLines.Num() == 0) return;

	AMyActorRoadLine* OldLine = SpawnedLines[0];

	if (OldLine)
	{
		OldLine->Destroy();
	}

	SpawnedLines.RemoveAt(0);
}