
#include "MyActorCharger.h"
#include "Kismet/GameplayStatics.h"
#include "MyPawnPlayer.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/BoxComponent.h"

AMyActorCharger::AMyActorCharger()
{

	PrimaryActorTick.bCanEverTick = false;


	//Components
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	ChargerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChargerMesh"));
	LampMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LampMesh"));
	BoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));
	Light = CreateDefaultSubobject<USpotLightComponent>(TEXT("Light"));

	ChargerMesh->SetupAttachment(Scene);
	BoxTrigger->SetupAttachment(ChargerMesh);
	LampMesh->SetupAttachment(Scene);
	Light->SetupAttachment(LampMesh);
	
	//Triger
	BoxTrigger->OnComponentBeginOverlap.AddDynamic(this, &AMyActorCharger::OnOverlapBegin);
	BoxTrigger->OnComponentEndOverlap.AddDynamic(this, &AMyActorCharger::OnOverlapEnd);


}


void AMyActorCharger::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<AMyPawnPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}


void AMyActorCharger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AMyActorCharger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	AMyPawnPlayer* Pawn = Cast<AMyPawnPlayer>(OtherActor);

	if (Pawn)
	{
		Pawn->IsCharging = true;
	}
}

void  AMyActorCharger::OnOverlapEnd(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	AMyPawnPlayer* Pawn = Cast<AMyPawnPlayer>(OtherActor);
	
	if (Pawn)
	{
		Pawn->IsCharging = false;
	}
}

