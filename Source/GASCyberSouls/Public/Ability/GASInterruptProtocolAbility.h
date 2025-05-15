// copyright GASCyberSouls

#pragma once

#include "CoreMinimal.h"
#include "Ability/GASQuickHackAbility.h"
#include "GASInterruptProtocolAbility.generated.h"

/**
 * Interrupt Protocol QuickHack ability
 * Cancels any hack or QuickHack currently being cast by the target
 */
UCLASS()
class GASCYBERSOULS_API UGASInterruptProtocolAbility : public UGASQuickHackAbility
{
	GENERATED_BODY()
	
public:
	UGASInterruptProtocolAbility();
	
	// Override to implement Interrupt Protocol specific functionality
	virtual void ApplyQuickHackEffect() override;
};