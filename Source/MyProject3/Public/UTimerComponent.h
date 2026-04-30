
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UTimerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MYPROJECT3_API UTimerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UTimerComponent();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float CurrentTime = 0.f;

	FTimerHandle TimerHandle;

	void BaseTimer();
	void GetTime();

	void CheckTime(float Current, float MaxTime);
		
};
