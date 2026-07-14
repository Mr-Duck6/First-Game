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

	//Components
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* LampMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UPointLightComponent* LightComponent;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> LampBlueprintClass;
};