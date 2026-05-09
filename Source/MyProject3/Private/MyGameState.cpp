#include "MyGameState.h"
#include"MyPawnPlayer.h"
#include "Kismet/GameplayStatics.h"

AMyGameState::AMyGameState()
{

}


void AMyGameState::RestartLevel()
{
        FString LevelName = GetWorld()->GetMapName();
        LevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
        UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName));
}


