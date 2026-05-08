


#include "MyPawnPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "MyActorRoadLine.h"
#include "MyActorGeneratorMap.h"


AMyPawnPlayer::AMyPawnPlayer()
{

	PrimaryActorTick.bCanEverTick = true;
	
    SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
    RootComponent = SceneComponent;


    PlayerMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMeshComponent"));
    PlayerMeshComponent->SetupAttachment(SceneComponent);

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    SpringArmComponent->SetupAttachment(PlayerMeshComponent);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(SpringArmComponent);

    CurrentEnergy = 100.0f;

}


void AMyPawnPlayer::BeginPlay()
{
	Super::BeginPlay();



    GetWorld()->GetTimerManager().SetTimer(EnergyTimer, this, &AMyPawnPlayer::UpdateEnergy, 0.05, true, 0.f);

    MapGenerator = Cast<AMyActorGeneratorMap>(UGameplayStatics::GetActorOfClass(GetWorld(), AMyActorGeneratorMap::StaticClass()));

    if (MapGenerator)
    {

        CurrentLineIndex = 0;
        CurrentLine = MapGenerator->GetLine(CurrentLineIndex);

        if (CurrentLine)
        {
            TargetLocation = CurrentLine->GetActorLocation();
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
    if (!MapGenerator || !CurrentLine)
    {
        return;
    }

    int32 NextIndex = CurrentLineIndex + 1;

    AMyActorRoadLine* NextLine =MapGenerator->GetLine(NextIndex);

    if (NextLine)
    {
        CurrentLine = NextLine;
        CurrentLineIndex = NextIndex;
    }
}

void AMyPawnPlayer::MoveBackward()
{

}

void AMyPawnPlayer::MoveLeft()
{

}

void AMyPawnPlayer::MoveRight()
{

}

void AMyPawnPlayer::UpdateEnergy()
{
    if (CurrentEnergy <=0)
    {
        PlayerDeath();
    }
    else if (!IsCharging)
    {
        CurrentEnergy -= SpendEnergy;
    }
    else
    {
        CurrentEnergy += SpendEnergy;
    }

}

void AMyPawnPlayer::PlayerDeath()
{

}


