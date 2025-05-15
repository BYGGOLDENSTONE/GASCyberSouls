// copyright GASCyberSouls

#pragma once

#include "CoreMinimal.h"
#include "GAS/GASGameplayAbility.h"
#include "GASDodgeAbility.generated.h"

/**
 * Dodge ability for GASCyberSouls
 * Used by dodge-capable enemies to dodge attacks to lower body
 */
UCLASS()
class GASCYBERSOULS_API UGASDodgeAbility : public UGASGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGASDodgeAbility();
	
	// Activate the ability
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	// End the ability
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
protected:
	// Dodge duration in seconds
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dodge")
	float DodgeDuration;
	
	// Dodge cooldown in seconds
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dodge")
	float CooldownTime;
	
	// Dodge distance
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dodge")
	float DodgeDistance;
};