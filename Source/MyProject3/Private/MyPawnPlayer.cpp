


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


AMyPawnPlayer::AMyPawnPlayer()
{

	PrimaryActorTick.bCanEverTick = true;
	
    SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
    RootComponent = SceneComponent;


    PlayerMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMeshComponent"));
    PlayerMeshComponent->SetupAttachment(SceneComponent);

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    SpringArmComponent->SetupAttachment(SceneComponent);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(SpringArmComponent);

    CurrentEnergy = 100.0f;
    CellX = 4;

}


void AMyPawnPlayer::BeginPlay()
{
	Super::BeginPlay();

    GetWorld()->GetTimerManager().SetTimer(EnergyTimer, this, &AMyPawnPlayer::UpdateEnergy, 0.05, true, 0.f);//Energy timer

    GetWorld()->GetTimerManager().SetTimer(RecordTimer, this, &AMyPawnPlayer::UpdateTime, 1, true, 0.f);//Record timer


    MapGenerator = Cast<AMyActorGeneratorMap>(UGameplayStatics::GetActorOfClass(GetWorld(), AMyActorGeneratorMap::StaticClass()));

    if (MapGenerator)
    {
        if (MapGenerator->SpawnedLines.Num() > 0)//Current line
        {
            CurrentLine = MapGenerator->SpawnedLines[0];
        }

        UE_LOG(LogTemp,Log,TEXT("CurrentLine is %s"),CurrentLine)
        if (CurrentLine)//Trget location
        {
            TargetLocation =CurrentLine->GetCellLocation(CellX);
            SetActorLocation(TargetLocation);
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

    PlayerInputComponent->BindAction("MoveForward", IE_Pressed, this, &AMyPawnPlayer::MoveForward);
    PlayerInputComponent->BindAction("MoveBackward", IE_Pressed, this, &AMyPawnPlayer::MoveBackward);
    PlayerInputComponent->BindAction("MoveLeft", IE_Pressed, this, &AMyPawnPlayer::MoveLeft);
    PlayerInputComponent->BindAction("MoveRight", IE_Pressed, this, &AMyPawnPlayer::MoveRight);
}


void AMyPawnPlayer::MoveForward()
{
    if (!MapGenerator || !CurrentLine) return;

    int32 CurrentIndex = MapGenerator->SpawnedLines.IndexOfByKey(CurrentLine);
    if (CurrentIndex == INDEX_NONE) return;

    int32 NextIndex = CurrentIndex + 1;

    if (!MapGenerator->SpawnedLines.IsValidIndex(NextIndex)) return;

    CurrentLine = MapGenerator->SpawnedLines[NextIndex];

    TargetLocation = CurrentLine->GetCellLocation(CellX);
    SetActorLocation(TargetLocation);
}

void AMyPawnPlayer::MoveBackward()
{
    if (!MapGenerator || !CurrentLine) return;

    int32 CurrentIndex = MapGenerator->SpawnedLines.IndexOfByKey(CurrentLine);

    if (CurrentIndex == INDEX_NONE) return;

    int32 PrevIndex = CurrentIndex - 1;

    if (!MapGenerator->SpawnedLines.IsValidIndex(PrevIndex)) return;

    CurrentLine = MapGenerator->SpawnedLines[PrevIndex];

    TargetLocation = CurrentLine->GetCellLocation(CellX);
    SetActorLocation(TargetLocation);
}

void AMyPawnPlayer::MoveLeft()
{
    if (!MapGenerator || !CurrentLine) return;

    PlayerMeshComponent->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

    int32 NextCell = CellX + 1;

    if (!CurrentLine->LinePoints.IsValidIndex(NextCell)) return;

    CellX = NextCell;

    TargetLocation = CurrentLine->GetCellLocation(CellX);
    SetActorLocation(TargetLocation);
}



void AMyPawnPlayer::MoveRight()
{
    if (!MapGenerator || !CurrentLine) return;

    PlayerMeshComponent->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));

    int32 NextCell = CellX - 1;

    if (!CurrentLine->LinePoints.IsValidIndex(NextCell)) return;

    CellX = NextCell;

    TargetLocation = CurrentLine->GetCellLocation(CellX);
    SetActorLocation(TargetLocation);
}

void AMyPawnPlayer::UpdateEnergy()//Energy
{
    if (IsDead) return;

    if (!IsCharging)
    {
        CurrentEnergy -= SpendEnergy;
    }
    else
    {
        CurrentEnergy += SpendEnergy;
    }

    CurrentEnergy = FMath::Clamp(CurrentEnergy, 0.f, MaxEnergy);

    UE_LOG(LogTemp, Warning, TEXT("Energy = %f"), CurrentEnergy);

    if (CurrentEnergy <= 0.f)
    {
        PlayerDeath();
    }
}

void AMyPawnPlayer::PlayerDeath()
{
    UE_LOG(LogTemp, Error, TEXT("PLAYER DEATH CALLED"));
    if (IsDead) return;

    IsDead = true;

    UE_LOG(LogTemp, Error, TEXT("PLAYER DEAD"));

    GetWorldTimerManager().ClearTimer(EnergyTimer);
    GetWorldTimerManager().ClearTimer(RecordTimer);

    AMyGameState* MyGS =
        Cast<AMyGameState>(GetWorld()->GetGameState());

    UMySaveGame* MySG =Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Slot1"), 0));

    if (!MySG)
    {
        MySG = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
    }

    if (MyGS && MySG)
    {
        if (CurrentTime > MySG->MaxLifeTime)
        {
            MySG->MaxLifeTime = CurrentTime;

            UGameplayStatics::SaveGameToSlot(MySG,TEXT("Slot1"),0);
        }

        MyGS->RestartLevel();
    }
}

void  AMyPawnPlayer::UpdateTime()//Update current time
{
    if (IsDead)
    {
        return;
    }
    CurrentTime++;
}
