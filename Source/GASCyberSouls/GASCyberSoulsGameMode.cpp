// Copyright Epic Games, Inc. All Rights Reserved.

#include "GASCyberSoulsGameMode.h"
#include "GASCyberSoulsCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGASCyberSoulsGameMode::AGASCyberSoulsGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
