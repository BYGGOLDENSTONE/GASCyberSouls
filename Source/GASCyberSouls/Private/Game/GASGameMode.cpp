// copyright GASCyberSouls


#include "Game/GASGameMode.h"
#include "Character/GASPlayerCharacter.h"
#include "Game/GASCyberSoulsHUD.h"
#include "UObject/ConstructorHelpers.h"

AGASGameMode::AGASGameMode()
{
	// Set default player character class
	static ConstructorHelpers::FClassFinder<AGASPlayerCharacter> PlayerCharacterBPClass(TEXT("/Game/Characters/BP_GASPlayerCharacter"));
	if (PlayerCharacterBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerCharacterBPClass.Class;
	}
	
	
}

void AGASGameMode::StartPlay()
{
	Super::StartPlay();

	// Initialize any game mode specific systems here
	
	// Log that the game has started
	UE_LOG(LogTemp, Display, TEXT("GASCyberSouls Game Started!"));
}