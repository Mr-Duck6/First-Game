
#include "UTimerComponent.h"



UTimerComponent::UTimerComponent()
{

	PrimaryComponentTick.bCanEverTick = true;


}



void UTimerComponent::BeginPlay()
{
	Super::BeginPlay();


	
}



void UTimerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

void UTimerComponent::BaseTimer()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, 999999.0f, false);

}

void UTimerComponent::GetTime()
{
	if (!TimerHandle.IsValid())
	{
		return;
	}

	CurrentTime = GetWorld()->GetTimerManager().GetTimerElapsed(TimerHandle);
}

void  UTimerComponent::CheckTime(float Current, float MaxTime)
{



}

