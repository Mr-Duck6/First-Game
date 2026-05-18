#include "UTimerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MySaveGame.h"

UTimerComponent::UTimerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTimerComponent::BeginPlay()
{
	Super::BeginPlay();
	LoadRecord();
	StartTimer();
}

void UTimerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsTimerRunning)
	{
		CurrentTime += DeltaTime;
	}
}

void UTimerComponent::StartTimer()
{
	CurrentTime = 0.f;
	bIsTimerRunning = true;
}

void UTimerComponent::StopAndCheckRecord()
{
	bIsTimerRunning = false;

	if (CurrentTime > BestTime)
	{
		BestTime = CurrentTime;
		SaveRecord(BestTime);
	}
}

void UTimerComponent::SaveRecord(float NewRecord)
{
	UMySaveGame* SaveGameInstanceRef = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	if (!SaveGameInstanceRef)
	{
		SaveGameInstanceRef = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	}

	if (SaveGameInstanceRef)
	{
		SaveGameInstanceRef->BestTime = NewRecord;
		bool bIsSaved = UGameplayStatics::SaveGameToSlot(SaveGameInstanceRef, SaveSlotName, 0);

		if (bIsSaved)
		{
			UE_LOG(LogTemp, Log, TEXT("Record securely saved. New Best Time: %.2f"), NewRecord);
		}
	}
}

void UTimerComponent::LoadRecord()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		UMySaveGame* LoadedGame = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
		if (LoadedGame)
		{
			BestTime = LoadedGame->BestTime;
			UE_LOG(LogTemp, Log, TEXT("Secure Record Loaded. Best Time is: %.2f"), BestTime);
			return;
		}
	}
	BestTime = 0.f;
}