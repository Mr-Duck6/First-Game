#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActorLamp.generated.h"

UCLASS()
class MYPROJECT3_API AMyActorLamp : public AActor
{
	GENERATED_BODY()

public:
	AMyActorLamp();

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* LampMesh;

	UPROPERTY(VisibleAnywhere)
		class UPointLightComponent* LampLight;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> LampBlueprintClass;

	void SpawnLampForLine(float LineY);
};