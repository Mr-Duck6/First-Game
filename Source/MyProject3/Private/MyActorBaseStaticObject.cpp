


#include "MyActorBaseStaticObject.h"


AMyActorBaseStaticObject::AMyActorBaseStaticObject()
{

	PrimaryActorTick.bCanEverTick = true;

	StaticObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = StaticObjectMesh;

}


void AMyActorBaseStaticObject::BeginPlay()
{
	Super::BeginPlay();
	
}


void AMyActorBaseStaticObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

