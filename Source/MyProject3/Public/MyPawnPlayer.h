
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyPawnPlayer.generated.h"

class AMyActorGeneratorMap;
class AMyActorRoadLine;
class AMyActorCharger;

DECLARE_LOG_CATEGORY_EXTERN(PlayerLog, Log, All);

UCLASS()
class MYPROJECT3_API AMyPawnPlayer : public APawn
{
	GENERATED_BODY()

public:

	AMyPawnPlayer();
	

	//Component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class USkeletalMeshComponent* SkeletanMesh;


	UPROPERTY(VisibleAnywhere)
	class USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UCameraComponent* CameraComponent;

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Skins
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skins")
		TArray<UStaticMesh*> AvailableStaticSkins;


	//Moveming
	void MoveForward();
	void MoveBackward();
	void MoveLeft();
	void MoveRight();

	//Can move?
	void CanMove(FVector Direction);
	bool bCanMove;
	FVector StartHit;
	FVector EndHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Move settings")
	int32 OneStep;

	//Player location, map
	UPROPERTY()
	AMyActorRoadLine* CurrentLine;
	int32 CellX;
	FVector TargetLocation;
	bool bIsMoveing;

	UPROPERTY()
		TArray<AMyActorRoadLine*> SpawnedLines;
	UPROPERTY()
		AMyActorGeneratorMap* MapGenerator;

	//Energy
	FTimerHandle EnergyTimer;

	UFUNCTION(BlueprintCallable, Category = "Energy")
	void UpdateEnergy();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
	bool bIsCharging;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Energy")
	float CurrentEnergy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
	float SpendEnergy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
	float ChargeEnergy;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Energy")
	float MaxEnergy;

	//Time and player status
	UFUNCTION(BlueprintCallable, Category = "Player status")
		void PlayerDeath();

	UFUNCTION(BlueprintCallable, Category = "Game Lifecycle")
		void Restart();

	bool bIsDead;

	FTimerHandle TimerAfterDead;

};
