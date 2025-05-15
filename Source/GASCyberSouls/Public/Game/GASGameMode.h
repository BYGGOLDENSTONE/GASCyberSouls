// copyright GASCyberSouls

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GASGameMode.generated.h"

/**
 * Game Mode class for GASCyberSouls game
 */
UCLASS()
class GASCYBERSOULS_API AGASGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AGASGameMode();

	// Called when the game starts
	virtual void StartPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|HUD")
	TSubclassOf<AGASCyberSoulsHUD> GASHUDClass;
	
	// The default player character class to use
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game")
	TSubclassOf<class AGASPlayerCharacter> DefaultPlayerCharacterClass;
};