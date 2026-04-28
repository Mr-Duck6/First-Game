


#include "MyActorCharger.h"
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
	
}


void AMyActorCharger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyActorCharger::Charge()
{

}

