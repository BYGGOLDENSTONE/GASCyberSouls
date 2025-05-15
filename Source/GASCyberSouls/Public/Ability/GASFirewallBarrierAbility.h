// copyright GASCyberSouls

#pragma once

#include "CoreMinimal.h"
#include "Ability/GASQuickHackAbility.h"
#include "GASFirewallBarrierAbility.generated.h"

/**
 * Firewall Barrier QuickHack ability
 * Shields the caster for 3s against all incoming quick hacks and suppresses hackprogress gain
 */
UCLASS()
class GASCYBERSOULS_API UGASFirewallBarrierAbility : public UGASQuickHackAbility
{
	GENERATED_BODY()
	
public:
	UGASFirewallBarrierAbility();
	
	// Override to implement Firewall Barrier specific functionality
	virtual void ApplyQuickHackEffect() override;
};