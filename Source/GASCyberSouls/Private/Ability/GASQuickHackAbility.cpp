// copyright GASCyberSouls

#include "Ability/GASQuickHackAbility.h"
#include "AbilitySystemComponent.h"
#include "Attribute/GASAttributeSet.h"
#include "GameplayEffect.h"
#include "GameFramework/Character.h"

UGASQuickHackAbility::UGASQuickHackAbility()
{
    // Default settings
    QuickHackType = EQuickHackType::None;
    bIsCasting = false;
    
    // Default values based on Game.txt
    // These will be overridden in blueprint subclasses for specific quickhacks
    CastTime = 5.0f;    // Default to Interrupt Protocol cast time
    Cooldown = 8.0f;    // Default to Interrupt Protocol cooldown
    Duration = 1.0f;    // Default to Interrupt Protocol duration
    
    // Set instancing policy - quickhacks should be instanced per execution
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
}

void UGASQuickHackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    // Call parent implementation
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }
    
    // Get ability system component
    UAbilitySystemComponent* AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get();
    if (!AbilitySystemComponent)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }
    
    // Set the casting flag
    bIsCasting = true;
    
    // Start the casting timer
    if (CastTime > 0.0f)
    {
        // Get the world from the actor info
        UWorld* World = ActorInfo->AbilitySystemComponent->GetWorld();
        if (World)
        {
            // Set the timer to apply the effect after cast time
            World->GetTimerManager().SetTimer(
                CastingTimerHandle,
                FTimerDelegate::CreateUObject(this, &UGASQuickHackAbility::OnQuickHackSucceeded),
                CastTime,
                false
            );
            
            // Optionally start a montage here
            // PlayMontage(...);
        }
    }
    else
    {
        // If cast time is 0, apply immediately
        OnQuickHackSucceeded();
    }
}

void UGASQuickHackAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    // If we're still casting and were cancelled, call the interrupted function
    if (bIsCasting && bWasCancelled)
    {
        OnQuickHackInterrupted();
    }
    
    // Clear the casting timer
    if (ActorInfo && ActorInfo->AbilitySystemComponent.IsValid())
    {
        UWorld* World = ActorInfo->AbilitySystemComponent->GetWorld();
        if (World)
        {
            World->GetTimerManager().ClearTimer(CastingTimerHandle);
        }
    }
    
    // Reset casting flag
    bIsCasting = false;
    
    // Call parent implementation
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGASQuickHackAbility::OnQuickHackSucceeded()
{
    // Apply the effect
    ApplyQuickHackEffect();
    
    // Reset casting flag
    bIsCasting = false;
    
    // Apply cooldown
    if (Cooldown > 0.0f)
    {
        UGameplayEffect* CooldownEffect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("QuickHackCooldown")));
        CooldownEffect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
        CooldownEffect->DurationMagnitude = FScalableFloat(Cooldown);
        
        // Add the cooldown tag to the effect
        FGameplayTag CooldownTag = FGameplayTag::RequestGameplayTag(FName(TEXT("Ability.QuickHack.Cooldown")));
        CooldownEffect->InheritableOwnedTagsContainer.AddTag(CooldownTag);
        
        // Apply the cooldown effect
        FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
        GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectToSelf(CooldownEffect, 1.0f, EffectContext);
    }
    
    // End the ability
    if (CurrentActorInfo)
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
    }
}

void UGASQuickHackAbility::OnQuickHackInterrupted()
{
    // Handle interruption (no effect applied)
    
    // Reset casting flag
    bIsCasting = false;
    
    // You can send an event or play a different montage here for interruption
}

void UGASQuickHackAbility::ApplyQuickHackEffect()
{
    // This will be overridden in subclasses for each specific quickhack type
    
    // Common implementation for all quickhacks
    if (CurrentActorInfo && CurrentActorInfo->AvatarActor.IsValid())
    {
        // Log the application of the quick hack
        UE_LOG(LogTemp, Display, TEXT("QuickHack applied: %s"), *GetNameSafe(this));
    }
}

FQuickHackData UGASQuickHackAbility::GetQuickHackData() const
{
    return FQuickHackData(QuickHackType, CastTime, Cooldown, Duration);
}