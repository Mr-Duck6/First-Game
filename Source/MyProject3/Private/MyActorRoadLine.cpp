#include "MyActorRoadLine.h"
#include "CoreMinimal.h"
#include "MyActorRoadLine.h"
#include "MyActorTrain.h"
#include "DrawDebugHelpers.h"
#include "MyActorLamp.h"
#include "MyActorBaseCar.h"
#include "Components/PointLightComponent.h"

DEFINE_LOG_CATEGORY(RoadLineLog);

AMyActorRoadLine::AMyActorRoadLine()
{
    PrimaryActorTick.bCanEverTick = true;
    RoadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RoadMesh"));
    RootComponent = RoadMesh;

    LineIndex = 0;
    LineLength = 11;
    CellSize = 100.f;
    StartLocationY = -500.f;
    StartCarYLocation = 650;
    MainRoadDirection = FVector::RightVector;

    YSpawn = -600;
}

void AMyActorRoadLine::BeginPlay()
{
    Super::BeginPlay();
    MainRoadDirection = FMath::RandBool() ? MainRoadDirection : -MainRoadDirection;//Choce road direction
}

void AMyActorRoadLine::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AMyActorRoadLine::GenerateCells()//Create grid
{
    UE_LOG(RoadLineLog, Display, TEXT("Function GenerateCells called"));
    LinePoints.Empty();

    float CurrentXLine = GetActorLocation().X;
    float CurrentZ = GetActorLocation().Z;

    for (int i = 0; i < LineLength; i++)
    {
        FVector CellLocation = FVector(CurrentXLine, StartLocationY, CurrentZ);
        LinePoints.Add(CellLocation);

        UE_LOG(RoadLineLog, Display, TEXT("Create point %s"), *CellLocation.ToString());
        StartLocationY += CellSize;
        DrawDebugPoint(GetWorld(), CellLocation, 5.f, FColor::Green, false, -1);
    }
}

FVector AMyActorRoadLine::GetCellLocation(int32 Index)//Getter
{
    UE_LOG(RoadLineLog, Display, TEXT("Function GetCellLocation called"));
    if (!LinePoints.IsValidIndex(Index)) return FVector::ZeroVector;
    return LinePoints[Index];
}


void AMyActorRoadLine::SpawnCar()//Car
{
    UE_LOG(RoadLineLog, Display, TEXT("Function SpawnCar called"));
    if (BluePrintToSpawnCar.Num() == 0 || LinePoints.Num() == 0 || CarsCases.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("Array is empty"));
        return;
    }
        FActorSpawnParameters SpawnParams;
        int32 CaseNum = FMath::RandRange(0, CarsCases.Num()-1);

        if (BluePrintToSpawnCar.IsValidIndex(CarsCases[CaseNum]->CarsType))
        {
            UE_LOG(RoadLineLog, Display, TEXT("Car Array is valid"));
            TSubclassOf<AMyActorBaseCar> ClassCarToSpawn = BluePrintToSpawnCar[CarsCases[CaseNum]->CarsType];
            for (int i = 0; i < CarsCases[CaseNum]->CarsOnTheRoad; i++)
            {
                TSubclassOf<AMyActorBaseCar> ClassToSpawn = BluePrintToSpawnCar[CarsCases[CaseNum]->CarsType];
                FVector SpawnLocation = GetActorLocation() - FVector (0,StartCarYLocation,0);
                FRotator SpawnRoation = FRotator::ZeroRotator + FRotator(0, 270, 0);

                AMyActorBaseCar* NewCar = GetWorld()->SpawnActor<AMyActorBaseCar>(ClassToSpawn, SpawnLocation, SpawnRoation, SpawnParams);
                NewCar->Speed = CarsCases[CaseNum]->Speed;
                if (NewCar)
                {
                    UE_LOG(RoadLineLog, Display, TEXT("First car spawn"));
                    NewCar->Speed = CarsCases[CaseNum]->Speed;
                    SpawnedCars.Add(NewCar);
                    YSpawn = YSpawn - 50;
                }
            }
        }

      
}
    

//////////////////////
void AMyActorRoadLine::SpawnLamp(int32 InLineIndex)
{
    UE_LOG(RoadLineLog, Display, TEXT("Function SpawnLamp called"));

    if (InLineIndex % 2 != 0) return;
    if (BluePrintToSpawnLamp.Num() == 0) return;

    TSubclassOf<AMyActorLamp> ClassToSpawn = BluePrintToSpawnLamp[0];

    if (!ClassToSpawn) return;

    float LampOffsets[2] = {-100, 000 };//Coridnate

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    for (int i = 0; i < 1; i++)//Spawn
    {
        AMyActorLamp* NewLamp = GetWorld()->SpawnActor<AMyActorLamp>(ClassToSpawn, FVector::ZeroVector, 
            FRotator::ZeroRotator, SpawnParams);
        if (NewLamp)
        {
            NewLamp->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
            NewLamp->SetActorRelativeLocation(FVector(0.f, LampOffsets[i], 200.f));
            SpawnedLamps.Add(NewLamp);

            UE_LOG(RoadLineLog, Display, TEXT("Spawn lamp"));
        }
    }

}
//////////////////////////

void AMyActorRoadLine::DeleteCars()
{
    UE_LOG(RoadLineLog, Display, TEXT("Function DeleteCars called"));
    for (AMyActorBaseCar* Car : SpawnedCars)
    {
        if (Car) Car->Destroy();
    }
    SpawnedCars.Empty();
}

void AMyActorRoadLine::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    UE_LOG(RoadLineLog, Display, TEXT("Function EndPlay called"));

    Super::EndPlay(EndPlayReason);
    GetWorld()->GetTimerManager().ClearTimer(TrainCycleTimer);
    GetWorld()->GetTimerManager().ClearTimer(WarningLightTimer);

    if (SpawnedTrain) SpawnedTrain->Destroy();
    for (AMyActorBaseCar* Car : SpawnedCars) if (Car) Car->Destroy();
    for (AMyActorLamp* Lamp : SpawnedLamps) if (Lamp) Lamp->Destroy();
}


void AMyActorRoadLine::InitializeRoadLine(int32 InLineIndex)
{
    UE_LOG(RoadLineLog, Display, TEXT("Function InitializeRoadLine called"));

    LineIndex = InLineIndex;
    GenerateCells();
    SpawnLamp(InLineIndex); 

    if (TrainClass)//Train road
    {
        UE_LOG(RoadLineLog, Display, TEXT("Is train line"));
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        FVector TrainSpawnLoc;

        if (MainRoadDirection == FVector(0, 1, 0))
        {
            TrainSpawnLoc = GetActorLocation() - FVector(0.f, 2000.f, 0.f);
        }
        else
        {
            TrainSpawnLoc = GetActorLocation() + FVector(0.f, 2000.f, 0.f);
        }
        FRotator TrainRot = GetActorRotation();

        SpawnedTrain = GetWorld()->SpawnActor<AMyActorTrain>(TrainClass, TrainSpawnLoc, TrainRot, SpawnParams);
        if (SpawnedTrain)
        {
            UE_LOG(RoadLineLog, Display, TEXT("Spawn train"));
            SpawnedTrain->MoveDirection = MainRoadDirection;
            SpawnedTrain->StartLocation = TrainSpawnLoc;

            float AttackCooldown = FMath::RandRange(9, 11);
            GetWorld()->GetTimerManager().SetTimer(TrainCycleTimer, this,
                &AMyActorRoadLine::PlanTrainAttack, AttackCooldown, true);
        }
    }
    else
    {
        if (BluePrintToSpawnCar.Num() > 0)
        {
            SpawnCar();
        }
    }
}

void AMyActorRoadLine::TriggerTrainMove()//Start move
{
    UE_LOG(RoadLineLog, Display, TEXT("Function TriggerTrainMove called "));
    if (SpawnedTrain)
    {
        SpawnedTrain->StartMove();
    }
}

void AMyActorRoadLine::PlanTrainAttack()//Edit red lamp
{
    UE_LOG(RoadLineLog, Display, TEXT("Function PlanTrainAttack called "));
    GetWorld()->GetTimerManager().SetTimer(WarningLightTimer, this,
        &AMyActorRoadLine::TriggerTrainMove, 3.f, false);
}