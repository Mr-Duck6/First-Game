
#include "MyActorCharger.h"
#include "Kismet/GameplayStatics.h"
#include "MyPawnPlayer.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/BoxComponent.h"

DEFINE_LOG_CATEGORY(ChargerLog);

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


void AMyActorCharger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp,//Charge player
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (Player)
	{
		UE_LOG(ChargerLog, Display, TEXT("Player on charger"));
		UE_LOG(ChargerLog, Display, TEXT("bIsCharging = %s"), Player->bIsCharging ? TEXT("true") : TEXT("false"));
		Player->bIsCharging = true;
	}
	else
	{
		UE_LOG(ChargerLog, Display, TEXT("Player not found"));
	}
}

void  AMyActorCharger::OnOverlapEnd(UPrimitiveComponent* OverlappedComp,//Stop charge player
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (Player)
	{
		UE_LOG(ChargerLog, Display, TEXT("Player out of charge"));
		UE_LOG(ChargerLog, Display, TEXT("bIsCharging = %s"), Player->bIsCharging ? TEXT("true") : TEXT("false"));
		Player->bIsCharging = false;
	}
	else
	{
		UE_LOG(ChargerLog, Display, TEXT("Player not found"));
	}
}

