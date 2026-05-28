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

    //Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
        class UStaticMeshComponent* CarMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
        class UBoxComponent* BoxCollision;

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    void InitializeCar(FVector Direction, FVector SpawnLocation);

    UFUNCTION()
        void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
            bool bFromSweep, const FHitResult& SweepResult);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car Settings")
        float MaxDriveDistance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car Settings")
        float Speed;

    float CarTargetSpeed;
    float CarCurrentSpeed;
    bool bCanMove;
    float StartDelayTimer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car Settings")
        FVector MoveDirection;
    FVector StartLocation;

};