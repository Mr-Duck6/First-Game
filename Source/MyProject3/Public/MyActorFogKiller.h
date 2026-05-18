#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "MyActorFogKiller.generated.h" 


class AMyPawnPlayer;

DECLARE_LOG_CATEGORY_EXTERN(FogLog, Log, All);

UCLASS()
class MYPROJECT3_API AMyActorFogKiller : public AActor
{
    GENERATED_BODY()

public:
    AMyActorFogKiller();

    //Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
        class UBoxComponent* TrigerComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
        class UParticleSystemComponent* FogParticles;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
        class USceneComponent* Scene;

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UFUNCTION()
        void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,//In fog
            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
            bool bFromSweep, const FHitResult& SweepResult);

    void MoveFog(float DeltaTime);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
        FVector MovementVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
        float Speed;

    AMyPawnPlayer* Player;
};