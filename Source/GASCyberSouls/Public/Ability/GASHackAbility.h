// copyright GASCyberSouls

#pragma once

#include "CoreMinimal.h"
#include "GAS/GASGameplayAbility.h"
#include "GASHackAbility.generated.h"

/**
 * Hack ability for GASCyberSouls
 * Used by hacker enemies to increase the player's HackProgress
 */
UCLASS()
class GASCYBERSOULS_API UGASHackAbility : public UGASGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGASHackAbility();
	
	// Activate the ability
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	// End the ability
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
protected:
	// Amount to increase HackProgress per tick
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hack")
	float HackProgressPerSecond;
	
	// Hack range in units
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hack")
	float HackRange;
	
	// How often to apply hack progress
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hack")
	float TickInterval;
	
	// Timer handle for applying hack progress
	FTimerHandle HackTimerHandle;
	
	// Apply hack progress to player
	UFUNCTION()
	void ApplyHackProgress();
};