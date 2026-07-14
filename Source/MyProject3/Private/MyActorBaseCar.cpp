#include "MyActorBaseCar.h"
#include "Components/BoxComponent.h"
#include "MyPawnPlayer.h"
#include "DrawDebugHelpers.h"

AMyActorBaseCar::AMyActorBaseCar()
{
    PrimaryActorTick.bCanEverTick = true;

    CarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CarMesh"));
    RootComponent = CarMesh;

    BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
    BoxCollision->SetupAttachment(RootComponent);

    BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AMyActorBaseCar::OnOverlapBegin);

    MaxDriveDistance = 1300;
    MoveDirection = FVector::RightVector;

}

void AMyActorBaseCar::BeginPlay()
{
    Super::BeginPlay();
    StartLocation = GetActorLocation();

}

void AMyActorBaseCar::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FCollisionQueryParams ObjectParams;
    ObjectParams.AddIgnoredActor(this);

    FVector NewLocation = GetActorLocation() + (MoveDirection * Speed * DeltaTime);//Move
    SetActorLocation(NewLocation, true);

    if (FVector::Dist(StartLocation, NewLocation) >= MaxDriveDistance)
    {
        SetActorLocation(StartLocation, false);
    }
}


void AMyActorBaseCar::OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{

    if (OtherActor && OtherActor->IsA(AMyPawnPlayer::StaticClass()))//Kill player
    {
        AMyPawnPlayer* PlayerActor = Cast<AMyPawnPlayer>(OtherActor);
        if (PlayerActor)
        {
            PlayerActor->PlayerDeath();
        }
    }
}
