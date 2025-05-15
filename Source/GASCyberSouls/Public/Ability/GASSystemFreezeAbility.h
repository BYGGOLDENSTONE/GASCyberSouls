// copyright GASCyberSouls

#pragma once

#include "CoreMinimal.h"
#include "Ability/GASQuickHackAbility.h"
#include "GASSystemFreezeAbility.generated.h"

/**
 * System Freeze QuickHack ability
 * Immobilizes the target for 2s: no movement, no attacks, no quick-hacks
 */
UCLASS()
class GASCYBERSOULS_API UGASSystemFreezeAbility : public UGASQuickHackAbility
{
	GENERATED_BODY()
	
public:
	UGASSystemFreezeAbility();
	
	// Override to implement System Freeze specific functionality
	virtual void ApplyQuickHackEffect() override;
};