
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* PlayerMeshComponent;
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


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
	class AMyActorGeneratorMap* MapGenerator;
	//Move
	void MoveForward();
	void MoveBackward();
	void MoveLeft();
	void MoveRight();


	UPROPERTY()
	AMyActorRoadLine* CurrentLine;

	int32 CellX;
	int32 CellY;

	FVector TargetLocation;

	int32 CurrentLineIndex;

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

	float SpendEnergy=2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Energy")
	float MaxEnergy = 100;


};
