// copyright GASCyberSouls

#pragma once

#include "CoreMinimal.h"
#include "GAS/GASGameplayAbility.h"
#include "GASBlockAbility.generated.h"

/**
 * Block ability for GASCyberSouls
 * Used by block-capable enemies to block attacks to upper body
 */
UCLASS()
class GASCYBERSOULS_API UGASBlockAbility : public UGASGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGASBlockAbility();
	
	// Activate the ability
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	// End the ability
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
protected:
	// Block duration in seconds
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Block")
	float BlockDuration;
	
	// Block cooldown in seconds
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Block")
	float CooldownTime;
};