#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActorTrain.generated.h"

class UBoxComponent;
class USpotLightComponent;
class AMyPawnPlayer;

DECLARE_LOG_CATEGORY_EXTERN(TrainLog, Log, All);

UCLASS()
class MYPROJECT3_API AMyActorTrain : public AActor
{
    GENERATED_BODY()

public:
    AMyActorTrain();

    //Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
        UStaticMeshComponent* Mesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
        UBoxComponent* BoxCollision;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
        USpotLightComponent* TrainLight;

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    FTimerHandle TrainTimer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Train Settings")
        float Speed = 2000.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Train Settings")
        float MaxDriveDistance = 4000.f;

    FVector StartLocation;
    FVector MoveDirection;
    bool bCanMove;

    void StartMove();

    UFUNCTION()
        void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
            bool bFromSweep, const FHitResult& SweepResult);//Kill player

    AMyPawnPlayer* PlayerRef;
};