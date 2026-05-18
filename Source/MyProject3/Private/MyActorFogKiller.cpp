#include "MyActorFogKiller.h"   
#include "MyPawnPlayer.h"     
#include "Kismet/GameplayStatics.h" 


DEFINE_LOG_CATEGORY(FogLog);

AMyActorFogKiller::AMyActorFogKiller()
{
    PrimaryActorTick.bCanEverTick = true;

    Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
    RootComponent = Scene;

    FogParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Fog"));
    FogParticles->SetupAttachment(RootComponent);

    TrigerComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Triger"));
    TrigerComponent->SetupAttachment(FogParticles);

    MovementVector = FVector::ForwardVector;

    Speed = 50.f;
}

void AMyActorFogKiller::BeginPlay()
{
    Super::BeginPlay();


    APawn* BasePawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    Player = Cast<AMyPawnPlayer>(BasePawn);

    if (TrigerComponent)
    {
        TrigerComponent->OnComponentBeginOverlap.AddDynamic(this, &AMyActorFogKiller::OnOverlapBegin);
    }
}

void AMyActorFogKiller::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    MoveFog(DeltaTime);

}


void AMyActorFogKiller::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    UE_LOG(FogLog, Display, TEXT("Object in the fog"));
    if (OtherActor && OtherActor == Player)//Kill player
    {
        Player->PlayerDeath();
    }
}


void AMyActorFogKiller::MoveFog(float DeltaTime)//Move fog
{
    UE_LOG(FogLog, Display, TEXT("Fog moveing"));
    FVector DeltaTranslation = MovementVector * Speed * DeltaTime;
    AddActorWorldOffset(DeltaTranslation, true);
}

