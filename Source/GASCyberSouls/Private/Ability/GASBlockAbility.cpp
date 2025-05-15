// copyright GASCyberSouls

#include "Ability/GASBlockAbility.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Character/GASCharacterBase.h"

UGASBlockAbility::UGASBlockAbility()
{
	// Default values
	BlockDuration = 2.0f;
	CooldownTime = 3.0f;
	
	// Set the ability tags
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Block")));
	
	// Set instancing policy
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGASBlockAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// Call parent implementation
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	// Add a gameplay tag to indicate blocking state
	FGameplayTag BlockingTag = FGameplayTag::RequestGameplayTag(FName("State.Blocking"));
	FGameplayTagContainer BlockingTagContainer;
	BlockingTagContainer.AddTag(BlockingTag);
	
	GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTags(BlockingTagContainer);
	
	// Set a timer to end the ability after duration
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &UGASBlockAbility::EndAbility, Handle, ActorInfo, ActivationInfo, true, false);
	
	ActorInfo->AbilitySystemComponent->GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		TimerDelegate,
		BlockDuration,
		false
	);
	
	// Play a montage or visual effect here
	UE_LOG(LogTemp, Display, TEXT("Block ability activated"));
}

void UGASBlockAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// Remove the blocking tag
	FGameplayTag BlockingTag = FGameplayTag::RequestGameplayTag(FName("State.Blocking"));
	FGameplayTagContainer BlockingTagContainer;
	BlockingTagContainer.AddTag(BlockingTag);
	
	if (ActorInfo && ActorInfo->AbilitySystemComponent.IsValid())
	{
		ActorInfo->AbilitySystemComponent->RemoveLooseGameplayTags(BlockingTagContainer);
	}
	
	// Apply cooldown
	if (!bWasCancelled && CooldownTime > 0.0f)
	{
		UGameplayEffect* CooldownEffect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("BlockCooldown")));
		CooldownEffect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
		CooldownEffect->DurationMagnitude = FScalableFloat(CooldownTime);
		
		// Add the cooldown tag to the effect
		FGameplayTag CooldownTag = FGameplayTag::RequestGameplayTag(FName(TEXT("Ability.Block.Cooldown")));
		CooldownEffect->InheritableOwnedTagsContainer.AddTag(CooldownTag);
		
		// Apply the cooldown effect
		FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
		GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectToSelf(CooldownEffect, 1.0f, EffectContext);
	}
	
	// Log the end of the ability
	UE_LOG(LogTemp, Display, TEXT("Block ability ended"));
	
	// Call parent implementation
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}