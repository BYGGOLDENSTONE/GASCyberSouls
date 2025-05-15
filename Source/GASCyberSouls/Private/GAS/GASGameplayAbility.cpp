// copyright GASCyberSouls


#include "GAS/GASGameplayAbility.h"
#include "AbilitySystemComponent.h"

UGASGameplayAbility::UGASGameplayAbility()
{
	// Default values
	bActivateAbilityOnGranted = false;
	
	// Set instancing policy - abilities should generally be instanced per actor
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGASGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	// If we're set to activate on granted and we're the authority
	if (bActivateAbilityOnGranted && ActorInfo->IsLocallyControlled())
	{
		// Try to activate the ability
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}