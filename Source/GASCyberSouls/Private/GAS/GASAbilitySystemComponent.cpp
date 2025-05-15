#include "GAS/GASAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameplayAbilitySpec.h"

UGASAbilitySystemComponent::UGASAbilitySystemComponent()
{
	// Default constructor
}

void UGASAbilitySystemComponent::BindAbilityActivationToInputComponent(UInputComponent* InputComponent, FGameplayAbilityInputBinds BindInfo)
{
	// Call the parent implementation
	Super::BindAbilityActivationToInputComponent(InputComponent, BindInfo);
}

void UGASAbilitySystemComponent::RemoveAbilitiesWithTags(const FGameplayTagContainer& Tags)
{
	// Get a list of active ability specs
	TArray<FGameplayAbilitySpec*> MatchingAbilities;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(Tags, MatchingAbilities);

	// Remove each ability that has matching tags
	for (FGameplayAbilitySpec* Spec : MatchingAbilities)
	{
		if (Spec->IsActive())
		{
			// Cancel the ability if it's active
			CancelAbilitySpec(*Spec, nullptr);
		}

		// Remove the ability
		ClearAbility(Spec->Handle);
	}
}

FGameplayAbilitySpecHandle UGASAbilitySystemComponent::GiveAbilityToComponent(TSubclassOf<UGameplayAbility> AbilityClass, int32 Level, int32 InputID)
{
	// Return early if we don't have a valid class
	if (!AbilityClass)
	{
		return FGameplayAbilitySpecHandle();
	}

	// Create a new ability spec and give it to the component
	FGameplayAbilitySpec AbilitySpec(AbilityClass, Level, InputID, this);
	return GiveAbility(AbilitySpec);
}

bool UGASAbilitySystemComponent::TryActivateAbilityByTag(const FGameplayTagContainer& GameplayTagContainer, bool bAllowRemoteActivation)
{
	// Try to activate an ability that matches the tag container
	return TryActivateAbilitiesByTag(GameplayTagContainer, bAllowRemoteActivation);
}