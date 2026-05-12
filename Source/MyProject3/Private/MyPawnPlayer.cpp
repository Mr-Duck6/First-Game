


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
#include "MyActorCharger.h"
#include "MyGameModeBase.h"


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

    //Variables
    CurrentEnergy = 100.0f;
    MaxEnergy = 100.0f;
    CellX = 4;
    bCanMove = false;
    IsDead = false;
    IsCharging = false;
    IsMoveing = false;
    MaxEnergy = 100;
    StartHit = FVector(0, 0, 50);
    SpendEnergy = 0.3;

    PlayerMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    PlayerMeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
}


void AMyPawnPlayer::BeginPlay()
{
	Super::BeginPlay();

    GetWorld()->GetTimerManager().SetTimer(EnergyTimer, this, &AMyPawnPlayer::UpdateEnergy, 0.05, true, 0.f);//Energy timer


    MapGenerator = Cast<AMyActorGeneratorMap>(UGameplayStatics::GetActorOfClass(GetWorld(), AMyActorGeneratorMap::StaticClass()));

    if (MapGenerator)
    {
        if (MapGenerator->SpawnedLines.Num() > 0)//Current line
        {
            CurrentLine = MapGenerator->SpawnedLines[0];
        }

        if (CurrentLine)//Trget location
        {
            TargetLocation = CurrentLine->GetCellLocation(CellX);
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

    //Bind
    PlayerInputComponent->BindAction("MoveForward", IE_Pressed, this, &AMyPawnPlayer::MoveForward);
    PlayerInputComponent->BindAction("MoveBackward", IE_Pressed, this, &AMyPawnPlayer::MoveBackward);
    PlayerInputComponent->BindAction("MoveLeft", IE_Pressed, this, &AMyPawnPlayer::MoveLeft);
    PlayerInputComponent->BindAction("MoveRight", IE_Pressed, this, &AMyPawnPlayer::MoveRight);
}


void AMyPawnPlayer::MoveForward()//Move forward
{
    if (!MapGenerator || !CurrentLine) return;

    CanMove(FVector(0, 100, 0));
    PlayerMeshComponent->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
    if (bCanMove)
    {
        int32 CurrentIndex = MapGenerator->SpawnedLines.IndexOfByKey(CurrentLine);
        if (CurrentIndex == INDEX_NONE) return;

        int32 NextIndex = CurrentIndex + 1;

        if (!MapGenerator->SpawnedLines.IsValidIndex(NextIndex)) return;

        CurrentLine = MapGenerator->SpawnedLines[NextIndex];

        TargetLocation = CurrentLine->GetCellLocation(CellX);

            SetActorLocation(TargetLocation);
    }
}

void AMyPawnPlayer::MoveBackward()//Move backward
{
    if (!MapGenerator || !CurrentLine) return;

    int32 CurrentIndex = MapGenerator->SpawnedLines.IndexOfByKey(CurrentLine);
    PlayerMeshComponent->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
    CanMove(FVector(0, -100, 0));

    if (bCanMove)
    {
        if (CurrentIndex == INDEX_NONE) return;

        int32 PrevIndex = CurrentIndex - 1;

        if (!MapGenerator->SpawnedLines.IsValidIndex(PrevIndex)) return;

        CurrentLine = MapGenerator->SpawnedLines[PrevIndex];

        TargetLocation = CurrentLine->GetCellLocation(CellX);
        SetActorLocation(TargetLocation);
    }
}

void AMyPawnPlayer::MoveLeft()//Move left
{
    if (!MapGenerator || !CurrentLine) return;

    CanMove(FVector(100, 0, 0));

    if (bCanMove)
    {
        PlayerMeshComponent->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

        int32 NextCell = CellX + 1;

        if (!CurrentLine->LinePoints.IsValidIndex(NextCell)) return;

        CellX = NextCell;

        TargetLocation = CurrentLine->GetCellLocation(CellX);
        SetActorLocation(TargetLocation);
    }
}



void AMyPawnPlayer::MoveRight()//Move right
{
    if (!MapGenerator || !CurrentLine) return;

    CanMove(FVector(-100, 0, 0));

    if (bCanMove)
    {

        PlayerMeshComponent->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));

        int32 NextCell = CellX - 1;

        if (!CurrentLine->LinePoints.IsValidIndex(NextCell)) return;

        CellX = NextCell;

        TargetLocation = CurrentLine->GetCellLocation(CellX);
        SetActorLocation(TargetLocation);
    }

}

void AMyPawnPlayer::UpdateEnergy()//Update energy
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

    if (CurrentEnergy <= 0.f)
    {
        CurrentEnergy = 0.f;  
        PlayerDeath();
        return;               
    }
}

void AMyPawnPlayer::PlayerDeath()//Player death
{
    UE_LOG(LogTemp, Error, TEXT("PLAYER DEATH CALLED"));
    if (IsDead) return;
    IsDead = true;

    GetWorldTimerManager().ClearTimer(EnergyTimer);

    UMySaveGame* MySG =Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Slot1"), 0));//Save

    if (!MySG)
    {
        MySG = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
    }

    if (MySG)
    {
        if (CurrentTime > MySG->MaxLifeTime)
        {
            MySG->MaxLifeTime = CurrentTime;
            UGameplayStatics::SaveGameToSlot(MySG, TEXT("Slot1"), 0);
        }
    }

    APlayerController* PC = Cast<APlayerController>(GetController());//Take control
    if (PC)
    {
        DisableInput(PC);
        PC->bShowMouseCursor = true;
        PC->SetInputMode(FInputModeUIOnly());
    }

    
    GetWorld()->GetTimerManager().SetTimer(TimerAfterDead, this, &AMyPawnPlayer::Restart, 10, false);

}


/*
void AMyPawnPlayer::Restart()
{
    UE_LOG(LogTemp, Error, TEXT("RESTART FUNCTION CALLED"));
    UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenu"));

}
*/

void AMyPawnPlayer::CanMove(FVector Direction)//Can move?
{
    FHitResult HitResult;
    FVector LocalStart = GetActorLocation() + FVector(0, 0, 50);
    FVector LocalEnd = LocalStart + Direction;

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, LocalStart, LocalEnd, ECC_Visibility, Params);//Trace

    if (bHit)
    {
        if (HitResult.GetActor() && HitResult.GetActor()->IsA(AMyActorCharger::StaticClass()))
        {
            bCanMove = true;
        }
        else
        {
            bCanMove = false;
        }
    }
    else
    {
        bCanMove = true;
    }
    DrawDebugLine(GetWorld(), LocalStart, LocalEnd, bHit ? FColor::Red : FColor::Green, false, 1.0f, 0, 2.0f);

}

