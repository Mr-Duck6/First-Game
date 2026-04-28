


#include "MyPawnPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "MyActorGeneratorMap.h"


AMyPawnPlayer::AMyPawnPlayer()
{

	PrimaryActorTick.bCanEverTick = true;
	
    Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
    RootComponent = Scene;


    PlayerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMesh"));
    PlayerMesh->SetupAttachment(Scene);

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(PlayerMesh); 

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);


}


void AMyPawnPlayer::BeginPlay()
{
	Super::BeginPlay();


	
}


void AMyPawnPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    SpendEnergy();
}


void AMyPawnPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAction("MoveForward", IE_Pressed, this, &AMyPawnPlayer::MoveForward);
    PlayerInputComponent->BindAction("MoveBackward", IE_Pressed, this, &AMyPawnPlayer::MoveBackward);
    PlayerInputComponent->BindAction("MoveLeft", IE_Pressed, this, &AMyPawnPlayer::MoveLeft);
    PlayerInputComponent->BindAction("MoveRight", IE_Pressed, this, &AMyPawnPlayer::MoveRight);
}


void AMyPawnPlayer::MoveForward()
{

}

void AMyPawnPlayer::MoveBackward()
{

}

void AMyPawnPlayer::MoveLeft()
{

}

void AMyPawnPlayer::MoveRight()
{

}

void AMyPawnPlayer::SpendEnergy()
{
    float Time = GetWorld()->GetDeltaSeconds();
    if (PlayerEnergySpend && !PlayerCharging)
    {
        CurrentEnergy -= SpendTime *Time;
    }


}

