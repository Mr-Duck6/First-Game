
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
	float CurrentEnergy = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Energy")
	float MaxEnergy = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Energy")
	float MinEnergy = 0;

	void SpendEnergy();


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Energy")
	bool PlayerCharging = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Energy")
	bool PlayerEnergySpend = true;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Energy")
	float SpendTime = 5.f;

};
