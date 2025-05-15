// copyright GASCyberSouls

#include "Ability/GASSystemFreezeAbility.h"
#include "AbilitySystemComponent.h"
#include "Character/GASCharacterBase.h"
#include "GameplayTagContainer.h"
#include "GameFramework/CharacterMovementComponent.h"

UGASSystemFreezeAbility::UGASSystemFreezeAbility()
{
    QuickHackType = EQuickHackType::SystemFreeze;
    CastTime = 7.0f;  // As specified in Game.txt
    Cooldown = 14.0f; // As specified in Game.txt
    Duration = 2.0f;  // As specified in Game.txt
    
    // Set the ability tags
    AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.QuickHack.SystemFreeze")));
}

void UGASSystemFreezeAbility::ApplyQuickHackEffect()
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
            // Create a gameplay effect for the freeze
            UGameplayEffect* FreezeEffect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("SystemFreeze")));
            FreezeEffect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
            FreezeEffect->DurationMagnitude = FScalableFloat(Duration);
            
            // Add the frozen tag to the effect
            FGameplayTag FrozenTag = FGameplayTag::RequestGameplayTag(FName(TEXT("State.Frozen")));
            FreezeEffect->InheritableOwnedTagsContainer.AddTag(FrozenTag);
            
            // Prevent movement and ability activation during freeze
            FGameplayTag PreventMovementTag = FGameplayTag::RequestGameplayTag(FName(TEXT("State.PreventMovement")));
            FreezeEffect->InheritableOwnedTagsContainer.AddTag(PreventMovementTag);
            
            // Apply the freeze effect to the target
            FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
            TargetASC->ApplyGameplayEffectToSelf(FreezeEffect, 1.0f, EffectContext);
            
            // Directly freeze the movement component
            UCharacterMovementComponent* MovementComp = TargetCharacter->GetCharacterMovement();
            if (MovementComp)
            {
                // Store original max walk speed to restore later
                float OriginalSpeed = MovementComp->MaxWalkSpeed;
                
                // Set speed to 0
                MovementComp->MaxWalkSpeed = 0.0f;
                
                // Schedule restoration of original speed after duration
                FTimerHandle UnfreezeTimerHandle;
                FTimerDelegate UnfreezeDelegate;
                UnfreezeDelegate.BindLambda([MovementComp, OriginalSpeed]()
                {
                    MovementComp->MaxWalkSpeed = OriginalSpeed;
                });
                
                GetWorld()->GetTimerManager().SetTimer(
                    UnfreezeTimerHandle,
                    UnfreezeDelegate,
                    Duration,
                    false
                );
            }
        }
    }
}