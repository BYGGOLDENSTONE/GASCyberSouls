// copyright GASCyberSouls

#include "Ability/GASFirewallBarrierAbility.h"
#include "AbilitySystemComponent.h"
#include "Character/GASCharacterBase.h"
#include "GameplayTagContainer.h"
#include "Attribute/GASAttributeSet.h"

UGASFirewallBarrierAbility::UGASFirewallBarrierAbility()
{
    QuickHackType = EQuickHackType::FirewallBarrier;
    CastTime = 6.0f;  // As specified in Game.txt
    Cooldown = 12.0f; // As specified in Game.txt
    Duration = 3.0f;  // As specified in Game.txt
    
    // Set the ability tags
    AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.QuickHack.FirewallBarrier")));
}

void UGASFirewallBarrierAbility::ApplyQuickHackEffect()
{
    // Call parent implementation
    Super::ApplyQuickHackEffect();
    
    // For the Firewall Barrier, we apply it to self rather than a target
    if (CurrentActorInfo && CurrentActorInfo->AvatarActor.IsValid())
    {
        AGASCharacterBase* SourceCharacter = Cast<AGASCharacterBase>(CurrentActorInfo->AvatarActor.Get());
        if (SourceCharacter)
        {
            UAbilitySystemComponent* SourceASC = SourceCharacter->GetAbilitySystemComponent();
            if (SourceASC)
            {
                // Create a gameplay effect for the firewall barrier
                UGameplayEffect* BarrierEffect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("FirewallBarrier")));
                BarrierEffect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
                BarrierEffect->DurationMagnitude = FScalableFloat(Duration);
                
                // Add the barrier tag to the effect
                FGameplayTag BarrierTag = FGameplayTag::RequestGameplayTag(FName(TEXT("State.FirewallBarrier")));
                BarrierEffect->InheritableOwnedTagsContainer.AddTag(BarrierTag);
                
                // Add a gameplay tag to prevent hack progress gain
                FGameplayTag PreventHackProgressTag = FGameplayTag::RequestGameplayTag(FName(TEXT("State.PreventHackProgress")));
                BarrierEffect->InheritableOwnedTagsContainer.AddTag(PreventHackProgressTag);
                
                // Add a gameplay tag to prevent being targeted by quickhacks
                FGameplayTag ImmuneToQuickHacksTag = FGameplayTag::RequestGameplayTag(FName(TEXT("State.ImmuneToQuickHacks")));
                BarrierEffect->InheritableOwnedTagsContainer.AddTag(ImmuneToQuickHacksTag);
                
                // Apply the barrier effect to self
                FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
                SourceASC->ApplyGameplayEffectToSelf(BarrierEffect, 1.0f, EffectContext);
                
                // Prevent the character from initiating new quick hacks during the barrier
                FGameplayTagContainer QuickHackTags;
                QuickHackTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.QuickHack")));
                
                FTimerHandle ReenableQuickHacksTimerHandle;
                
                // Disable quick hack abilities
                for (const FGameplayAbilitySpec& AbilitySpec : SourceASC->GetActivatableAbilities())
                {
                    UGameplayAbility* Ability = AbilitySpec.Ability;
                    if (Ability && Ability->AbilityTags.HasAny(QuickHackTags))
                    {
                       // SourceASC->SetAbilityEnabled(AbilitySpec.Handle, false);
                    }
                }
                
                // Re-enable quick hack abilities after duration
                FTimerDelegate ReenableDelegate;
                ReenableDelegate.BindLambda([SourceASC, QuickHackTags]()
                {
                    for (const FGameplayAbilitySpec& AbilitySpec : SourceASC->GetActivatableAbilities())
                    {
                        UGameplayAbility* Ability = AbilitySpec.Ability;
                        if (Ability && Ability->AbilityTags.HasAny(QuickHackTags))
                        {
                            //SourceASC->SetAbilityEnabled(AbilitySpec.Handle, true);
                        }
                    }
                });
                
                GetWorld()->GetTimerManager().SetTimer(
                    ReenableQuickHacksTimerHandle,
                    ReenableDelegate,
                    Duration,
                    false
                );
            }
        }
    }
}

