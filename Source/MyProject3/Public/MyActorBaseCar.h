#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActorBaseCar.generated.h"

class UBoxComponent;
class AMyActorRoadLine;

UCLASS()
class MYPROJECT3_API AMyActorBaseCar : public AActor
{
    GENERATED_BODY()

public:
    AMyActorBaseCar();

    //Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
        class UStaticMeshComponent* CarMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
        class UBoxComponent* BoxCollision;

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UFUNCTION()
        void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
            bool bFromSweep, const FHitResult& SweepResult);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car Settings")
        float MaxDriveDistance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car Settings")
        float Speed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car Settings")
        FVector MoveDirection;

    FVector StartLocation;


};