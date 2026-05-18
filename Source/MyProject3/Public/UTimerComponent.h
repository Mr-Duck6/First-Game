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

	UPROPERTY(BlueprintReadOnly, Category = "Timer")
		float CurrentTime = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Timer")
		float BestTime = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Timer")
		bool bIsTimerRunning = false;

private:
	void LoadRecord();
	void SaveRecord(float NewRecord);

	const FString SaveSlotName = TEXT("Slot1");
};