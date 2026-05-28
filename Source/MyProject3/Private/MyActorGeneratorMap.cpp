#include "MyActorGeneratorMap.h"
#include "MyPawnPlayer.h"
#include "MyActorBaseStaticObject.h"
#include "MyActorCharger.h"
#include "MyActorFogKiller.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(GeneratorMapLog);

AMyActorGeneratorMap::AMyActorGeneratorMap()
{
    PrimaryActorTick.bCanEverTick = true;

    Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
    RootComponent = Scene;

    DevMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    DevMesh->SetupAttachment(Scene);

    TrainCounter = 0;
    SafeCounter = 0;
    DangerCounter = 0;
    ChargerCounter = 0;
    MaxBarrelInLine = 3;
    BarrelCouner = 0;

    MaxLines = 30;
    StartLinesCount = 30;
    DistanceBetweenLines = 100.f;
    CurrentXLine = 0.f;
    NextSpawnTrigger = 5.f;
}

void AMyActorGeneratorMap::BeginPlay()
{
    Super::BeginPlay();

    Player = Cast<AMyPawnPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());
    Fog = Cast<AMyActorFogKiller>(UGameplayStatics::GetActorOfClass(GetWorld(),
        AMyActorFogKiller::StaticClass()));

    for (int32 i = 0; i < StartLinesCount; i++)//Spawn first objets
    {
        SpawnLine();
    }

    for (int32 i = 1; i < StartLinesCount; i++)
    {
        SpawnOnbjects(i);
    }
}

void AMyActorGeneratorMap::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    CheckPlayerProgress();//Check player location
}

void AMyActorGeneratorMap::CheckPlayerProgress()
{
    UE_LOG(GeneratorMapLog, Display, TEXT("Function CheckPlayerProgress called"));
    if (!Player || !Fog) return;

    float PlayerX = Player->GetActorLocation().X;
    float FogX = Fog->GetActorLocation().X;

    int32 CurrentPlayerLineIndex = FMath::FloorToInt(PlayerX / DistanceBetweenLines);

    if (CurrentPlayerLineIndex >= NextSpawnTrigger)//Spawn line, if player is moveing
    {
        NextSpawnTrigger += 5;
        for (int32 i = 0; i < 10; i++)
        {
            SpawnLine();
            int32 NewLineIndex = SpawnedLines.Num() - 1;
            SpawnOnbjects(NewLineIndex);
        }
    }

    if (SpawnedLines.Num() > 0)//Delete line behind the fog
    {
        AMyActorRoadLine* OldLine = SpawnedLines[0];
        if (OldLine)
        {
            UE_LOG(GeneratorMapLog, Display, TEXT("Fog clear objects"));
            float LineX = OldLine->GetActorLocation().X;
            float SafeDistanceBehindFog = 400.0f;
            if (LineX <= (FogX - SafeDistanceBehindFog))
            {
                RemoveOldLine();
            }
        }
    }

    float DistanceOfFog = PlayerX - FogX;//Teleport
    if (DistanceOfFog >= 2000.0f)
    {
        UE_LOG(GeneratorMapLog, Display, TEXT("Fog teleported"));
        float NewLocationX = PlayerX - 1000.0f;
        FVector CurrentFogLoc = Fog->GetActorLocation();
        Fog->SetActorLocation(FVector(NewLocationX, CurrentFogLoc.Y, CurrentFogLoc.Z));
    }
}

bool AMyActorGeneratorMap::CheckSafeRoad(AMyActorRoadLine* Line)//Is safe road
{
    UE_LOG(GeneratorMapLog, Display, TEXT("Function CheckSafeRoad called"));
    if (!Line || !RoadLines.IsValidIndex(1)) return false;
    return Line->GetClass() == RoadLines[1];
}

bool AMyActorGeneratorMap::CheckDangerRoad(AMyActorRoadLine* Line)//Is danger road
{
    UE_LOG(GeneratorMapLog, Display, TEXT("Function CheckDangerRoad called"));
    if (!Line || !RoadLines.IsValidIndex(0)) return false;
    return Line->GetClass() == RoadLines[0];
}

void AMyActorGeneratorMap::SpawnOnbjects(int32 LineIndex)
{
    UE_LOG(GeneratorMapLog, Display, TEXT("Function SpawnOnbjects called"));
    if (!SpawnedLines.IsValidIndex(LineIndex)) return;

    AMyActorRoadLine* TargetLine = SpawnedLines[LineIndex];
    if (!TargetLine) return;

    BarrelCouner = 0;
    ChargerCounter = 0;

    int32 RandomNum = FMath::RandRange(0, 1);//Chose object to spawn
    if (RandomNum == 0) SpawnBarrel(TargetLine);
    else SpawnCharger(TargetLine);
}

void AMyActorGeneratorMap::SpawnBarrel(AMyActorRoadLine* TargetLine)
{
    UE_LOG(GeneratorMapLog, Display, TEXT("Function SpawnBarrel called"));
    if (!BlueprintToSpawnStaticObject || !TargetLine) return;

    if (CheckSafeRoad(TargetLine))
    {
        for (int32 i = 0; i < TargetLine->LinePoints.Num(); i++)
        {
            if (BarrelCouner >= MaxBarrelInLine) return;

            if (FMath::RandRange(0, 1) == 0)
            {
                UE_LOG(GeneratorMapLog, Display, TEXT("Spawn barrel"));
                FVector SpawnLoc = TargetLine->LinePoints[i];
                FActorSpawnParameters SpawnParams;
                SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

                AMyActorBaseStaticObject* NewObject = GetWorld()->SpawnActor<AMyActorBaseStaticObject>(BlueprintToSpawnStaticObject,
                    SpawnLoc, FRotator::ZeroRotator, SpawnParams);

                if (NewObject)
                {
                    UE_LOG(GeneratorMapLog, Log, TEXT("BarrelCouner is %b"), BarrelCouner);
                    BarrelCouner++;
                    SpawnedBarrel.Add(NewObject);
                }
            }
        }
    }
}

void AMyActorGeneratorMap::SpawnCharger(AMyActorRoadLine* TargetLine)
{
    UE_LOG(GeneratorMapLog, Display, TEXT("Function SpawnCharger called"));
    if (!BlueprintToSpawnCharger || !TargetLine) return;

    if (CheckSafeRoad(TargetLine))
    {
        for (int32 i = 0; i < TargetLine->LinePoints.Num(); i++)
        {
            if (ChargerCounter == 1) return;

            if (FMath::RandRange(0, 1) == 0)
            {
                UE_LOG(GeneratorMapLog, Display, TEXT("Spawn charger"));
                FVector SpawnLoc = TargetLine->LinePoints[i];
                FActorSpawnParameters SpawnParams;
                SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

                AMyActorCharger* NewObject = GetWorld()->SpawnActor<AMyActorCharger>(BlueprintToSpawnCharger,
                    SpawnLoc, FRotator::ZeroRotator, SpawnParams);

                if (NewObject)
                {
                    UE_LOG(GeneratorMapLog, Log, TEXT("ChargerCounter is %b"), ChargerCounter);
                    ChargerCounter++;
                    SpawnedCharger.Add(NewObject);
                }
            }
        }
    }
}

void AMyActorGeneratorMap::DeleteOldOnbjects(float LineX)
{
    UE_LOG(GeneratorMapLog, Display, TEXT("Function DeleteOldOnbjects called"));

    float Tolerance = 50.f;

    for (int32 i = SpawnedBarrel.Num() - 1; i >= 0; i--)//Clear barrel
    {
        if (!SpawnedBarrel[i]) continue;

        if (FMath::IsNearlyEqual(SpawnedBarrel[i]->GetActorLocation().X, LineX, Tolerance))
        {
            UE_LOG(GeneratorMapLog, Display, TEXT("Delete one barrel"));
            SpawnedBarrel[i]->Destroy();
            SpawnedBarrel.RemoveAt(i);
        }
    }

    for (int32 i = SpawnedCharger.Num() - 1; i >= 0; i--)//Clear charger
    {
        if (!SpawnedCharger[i]) continue;

        if (FMath::IsNearlyEqual(SpawnedCharger[i]->GetActorLocation().X, LineX, Tolerance))
        {
            UE_LOG(GeneratorMapLog, Display, TEXT("Delete one charger"));
            SpawnedCharger[i]->Destroy();
            SpawnedCharger.RemoveAt(i);
        }
    }
}

void AMyActorGeneratorMap::RemoveOldLine()
{
    UE_LOG(GeneratorMapLog, Display, TEXT("Function RemoveOldLine called"));

    if (SpawnedLines.Num() == 0) return;

    AMyActorRoadLine* OldLine = SpawnedLines[0];
    if (!OldLine) return;

    float LineX = OldLine->GetActorLocation().X;

    DeleteOldOnbjects(LineX);
    OldLine->Destroy();
    SpawnedLines.RemoveAt(0);

}

void AMyActorGeneratorMap::SpawnLine()
{
    UE_LOG(GeneratorMapLog, Display, TEXT("Function SpawnLine called"));
    if (RoadLines.Num() == 0) return;
    int32 RandomIndex = 0;
    if (SpawnedLines.Num() == 0)//First road is safe
    {
        RandomIndex = 1;
        SafeCounter++;
    }
    else
    {
        RandomIndex = FMath::RandRange(0, RoadLines.Num() - 1);//Random road
        switch (RandomIndex)//Update counters
        {
        case 0: DangerCounter++; break;
        case 1: SafeCounter++; break;
        case 2: TrainCounter++;  break;
        }

        if (DangerCounter >= 4) { RandomIndex = 1; DangerCounter = 0; }//Check limits

        else if (SafeCounter >= 3){ RandomIndex = 0; SafeCounter = 0;}

        else if (TrainCounter >= 3) { RandomIndex = 0; TrainCounter = 0; }
    }


    UE_LOG(GeneratorMapLog, Log, TEXT("DangerCounter is %d, SafeCounter is %d,TrainCounter is %d"),
        DangerCounter, SafeCounter, TrainCounter);

    TSubclassOf<AMyActorRoadLine> RandomClass = RoadLines[RandomIndex];
    FVector SpawnLocation = GetActorLocation() + FVector(CurrentXLine, 0.f, 0.f);
 
    AMyActorRoadLine* NewLine = GetWorld()->SpawnActor<AMyActorRoadLine>(RandomClass,//Spawn
        SpawnLocation, FRotator::ZeroRotator);

    if (NewLine)
    {
        UE_LOG(GeneratorMapLog, Log, TEXT("Spawn %s road"),*RandomClass);
        int32 GlobalLineIndex = FMath::RoundToInt(CurrentXLine / DistanceBetweenLines);

        NewLine->InitializeRoadLine(GlobalLineIndex);

        SpawnedLines.Add(NewLine);
        CurrentXLine += DistanceBetweenLines;
    }
}

