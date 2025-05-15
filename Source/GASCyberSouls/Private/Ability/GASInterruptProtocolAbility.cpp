// copyright GASCyberSouls

#include "Ability/GASInterruptProtocolAbility.h"
#include "AbilitySystemComponent.h"
#include "Character/GASCharacterBase.h"
#include "GameplayTagContainer.h"

UGASInterruptProtocolAbility::UGASInterruptProtocolAbility()
{
    QuickHackType = EQuickHackType::InterruptProtocol;
    CastTime = 5.0f;  // As specified in Game.txt
    Cooldown = 8.0f;  // As specified in Game.txt
    Duration = 1.0f;  // Stun duration after interrupt
    
    // Set the ability tags
    AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.QuickHack.InterruptProtocol")));
    CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.QuickHack")));
}

void UGASInterruptProtocolAbility::ApplyQuickHackEffect()
{
    // Call parent implementation
    Super::ApplyQuickHackEffect();
    
    // Get the target from the targeting component
    AGASCharacterBase* TargetCharacter = nullptr;
    
    if (CurrentActorInfo && CurrentActorInfo->AvatarActor.IsValid())
    {
        // Assume we have a reference to the targeting component on the player
        // This would need to be set up in your GASPlayerCharacter class
        AGASCharacterBase* SourceCharacter = Cast<AGASCharacterBase>(CurrentActorInfo->AvatarActor.Get());
        if (SourceCharacter)
        {
            // Here you would get the target from the targeting component
            // For simplicity, let's assume we can get it directly
            // In a real implementation, you'd use the targeting component
            
            // Example (pseudocode):
            // UGASTargetingComponent* TargetingComp = SourceCharacter->GetTargetingComponent();
            // if (TargetingComp)
            // {
            //     TargetCharacter = TargetingComp->GetCurrentTarget();
            // }
        }
    }
    
    if (TargetCharacter)
    {
        // Get the ability system component of the target
        UAbilitySystemComponent* TargetASC = TargetCharacter->GetAbilitySystemComponent();
        if (TargetASC)
        {
            // Cancel any active QuickHack abilities on the target
            FGameplayTagContainer QuickHackTags;
            QuickHackTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.QuickHack")));
            TargetASC->CancelAbilities(&QuickHackTags);
            
            // Apply a stun effect to the target
            // Create a gameplay effect for the stun
            UGameplayEffect* StunEffect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("InterruptProtocolStun")));
            StunEffect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
            StunEffect->DurationMagnitude = FScalableFloat(Duration);
            
            // Add the stun tag to the effect
            FGameplayTag StunTag = FGameplayTag::RequestGameplayTag(FName(TEXT("State.Stunned")));
            StunEffect->InheritableOwnedTagsContainer.AddTag(StunTag);
            
            // Apply the stun effect to the target
            FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
            TargetASC->ApplyGameplayEffectToSelf(StunEffect, 1.0f, EffectContext);
        }
    }
}