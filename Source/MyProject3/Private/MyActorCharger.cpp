
#include "MyActorCharger.h"
#include "Kismet/GameplayStatics.h"
#include "MyPawnPlayer.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

AMyActorCharger::AMyActorCharger()
{

	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	ChargerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChargerMesh"));
	BoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));

	ChargerMesh->SetupAttachment(Scene);
	BoxTrigger->SetupAttachment(ChargerMesh);

}


void AMyActorCharger::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<AMyPawnPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}


void AMyActorCharger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Charge(Player->IsCharging);
}

void AMyActorCharger::Charge(bool IsCharging)
{
	if (IsCharging && Player)
	{
		Player->CurrentEnergy = +ChargeSpeed;
	}
}


void AMyActorCharger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	Player->IsCharging = true;
}

void  AMyActorCharger::OnOverlapEnd(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	Player->IsCharging = false;
}

