// copyright GASCyberSouls



#pragma once

#include "CoreMinimal.h"
#include "GAS/GASGameplayAbility.h"
#include "Character/GASTypes.h"
#include "GASSlashAbility.generated.h"

/**
 * Slash ability for GASCyberSouls
 * Player's main attack ability that targets specific body parts
 */
UCLASS()
class GASCYBERSOULS_API UGASSlashAbility : public UGASGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGASSlashAbility();
	
	// Activate the ability
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
protected:
	// Base damage amount for this slash
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slash")
	float BaseDamage;
	
	// Bonus damage multiplier when targeting specific body parts
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slash")
	float BodyPartDamageMultiplier;
	
	// Range of the slash attack
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slash")
	float SlashRange;
	
	// Cooldown time in seconds
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slash")
	float CooldownTime;
	
	// Montages for different body part attacks
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slash|Animation")
	UAnimMontage* UpperBodySlashMontage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slash|Animation")
	UAnimMontage* LowerBodySlashMontage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slash|Animation")
	UAnimMontage* LeftLegSlashMontage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slash|Animation")
	UAnimMontage* RightLegSlashMontage;
	
	UFUNCTION(BlueprintCallable) // Or other UFUNCTION specifiers if needed
	void ApplySlashDamage(); // Should take NO parameters

	
};