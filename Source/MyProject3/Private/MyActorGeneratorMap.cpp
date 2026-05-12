#include "MyActorGeneratorMap.h"
#include "MyActorRoadLine.h"
#include "MyPawnPlayer.h"
#include "MyActorBaseStaticObject.h"
#include "MyActorCharger.h"
#include "MyActorBaseCar.h"
#include "Kismet/GameplayStatics.h"

AMyActorGeneratorMap::AMyActorGeneratorMap()
{
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Scene);

	//Variables
	TrainCounter = 0;
	SafeCounter = 0;
	DangerCounter = 0;

	ChargerCounterInLine = 0;
	ChargerDistance = 0;
	MaxBarrelInRow = 2;
	BarrelCouner = 0;

	LinesBehindToKeep = 5;

	MaxLines = 30;
	StartLinesCount = 30;
	DistanceBetweenLines = 100.f;
	LampOffsetX = 493.f;
	LampZ = 500.f;
	CurrentY = 0.f;
	NextSpawnTrigger = 100.f;
	PassedLines = 0;
}

void AMyActorGeneratorMap::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<AMyPawnPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());

	NextSpawnTrigger = DistanceBetweenLines;

	for (int32 i = 0; i < StartLinesCount; i++)//Spawned first lines
	{
		SpawnLine();
		SpawnOnbjects(i);
	}
}

void AMyActorGeneratorMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckPlayerProgress();
}


void AMyActorGeneratorMap::CheckPlayerProgress()
{
	if (!Player) return;

	float PlayerY = Player->GetActorLocation().Y;

	if (PlayerY > NextSpawnTrigger)//Spawn new lines and delete old lines
	{
		SpawnLine();

		SpawnOnbjects(SpawnedLines.Num() - 1);

		PassedLines++;

		if (PassedLines >= 10)
		{
			RemoveOldLine();
			RemoveOldLine();

			PassedLines = 0;
		}

		NextSpawnTrigger += DistanceBetweenLines;
	}
}


bool AMyActorGeneratorMap::CheckSafeRoad(int32 LineIndex)//Check line at safe road
{
	if (!SpawnedLines.IsValidIndex(LineIndex)) return false;

	if (!RoadLines.IsValidIndex(1)) return false;

	return SpawnedLines[LineIndex]->GetClass() == RoadLines[1];
}


bool AMyActorGeneratorMap::CheckDangerRoad(int32 LineIndex)//Check line at Danger road
{
	if (!SpawnedLines.IsValidIndex(LineIndex)) return false;

	if (!RoadLines.IsValidIndex(1)) return false;

	return SpawnedLines[LineIndex]->GetClass() == RoadLines[0];
}


void AMyActorGeneratorMap::SpawnOnbjects(int32 LineIndex)//Spawn objects on line
{
	if (!SpawnedLines.IsValidIndex(LineIndex)) return;

	BarrelCouner = 0;
	ChargerCounterInLine = 0;

	int32 RandomNum = FMath::RandRange(0, 1);

	if (RandomNum == 0)
	{
		SpawnBarrel(LineIndex);
	}
	else
	{
		if (ChargerDistance < 4) return;

		SpawnCharger(LineIndex);
		ChargerDistance = 0;
	}
}


void AMyActorGeneratorMap::SpawnBarrel(int32 LineIndex)//Spawn barrel
{

	if (!BlueprintToSpawnBarrel) return;

	if (CheckSafeRoad(LineIndex))
	{
		for (int32 i = 0; i < SpawnedLines[LineIndex]->LinePoints.Num(); i++)
		{
			if (BarrelCouner >= MaxBarrelInRow)  return;


			if (FMath::RandRange(0, 1) == 0)
			{
				AMyActorBaseStaticObject* NewObject =
					GetWorld()->SpawnActor<AMyActorBaseStaticObject>(
						BlueprintToSpawnBarrel,
						SpawnedLines[LineIndex]->LinePoints[i],
						FRotator::ZeroRotator);

				if (NewObject)
				{
					BarrelCouner++;
					SpawnedBarrel.Add(NewObject);
				}
			}
		}
	}
}

void AMyActorGeneratorMap::SpawnCharger(int32 LineIndex)//Spawn charger
{
	if (!BlueprintToSpawnCharger)  return;

	if (CheckSafeRoad(LineIndex))
	{
		for (int32 i = 0; i < SpawnedLines[LineIndex]->LinePoints.Num(); i++)
		{
			if (ChargerCounterInLine == 1) return;

			if (FMath::RandRange(0, 1) == 0)
			{
				AMyActorCharger* NewObject =GetWorld()->SpawnActor<AMyActorCharger>(BlueprintToSpawnCharger,
					SpawnedLines[LineIndex]->LinePoints[i],FRotator::ZeroRotator);

				if (NewObject)
				{
					ChargerCounterInLine++;
					SpawnedCharger.Add(NewObject);
					
				}
			}
		}
	}

}


void AMyActorGeneratorMap::SpawnCar(int32 LineIndex)
{

	if (!BlueprintToSpawnCar) return;

	if (CheckDangerRoad(LineIndex))
	{

		AMyActorBaseCar* NewCar = GetWorld()->SpawnActor<AMyActorBaseCar>(BlueprintToSpawnCar,
			SpawnedLines[LineIndex]->LinePoints[0], FRotator::ZeroRotator);

		if (NewCar)
		{
			SpawnedCar.Add(NewCar);
		}
	}
}

void AMyActorGeneratorMap::DeleteOldOnbjects(float LineY)//Delete old objects
{
	for (int32 i = SpawnedBarrel.Num() - 1; i >= 0; i--)
	{
		if (!SpawnedBarrel[i]) continue;

		float BarrelY = SpawnedBarrel[i]->GetActorLocation().Y;

		if (FMath::IsNearlyEqual(BarrelY, LineY, 1.f))
		{
			SpawnedBarrel[i]->Destroy();

			SpawnedBarrel.RemoveAt(i);
		}
	}

	for (int32 i = SpawnedCharger.Num() - 1; i >= 0; i--)
	{
		if (!SpawnedCharger[i]) continue;

		float ChargerY = SpawnedCharger[i]->GetActorLocation().Y;

		if (FMath::IsNearlyEqual(ChargerY, LineY, 1.f))
		{
			SpawnedCharger[i]->Destroy();
			SpawnedCharger.RemoveAt(i);
		}
	}
}

void AMyActorGeneratorMap::RemoveOldLine()//Remove old objects
{
	if (SpawnedLines.Num() == 0) return;
	if (!Player) return;

	AMyActorRoadLine* OldLine = SpawnedLines[0];

	if (!OldLine) return;

	float PlayerY = Player->GetActorLocation().Y;
	float LineY = OldLine->GetActorLocation().Y;

	if (PlayerY - LineY < DistanceBetweenLines * LinesBehindToKeep)
	{
		return;
	}

	DeleteOldOnbjects(LineY);

	for (int32 i = SpawnedLamps.Num() - 1; i >= 0; i--)//Spawn lamps
	{
		if (!SpawnedLamps[i]) continue;

		float LampY = SpawnedLamps[i]->GetActorLocation().Y;

		if (FMath::IsNearlyEqual(LampY, LineY, 1.f))
		{
			SpawnedLamps[i]->Destroy();
			SpawnedLamps.RemoveAt(i);
		}
	}

	OldLine->Destroy();//Clear lines

	SpawnedLines.RemoveAt(0);
}

void AMyActorGeneratorMap::SpawnLine()//Spawn functions
{
	if (RoadLines.Num() == 0) return;

	int32 RandomIndex = FMath::RandRange(0, RoadLines.Num() - 1);//Chose road

	switch (RandomIndex)//Update limits
	{
	case 0:
		DangerCounter++;
		break;
	case 1:
		ChargerDistance++;
		SafeCounter++;
		break;
	case 2:
		TrainCounter++;
		break;
	}

	if (DangerCounter >= 4)//Check limits
	{
		RandomIndex = 1;
		DangerCounter = 0;
	}
	else if (SafeCounter >= 3)
	{
		RandomIndex = 0;
		SafeCounter = 0;
	}
	else if (TrainCounter >= 3)
	{
		RandomIndex = 0;
		TrainCounter = 0;
	}

	TSubclassOf<AMyActorRoadLine> RandomClass = RoadLines[RandomIndex];//Spawn road

	FVector SpawnLocation = GetActorLocation() + FVector(0.f, CurrentY, 0.f);

	AMyActorRoadLine* NewLine = GetWorld()->SpawnActor<AMyActorRoadLine>(RandomClass, SpawnLocation, FRotator(0.f, 90.f, 0.f));

	if (!NewLine) return;

	SpawnedLines.Add(NewLine);


	if (LampBlueprintClass)//Spawn lamps
	{
		AActor* LampL = GetWorld()->SpawnActor<AActor>(LampBlueprintClass, FVector(LampOffsetX, CurrentY, LampZ), FRotator::ZeroRotator);

		AActor* LampR = GetWorld()->SpawnActor<AActor>(LampBlueprintClass, FVector(-LampOffsetX, CurrentY, LampZ), FRotator::ZeroRotator);

		SpawnedLamps.Add(LampL);
		SpawnedLamps.Add(LampR);
	}

	CurrentY += DistanceBetweenLines;
}