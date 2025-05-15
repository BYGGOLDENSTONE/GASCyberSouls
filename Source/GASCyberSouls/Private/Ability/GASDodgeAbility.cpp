// copyright GASCyberSouls

#include "Ability/GASDodgeAbility.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Character/GASCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

UGASDodgeAbility::UGASDodgeAbility()
{
	// Default values
	DodgeDuration = 0.5f;
	CooldownTime = 3.0f;
	DodgeDistance = 300.0f;
	
	// Set the ability tags
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Dodge")));
	
	// Set instancing policy
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
}

void UGASDodgeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// Call parent implementation
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	// Add a gameplay tag to indicate dodging state
	FGameplayTag DodgingTag = FGameplayTag::RequestGameplayTag(FName("State.Dodging"));
	FGameplayTagContainer DodgingTagContainer;
	DodgingTagContainer.AddTag(DodgingTag);
	
	GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTags(DodgingTagContainer);
	
	// Perform the dodge movement
	if (ActorInfo->AvatarActor.IsValid())
	{
		ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
		if (Character)
		{
			// Determine dodge direction - away from the player
			FVector DodgeDirection = Character->GetActorForwardVector() * -1.0f;
			
			// Find nearest player to determine better dodge direction
			APlayerController* PC = GetWorld()->GetFirstPlayerController();
			if (PC && PC->GetPawn())
			{
				FVector PlayerDirection = PC->GetPawn()->GetActorLocation() - Character->GetActorLocation();
				PlayerDirection.Normalize();
				
				// Dodge perpendicular to player direction
				DodgeDirection = FVector::CrossProduct(PlayerDirection, FVector(0, 0, 1));
				
				// Randomly choose left or right perpendicular direction
				if (FMath::RandBool())
				{
					DodgeDirection *= -1.0f;
				}
			}
			
			// Apply dodge impulse
			Character->LaunchCharacter(DodgeDirection * DodgeDistance, true, true);
			
			// Play a montage or visual effect here
			UE_LOG(LogTemp, Display, TEXT("Dodge ability activated"));
		}
	}
	
	// Set a timer to end the ability after duration
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &UGASDodgeAbility::EndAbility, Handle, ActorInfo, ActivationInfo, true, false);
	
	ActorInfo->AbilitySystemComponent->GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		TimerDelegate,
		DodgeDuration,
		false
	);
}

void UGASDodgeAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// Remove the dodging tag
	FGameplayTag DodgingTag = FGameplayTag::RequestGameplayTag(FName("State.Dodging"));
	FGameplayTagContainer DodgingTagContainer;
	DodgingTagContainer.AddTag(DodgingTag);
	
	if (ActorInfo && ActorInfo->AbilitySystemComponent.IsValid())
	{
		ActorInfo->AbilitySystemComponent->RemoveLooseGameplayTags(DodgingTagContainer);
	}
	
	// Apply cooldown
	if (!bWasCancelled && CooldownTime > 0.0f)
	{
		UGameplayEffect* CooldownEffect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("DodgeCooldown")));
		CooldownEffect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
		CooldownEffect->DurationMagnitude = FScalableFloat(CooldownTime);
		
		// Add the cooldown tag to the effect
		FGameplayTag CooldownTag = FGameplayTag::RequestGameplayTag(FName(TEXT("Ability.Dodge.Cooldown")));
		CooldownEffect->InheritableOwnedTagsContainer.AddTag(CooldownTag);
		
		// Apply the cooldown effect
		FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
		GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectToSelf(CooldownEffect, 1.0f, EffectContext);
	}
	
	// Log the end of the ability
	UE_LOG(LogTemp, Display, TEXT("Dodge ability ended"));
	
	// Call parent implementation
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}