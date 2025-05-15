// copyright GASCyberSouls

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GASGameplayAbility.generated.h"

/**
 * Base Gameplay Ability class for GASCyberSouls game
 */
UCLASS()
class GASCYBERSOULS_API UGASGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGASGameplayAbility();

	// Abilities with this set to true will automatically activate when the input is pressed
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	bool bActivateAbilityOnGranted;

	// Tag that defines the ability input binding
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	FGameplayTag AbilityInputTag;

	// Called when the ability is activated
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
};