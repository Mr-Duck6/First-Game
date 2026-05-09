


#include "MyActorBaseCar.h"
#include "MyPawnPlayer.h"
#include "Components/BoxComponent.h"


AMyActorBaseCar::AMyActorBaseCar()
{

	PrimaryActorTick.bCanEverTick = true;

	CarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = BoxCollision;
	CarMesh->SetupAttachment(BoxCollision);

	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxCollision->SetCollisionResponseToAllChannels(ECR_Overlap);

	
}



void AMyActorBaseCar::BeginPlay()
{
	Super::BeginPlay();
	
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AMyActorBaseCar::OnOverlapBegin);
}


void AMyActorBaseCar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveCar(DeltaTime);

	if (GetActorLocation().X > MaxX)
	{
		SetActorLocation(StartLocation);
	}
}

void AMyActorBaseCar::MoveCar(float DeltaTime)//Move car
{
	UE_LOG(LogTemp,Display,TEXT("Function MoveCar called"))
	SetActorLocation(GetActorLocation() +MoveDirection * Speed * DeltaTime);
}

void AMyActorBaseCar::InitializeCar(FVector Direction,float NewSpeed,FVector SpawnPos)//Edit car
{
	UE_LOG(LogTemp, Display, TEXT("Function InitializeCar called"))
	MoveDirection = Direction;
	Speed = NewSpeed;
	StartLocation = SpawnPos;

	SetActorLocation(SpawnPos);
}

void AMyActorBaseCar::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,//Triger function
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult)
{
	AMyPawnPlayer* PlayerREF = Cast<AMyPawnPlayer>(OtherActor);
	if (OtherActor && (OtherActor != this)&&PlayerREF)
	{
		UE_LOG(LogTemp, Display, TEXT("Triger activate"))
		UE_LOG(LogTemp, Warning, TEXT("Actor %s entered the trigger"), *OtherActor->GetName());
		PlayerREF->PlayerDeath();
	}
}