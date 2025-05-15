#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GASAbilitySystemComponent.generated.h"

/**
 * Custom Ability System Component for GASCyberSouls game
 */
UCLASS()
class GASCYBERSOULS_API UGASAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UGASAbilitySystemComponent();

	// Input handling for abilities
	void BindAbilityActivationToInputComponent(UInputComponent* InputComponent, FGameplayAbilityInputBinds BindInfo);
	
	// Clear abilities with the given tags
	void RemoveAbilitiesWithTags(const FGameplayTagContainer& Tags);
	
	// Give an ability specified by class
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	FGameplayAbilitySpecHandle GiveAbilityToComponent(TSubclassOf<UGameplayAbility> AbilityClass, int32 Level = 1, int32 InputID = -1);
	
	// Try to activate an ability by tag
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool TryActivateAbilityByTag(const FGameplayTagContainer& GameplayTagContainer, bool bAllowRemoteActivation = true);
};