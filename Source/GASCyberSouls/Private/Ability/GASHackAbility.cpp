// copyright GASCyberSouls

#include "Ability/GASHackAbility.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Character/GASCharacterBase.h"
#include "Attribute/GASAttributeSet.h"

UGASHackAbility::UGASHackAbility()
{
	// Default values
	HackProgressPerSecond = 2.0f;
	HackRange = 800.0f;
	TickInterval = 1.0f;
	
	// Set the ability tags
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Hack")));
	
	// Hack is a continuous ability
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGASHackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// Call parent implementation
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	// Add a gameplay tag to indicate hacking state
	FGameplayTag HackingTag = FGameplayTag::RequestGameplayTag(FName("State.Hacking"));
	FGameplayTagContainer HackingTagContainer;
	HackingTagContainer.AddTag(HackingTag);
	
	GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTags(HackingTagContainer);
	
	// Start timer to apply hack progress periodically
	if (ActorInfo && ActorInfo->AbilitySystemComponent.IsValid())
	{
		UWorld* World = ActorInfo->AbilitySystemComponent->GetWorld();
		if (World)
		{
			World->GetTimerManager().SetTimer(
				HackTimerHandle,
				this,
				&UGASHackAbility::ApplyHackProgress,
				TickInterval,
				true
			);
		}
	}
	
	// Play a montage or visual effect here
	UE_LOG(LogTemp, Display, TEXT("Hack ability activated"));
}

void UGASHackAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// Clear the timer
	if (ActorInfo && ActorInfo->AbilitySystemComponent.IsValid())
	{
		UWorld* World = ActorInfo->AbilitySystemComponent->GetWorld();
		if (World)
		{
			World->GetTimerManager().ClearTimer(HackTimerHandle);
		}
	}
	
	// Remove the hacking tag
	FGameplayTag HackingTag = FGameplayTag::RequestGameplayTag(FName("State.Hacking"));
	FGameplayTagContainer HackingTagContainer;
	HackingTagContainer.AddTag(HackingTag);
	
	if (ActorInfo && ActorInfo->AbilitySystemComponent.IsValid())
	{
		ActorInfo->AbilitySystemComponent->RemoveLooseGameplayTags(HackingTagContainer);
	}
	
	// Log the end of the ability
	UE_LOG(LogTemp, Display, TEXT("Hack ability ended"));
	
	// Call parent implementation
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGASHackAbility::ApplyHackProgress()
{
	if (!CurrentActorInfo || !CurrentActorInfo->AvatarActor.IsValid())
	{
		return;
	}
	
	AActor* SourceActor = CurrentActorInfo->AvatarActor.Get();
	
	// Find the player to hack
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC && PC->GetPawn())
	{
		APawn* PlayerPawn = PC->GetPawn();
		
		// Check if player is in range
		float Distance = FVector::Distance(SourceActor->GetActorLocation(), PlayerPawn->GetActorLocation());
		if (Distance <= HackRange)
		{
			// Check if the player has the Gameplay Ability System component
			AGASCharacterBase* PlayerCharacter = Cast<AGASCharacterBase>(PlayerPawn);
			if (PlayerCharacter)
			{
				UAbilitySystemComponent* PlayerASC = PlayerCharacter->GetAbilitySystemComponent();
				if (PlayerASC)
				{
					// Check if player has the PreventHackProgress tag
					FGameplayTag PreventHackProgressTag = FGameplayTag::RequestGameplayTag(FName("State.PreventHackProgress"));
					if (!PlayerASC->HasMatchingGameplayTag(PreventHackProgressTag))
					{
						// Create a gameplay effect to increase HackProgress
						UGameplayEffect* HackEffect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("IncreaseHackProgress")));
						HackEffect->DurationPolicy = EGameplayEffectDurationType::Instant;
						
						int32 Idx = HackEffect->Modifiers.Num();
						HackEffect->Modifiers.SetNum(Idx + 1);
						FGameplayModifierInfo& ModifierInfo = HackEffect->Modifiers[Idx];
						
						// The value per tick is the amount per second times the tick interval
						ModifierInfo.ModifierMagnitude = FScalableFloat(HackProgressPerSecond * TickInterval);
						ModifierInfo.ModifierOp = EGameplayModOp::Additive;
						ModifierInfo.Attribute = UGASAttributeSet::GetHackProgressAttribute();
						
						// Apply the hack effect
						FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
						PlayerASC->ApplyGameplayEffectToSelf(HackEffect, 1.0f, EffectContext);
						
						// Log the hack progress
						UE_LOG(LogTemp, Display, TEXT("Applying hack progress: %f"), HackProgressPerSecond * TickInterval);
					}
					else
					{
						UE_LOG(LogTemp, Display, TEXT("Player is protected from hack progress"));
					}
				}
			}
		}
		else
		{
			// Player out of range, end the ability
			UE_LOG(LogTemp, Display, TEXT("Player out of range, ending hack ability"));
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		}
	}
}