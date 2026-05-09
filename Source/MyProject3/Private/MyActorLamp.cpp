#include "MyActorLamp.h"
#include "Components/PointLightComponent.h"

AMyActorLamp::AMyActorLamp()
{
	PrimaryActorTick.bCanEverTick = false;

	LampMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LampMesh"));
	RootComponent = LampMesh;

	LampLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("LampLight"));
	LampLight->SetupAttachment(RootComponent);
	LampLight->SetRelativeLocation(FVector(0, 0, 50));
}

void AMyActorLamp::SpawnLampForLine(float LineY)
{
	if (!LampBlueprintClass) return;

	float LeftX = 300;
	float RightX = -300;
	float Z = 200.f;

	GetWorld()->SpawnActor<AActor>(LampBlueprintClass,FVector(LeftX, LineY, Z),
	FRotator::ZeroRotator);

	GetWorld()->SpawnActor<AActor>(LampBlueprintClass,FVector(RightX, LineY, Z),
	FRotator::ZeroRotator);
}