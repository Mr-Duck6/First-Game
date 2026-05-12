
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyPawnPlayer.generated.h"

class AMyActorGeneratorMap;
class AMyActorRoadLine;
class AMyActorCharger;

UCLASS()
class MYPROJECT3_API AMyPawnPlayer : public APawn
{
	GENERATED_BODY()

public:

	AMyPawnPlayer();
	

	//Component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* PlayerMeshComponent;

	UPROPERTY(VisibleAnywhere)
		USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UCameraComponent* CameraComponent;

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Functions
	void MoveForward();
	void MoveBackward();
	void MoveLeft();
	void MoveRight();

	//Referance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
		class AMyActorGeneratorMap* MapGenerator;

	UPROPERTY()
	AMyActorRoadLine* CurrentLine;

	int32 CellX;
	FVector TargetLocation;
	bool IsMoveing;

	//Energy
	FTimerHandle EnergyTimer;

	UFUNCTION(BlueprintCallable, Category = "Energy")
	void UpdateEnergy();

	UFUNCTION(BlueprintCallable, Category = "Energy")
	void PlayerDeath();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Energy")
	bool IsCharging;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Energy")
	float CurrentEnergy;

	float SpendEnergy;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Energy")
	float MaxEnergy;

	//Time and player status
	float CurrentTime;
	bool IsDead;

	UPROPERTY()
		TArray<AMyActorRoadLine*> SpawnedLines;

	FTimerHandle TimerAfterDead;

	//void Restart();
	FVector StartHit;
	FVector EndHit;

	void CanMove(FVector Direction);
	bool bCanMove;
};
