#include "MyActorLamp.h"
#include "Components/SpotLightComponent.h" 

AMyActorLamp::AMyActorLamp()
{
	PrimaryActorTick.bCanEverTick = false;

	LampMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LampMesh"));
	RootComponent = LampMesh;

	LampLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("LampLight"));
	LampLight->SetupAttachment(RootComponent);
	LightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("LightComponent"));
}