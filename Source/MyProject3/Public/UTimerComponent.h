#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UTimerComponent.generated.h"

class UMySaveGame;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYPROJECT3_API UTimerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTimerComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Timer")
		void StartTimer();

	UFUNCTION(BlueprintCallable, Category = "Timer")
		void StopAndCheckRecord();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
		float CurrentTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
		float BestTime;

	UPROPERTY(BlueprintReadOnly, Category = "Timer")
		bool bIsTimerRunning = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveGame")
		FString SaveSlotName = TEXT("MyTimerSaveSlot");

private:
	void LoadRecord();
	void SaveRecord(float NewRecord);
};