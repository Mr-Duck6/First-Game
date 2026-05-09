

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActorBaseCar.generated.h"

class UBoxComponent;

UCLASS()
class MYPROJECT3_API AMyActorBaseCar : public AActor
{
	GENERATED_BODY()
	
public:	

	AMyActorBaseCar();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* CarMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class UBoxComponent* BoxCollision;


protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	void MoveCar(float DeltaTime);

	void InitializeCar(FVector Direction,float NewSpeed,FVector SpawnPos);

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

		float MaxX = 600.f;

	UPROPERTY()
		FVector MoveDirection = FVector::RightVector;

	FVector StartLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float Speed = 50.f;

};

