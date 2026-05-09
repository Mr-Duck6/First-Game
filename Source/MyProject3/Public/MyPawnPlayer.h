
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyPawnPlayer.generated.h"

class AMyActorGeneratorMap;
class AMyActorRoadLine;

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


	//Move
	void MoveForward();
	void MoveBackward();
	void MoveLeft();
	void MoveRight();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
		class AMyActorGeneratorMap* MapGenerator;

	UPROPERTY()
	AMyActorRoadLine* CurrentLine;

	int32 CellX;

	FVector TargetLocation;

	bool IsMoveing = false;

	//Energy
	FTimerHandle EnergyTimer;

	UFUNCTION(BlueprintCallable, Category = "Energy")
	void UpdateEnergy();

	UFUNCTION(BlueprintCallable, Category = "Energy")
	void PlayerDeath();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Energy")
	bool IsCharging = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Energy")
	float CurrentEnergy;

	float SpendEnergy=0.5;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Energy")
	float MaxEnergy = 100;

	//Time and player status
	float CurrentTime;

	FTimerHandle RecordTimer;

	void UpdateTime();

	bool IsDead = false;

	UPROPERTY()
		TArray<AMyActorRoadLine*> SpawnedLines;

};
