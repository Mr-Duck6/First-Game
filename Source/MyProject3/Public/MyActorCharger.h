
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActorCharger.generated.h"



UCLASS()
class MYPROJECT3_API AMyActorCharger : public AActor
{
	GENERATED_BODY()
	
public:	

	AMyActorCharger();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* ChargerMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UBoxComponent* BoxTrigger;


protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	void Charge();

	float ChargeTime = 4.f;


};
