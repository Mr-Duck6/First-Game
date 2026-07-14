#include "MyActorLamp.h"
#include "Components/SpotLightComponent.h" 

AMyActorLamp::AMyActorLamp()
{
	PrimaryActorTick.bCanEverTick = false;

	LampMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LampMesh"));
	RootComponent = LampMesh;
	LightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("LightComponent"));
	LightComponent->SetupAttachment(LampMesh);
}