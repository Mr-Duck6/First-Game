#include "UTimerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MySaveGame.h"

UTimerComponent::UTimerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	BestTime = 0.f;
	CurrentTime = 0.f;
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
	if (SaveSlotName.IsEmpty())
	{
		SaveSlotName = TEXT("DefaultSlot");
	}

	UMySaveGame* SaveGameInstanceRef = nullptr;

	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		SaveGameInstanceRef = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	}

	if (!SaveGameInstanceRef)
	{
		SaveGameInstanceRef = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	}

	if (SaveGameInstanceRef)
	{
		SaveGameInstanceRef->BestTime = NewRecord;
		UGameplayStatics::SaveGameToSlot(SaveGameInstanceRef, SaveSlotName, 0);
	}
}

void UTimerComponent::LoadRecord()//Load
{
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))//Check file
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