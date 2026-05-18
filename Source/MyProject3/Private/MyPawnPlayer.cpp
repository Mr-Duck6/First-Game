


#include "MyPawnPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "MyActorRoadLine.h"
#include "MyGameState.h"
#include "MySaveGame.h"
#include "MyActorGeneratorMap.h"
#include "UTimerComponent.h"
#include "MyActorCharger.h"
#include "MyGameModeBase.h"

DEFINE_LOG_CATEGORY(PlayerLog);

AMyPawnPlayer::AMyPawnPlayer()
{

	PrimaryActorTick.bCanEverTick = true;
	
    //Components
    SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
    RootComponent = SceneComponent;


    PlayerMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMeshComponent"));
    PlayerMeshComponent->SetupAttachment(SceneComponent);

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    SpringArmComponent->SetupAttachment(SceneComponent);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(SpringArmComponent);

    //Energy
    CurrentEnergy = 100.0f;
    MaxEnergy = 100.0f;
    bIsCharging = false;
    SpendEnergy = 0.3;
    ChargeEnergy = 0.5;

    //Map
    CellX = 4;
    bCanMove = false;
    bIsDead = false;

    //Moveming
    bIsMoveing = false;
    StartHit = FVector(0, 0, 50);


    PlayerMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);//Collision
    PlayerMeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
}


void AMyPawnPlayer::BeginPlay()
{
	Super::BeginPlay();

    GetWorld()->GetTimerManager().SetTimer(EnergyTimer, this, &AMyPawnPlayer::UpdateEnergy, 0.05, true, 0.f);//Energy timer


    MapGenerator = Cast<AMyActorGeneratorMap>(UGameplayStatics::GetActorOfClass(GetWorld(), AMyActorGeneratorMap::StaticClass()));

    if (MapGenerator)//Set start location
    {
        if (MapGenerator->SpawnedLines.Num() > 0)//Current line is first line
        {
            CurrentLine = MapGenerator->SpawnedLines[0];
            UE_LOG(PlayerLog, Log, TEXT("CurrentLine location is %s"), *CurrentLine->GetActorLocation().ToString());
        }

        if (CurrentLine)//Player start position
        {
            TargetLocation = CurrentLine->GetCellLocation(CellX);
            SetActorLocation(TargetLocation);
            UE_LOG(PlayerLog, Log, TEXT("Player location is %s"), *TargetLocation.ToString());
        }
    }


}

void AMyPawnPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyPawnPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    //Bind
    PlayerInputComponent->BindAction("MoveForward", IE_Pressed, this, &AMyPawnPlayer::MoveForward);
    PlayerInputComponent->BindAction("MoveBackward", IE_Pressed, this, &AMyPawnPlayer::MoveBackward);
    PlayerInputComponent->BindAction("MoveLeft", IE_Pressed, this, &AMyPawnPlayer::MoveLeft);
    PlayerInputComponent->BindAction("MoveRight", IE_Pressed, this, &AMyPawnPlayer::MoveRight);
}


void AMyPawnPlayer::MoveForward()
{
    UE_LOG(PlayerLog, Display, TEXT("Function MoveForward called"));

    if (!MapGenerator || !CurrentLine) return;

    CanMove(FVector(100.f, 0.f, 0.f)); 
    PlayerMeshComponent->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));

    if (bCanMove)
    {
        int32 CurrentIndex = INDEX_NONE;

        if (MapGenerator->SpawnedLines.Find(CurrentLine, CurrentIndex))
        {
            int32 NextIndex = CurrentIndex + 1;//Next line

            if (!MapGenerator->SpawnedLines.IsValidIndex(NextIndex)) return;

            CurrentLine = MapGenerator->SpawnedLines[NextIndex];//Set new location
            TargetLocation = CurrentLine->GetCellLocation(CellX);
            SetActorLocation(TargetLocation);
            UE_LOG(PlayerLog, Log, TEXT("New player location is %s"), *TargetLocation.ToString());
        }
    }
}

void AMyPawnPlayer::MoveBackward()
{
    UE_LOG(PlayerLog, Display, TEXT("Function MoveBackward called"));
    if (!MapGenerator || !CurrentLine) return;

    CanMove(FVector(-100.f, 0.f, 0.f));
    PlayerMeshComponent->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));

    if (bCanMove)
    {
        int32 CurrentIndex = INDEX_NONE;
        if (MapGenerator->SpawnedLines.Find(CurrentLine, CurrentIndex))
        {
            int32 PrevIndex = CurrentIndex - 1;//Next line

            if (!MapGenerator->SpawnedLines.IsValidIndex(PrevIndex)) return;

            CurrentLine = MapGenerator->SpawnedLines[PrevIndex];//Set new location
            TargetLocation = CurrentLine->GetCellLocation(CellX);
            SetActorLocation(TargetLocation);
            UE_LOG(PlayerLog, Log, TEXT("New player location is %s"), *TargetLocation.ToString());
        }
    }
}

void AMyPawnPlayer::MoveLeft()//Move left
{
    UE_LOG(PlayerLog, Display, TEXT("Function MoveLeft called"));
    if (!MapGenerator || !CurrentLine) return;

    CanMove(FVector(0, -100, 0));

    if (bCanMove)
    {
        PlayerMeshComponent->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

        int32 NextCell = CellX -1;

        if (!CurrentLine->LinePoints.IsValidIndex(NextCell)) return;

        CellX = NextCell;
        TargetLocation = CurrentLine->GetCellLocation(CellX);
        SetActorLocation(TargetLocation);
        UE_LOG(PlayerLog, Log, TEXT("New player location is %s"), *TargetLocation.ToString());
    }
}



void AMyPawnPlayer::MoveRight()//Move right
{
    UE_LOG(PlayerLog, Display, TEXT("Function MoveRight called"));
    if (!MapGenerator || !CurrentLine) return;

    CanMove(FVector(0, 100, 0));

    if (bCanMove)
    {
        UE_LOG(PlayerLog, Log, TEXT("bCanMove is %s"), bCanMove ? TEXT("true") : TEXT("false"));
        PlayerMeshComponent->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));

        int32 NextCell = CellX + 1;

        if (!CurrentLine->LinePoints.IsValidIndex(NextCell)) return;

        CellX = NextCell;
        TargetLocation = CurrentLine->GetCellLocation(CellX);
        SetActorLocation(TargetLocation);
        UE_LOG(PlayerLog, Log, TEXT("New player location is %s"), *TargetLocation.ToString());
    }

}

void AMyPawnPlayer::UpdateEnergy()//Update energy
{
    if (bIsDead) return;

    UE_LOG(PlayerLog, Log, TEXT("bIsDead is %s, bIsCharging is %s, CurrentEnergy is %f"),
        bIsDead ? TEXT("true") : TEXT("false"),
        bIsCharging ? TEXT("true") : TEXT("false"),
        CurrentEnergy);

    if (!bIsCharging)//Spend energy
    {
        CurrentEnergy -= SpendEnergy;
    }
    else//Charging
    {
        CurrentEnergy += ChargeEnergy;
    }

    CurrentEnergy = FMath::Clamp(CurrentEnergy, 0.f, MaxEnergy);//Limit

    if (CurrentEnergy <= 0.f)//Death
    {
        CurrentEnergy = 0.f;  
        PlayerDeath();
        return;               
    }
}

void AMyPawnPlayer::PlayerDeath()//Player death
{
    UE_LOG(LogTemp, Log, TEXT("Player is dead"));
    if (bIsDead) return;
    bIsDead = true;

    UTimerComponent* TimerComp = FindComponentByClass<UTimerComponent>();//Stop timer
    if (TimerComp)
    {
        TimerComp->StopAndCheckRecord();
    }

    GetWorldTimerManager().ClearTimer(EnergyTimer);
    GetWorld()->GetTimerManager().SetTimer(TimerAfterDead, this, &AMyPawnPlayer::Restart, 10, false);

}


void AMyPawnPlayer::CanMove(FVector Direction)//Can move?
{
    UE_LOG(PlayerLog,Display,TEXT("Function CanMove called"))

    FHitResult HitResult;
    FVector LocalStart = GetActorLocation() + FVector(0, 0, 50);
    FVector LocalEnd = LocalStart + Direction;

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, LocalStart, LocalEnd, ECC_Visibility, Params);

    if (bHit)
    {
        UE_LOG(PlayerLog, Log, TEXT("bHit is %s"), bHit ? TEXT("true") : TEXT("false"));
        AActor* HitActor = HitResult.GetActor();

        if (HitActor)
        {
            UE_LOG(PlayerLog, Warning, TEXT(" %s "), *HitActor->GetName());
        }

        if (HitActor && HitActor->IsA(AMyActorCharger::StaticClass()))
        {
            bCanMove = true;
            bIsCharging = true;
        }
        else
        {
            bCanMove = false;
        }
    }
    else
    {
        bCanMove = true;

        bIsCharging = false;
    }

    UE_LOG(PlayerLog, Log, TEXT("bCanMove is %s"), bCanMove ? TEXT("true") : TEXT("false"));
    UE_LOG(PlayerLog, Log, TEXT("bIsCharging is %s"), bIsCharging ? TEXT("true") : TEXT("false"));
    DrawDebugLine(GetWorld(), LocalStart, LocalEnd, bHit ? FColor::Red : FColor::Green, false, 1.0f, 0, 2.0f);

}