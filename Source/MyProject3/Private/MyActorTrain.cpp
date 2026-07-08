#include "MyActorTrain.h"
#include "Components/BoxComponent.h"
#include "MyPawnPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SpotLightComponent.h"

DEFINE_LOG_CATEGORY(TrainLog);

AMyActorTrain::AMyActorTrain()
{
    PrimaryActorTick.bCanEverTick = true;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrainMesh"));
    RootComponent = Mesh;

    BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
    BoxCollision->SetupAttachment(RootComponent);

    TrainLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("TrainLight"));
    TrainLight->SetupAttachment(RootComponent);

    bCanMove = false;
    MoveDirection = FVector::ZeroVector;
}

void AMyActorTrain::BeginPlay()
{
    Super::BeginPlay();

    APawn* BasePawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    PlayerRef = Cast<AMyPawnPlayer>(BasePawn);

    StartLocation = GetActorLocation();

        BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AMyActorTrain::OnOverlapBegin);
}

void AMyActorTrain::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bCanMove)
    {
        FVector NewLocation = GetActorLocation() + (MoveDirection * Speed * DeltaTime);

        SetActorLocation(NewLocation, false);

        if (FVector::Dist(StartLocation, NewLocation) >= MaxDriveDistance)//Train moving
        {
            bCanMove = false;
            SetActorLocation(StartLocation, false);
        }
    }
}

void AMyActorTrain::StartMove()
{
    UE_LOG(TrainLog, Display, TEXT("Function StartMove called"));
    bCanMove = true;
    UE_LOG(TrainLog, Log, TEXT("bCanMove is %s"), bCanMove ? TEXT("true") : TEXT("false"));
}

void AMyActorTrain::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)//Kill player
{
    if (OtherActor && OtherActor != this)
    {
        AMyPawnPlayer* Player = Cast<AMyPawnPlayer>(OtherActor);
        if (Player)
        {
            UE_LOG(TrainLog, Display, TEXT("Function OnOverlapBegin called"));
            PlayerRef->PlayerDeath();
        }
    }
}