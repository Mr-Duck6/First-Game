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

    CarTargetSpeed = 0.f;
    CarCurrentSpeed = 0.f;
    bCanMove = false;

    StartDelayTimer = FMath::FRandRange(0.f, 2.0f);
}

void AMyActorBaseCar::BeginPlay()
{
    Super::BeginPlay();
    StartLocation = GetActorLocation();
}

void AMyActorBaseCar::InitializeCar(FVector Direction, float InSpeed, FVector SpawnLocation)//Edit car
{
    MoveDirection = Direction;
    CarTargetSpeed = InSpeed;
    CarCurrentSpeed = InSpeed;
    Speed = InSpeed;
}

void AMyActorBaseCar::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bCanMove)//Teleport car
    {
        StartDelayTimer -= DeltaTime;

        if (StartDelayTimer <= 0.f)
        {
            bCanMove = true;
        }
        return;
    }
    FVector StartCheck = GetActorLocation();
    FVector EndCheck = StartCheck + (MoveDirection * 350.f);
    FHitResult HitResult;

    FCollisionQueryParams ObjectParams;
    ObjectParams.AddIgnoredActor(this);


    bool bObjectInFront = GetWorld()->LineTraceSingleByChannel(HitResult, StartCheck, 
        EndCheck, ECC_WorldDynamic, ObjectParams);

    if (bObjectInFront && HitResult.GetActor() && HitResult.GetActor()->IsA(AMyActorBaseCar::StaticClass()))//Stoped
    {
        CarCurrentSpeed = FMath::FInterpTo(CarCurrentSpeed, 0.f, DeltaTime, 5.f);
    }
    else//Speed up
    {
        CarCurrentSpeed = FMath::FInterpTo(CarCurrentSpeed, CarTargetSpeed, DeltaTime, 2.f);
    }

    FVector NewLocation = GetActorLocation() + (MoveDirection * CarCurrentSpeed * DeltaTime);//Move
    SetActorLocation(NewLocation, true);

    if (FVector::Dist(StartLocation, NewLocation) >= MaxDriveDistance)//End road
    {
        bCanMove = false;
        StartDelayTimer = FMath::FRandRange(0.5f, 2.0f);
        SetActorLocation(StartLocation, false);
    }
}

void AMyActorBaseCar::MoveCar(float DeltaTime) {}

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