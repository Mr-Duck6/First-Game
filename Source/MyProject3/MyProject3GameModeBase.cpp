
#include "MyProject3GameModeBase.h"
#include "Kismet/GameplayStatics.h" 

void AMyProject3GameModeBase::RestartLevel()
{
    FString CurrentLevelName = GetWorld()->GetMapName();
    UGameplayStatics::OpenLevel(this, FName(*CurrentLevelName), true);
}
