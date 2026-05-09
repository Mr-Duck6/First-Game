#include "MyGameModeBase.h"
#include "MyGameState.h"
#include "MyPawnPlayer.h"

AMyGameModeBase::AMyGameModeBase()
{
    GameStateClass = AMyGameState::StaticClass();
    DefaultPawnClass = AMyPawnPlayer::StaticClass(); 
}