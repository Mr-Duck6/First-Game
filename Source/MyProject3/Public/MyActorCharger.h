
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
		class UStaticMeshComponent* LampMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UBoxComponent* BoxTrigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class USpotLightComponent* Light;


protected:

	virtual void BeginPlay() override;

public:	
	class AMyPawnPlayer* Player;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp,AActor* OtherActor,UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp,AActor* OtherActor,
		UPrimitiveComponent* OtherComp,int32 OtherBodyIndex);

};
