// copyright GASCyberSouls

#pragma once

#include "CoreMinimal.h"
#include "GAS/GASGameplayAbility.h"
#include "GASAttackAbility.generated.h"

/**
 * Attack ability for GASCyberSouls
 * Handles basic attack functionality
 */
UCLASS()
class GASCYBERSOULS_API UGASAttackAbility : public UGASGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGASAttackAbility();
	
	// Activate the ability
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
protected:
	// Base damage amount for this attack
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float BaseDamage;
	
	// Attack range
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float AttackRange;
	
	// Attack radius (for area attacks)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float AttackRadius;
	
	// Cooldown time in seconds
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float CooldownTime;
	
	// Apply damage to target
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void ApplyDamage();
};