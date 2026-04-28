
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyPawnPlayer.generated.h"

class MyActorGeneratorMap;

UCLASS()
class MYPROJECT3_API AMyPawnPlayer : public APawn
{
	GENERATED_BODY()

public:

	AMyPawnPlayer();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* PlayerMesh;
	class USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UCameraComponent* Camera;

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward();
	void MoveBackward();
	void MoveLeft();
	void MoveRight();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Energy")
	float CurrentEnergy = 0;
	float MaxEnergy = 100;
	float MinEnergy = 0;

	void SpendEnergy();

	bool PlayerCharging = false;
	bool PlayerEnergySpend = true;

	float SpendTime = 5.f;

};
