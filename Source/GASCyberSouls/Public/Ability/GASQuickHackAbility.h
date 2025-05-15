// copyright GASCyberSouls

#pragma once

#include "CoreMinimal.h"
#include "GAS/GASGameplayAbility.h"
#include "Character/GASTypes.h"
#include "GASQuickHackAbility.generated.h"

/**
 * Base QuickHack ability for CyberSouls game
 */
UCLASS()
class GASCYBERSOULS_API UGASQuickHackAbility : public UGASGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGASQuickHackAbility();
	
	// The type of quickhack
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuickHack")
	EQuickHackType QuickHackType;
	
	// Cast time in seconds
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuickHack")
	float CastTime;
	
	// Cooldown in seconds
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuickHack")
	float Cooldown;
	
	// Duration of effect
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuickHack")
	float Duration;
	
	// Override to implement quick hack specific functionality
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	// End the ability
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	// Called when the quick hack is successfully cast
	UFUNCTION(BlueprintCallable, Category = "QuickHack")
	virtual void OnQuickHackSucceeded();
	
	// Called when the quick hack is interrupted
	UFUNCTION(BlueprintCallable, Category = "QuickHack")
	virtual void OnQuickHackInterrupted();
	
	// Get the QuickHack data
	UFUNCTION(BlueprintCallable, Category = "QuickHack")
	FQuickHackData GetQuickHackData() const;
	
protected:
	// Is the quickhack currently casting?
	bool bIsCasting;
	
	// Timer handle for casting
	FTimerHandle CastingTimerHandle;
	
	// Apply the effect of the quick hack
	UFUNCTION(BlueprintCallable, Category = "QuickHack")
	virtual void ApplyQuickHackEffect();
};