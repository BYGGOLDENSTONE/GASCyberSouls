// copyright GASCyberSouls

#include "Ability/GASSlashAbility.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Character/GASCharacterBase.h"
#include "Character/GASPlayerCharacter.h"
#include "Enemy/GASEnemyCharacter.h"
#include "Attribute/GASAttributeSet.h"
#include "Character/GASTargetingComponent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"

UGASSlashAbility::UGASSlashAbility()
{
	// Default values
	BaseDamage = 15.0f;
	BodyPartDamageMultiplier = 1.5f;
	SlashRange = 250.0f;
	CooldownTime = 0.8f;
	
	// Set the ability tags
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Slash")));
	
	// Set instancing policy
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
}

void UGASSlashAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	// Call parent implementation
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	// Get the player character
	AGASPlayerCharacter* PlayerCharacter = Cast<AGASPlayerCharacter>(ActorInfo->AvatarActor.Get());
	if (!PlayerCharacter)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	// Get the targeting component
	UGASTargetingComponent* TargetingComp = PlayerCharacter->GetTargetingComponent();
	if (!TargetingComp || !TargetingComp->HasTarget())
	{
		UE_LOG(LogTemp, Display, TEXT("No target for slash"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	// Check if target is in range
	AGASCharacterBase* TargetCharacter = TargetingComp->GetCurrentTarget();
	if (TargetCharacter)
	{
		float Distance = FVector::Distance(PlayerCharacter->GetActorLocation(), TargetCharacter->GetActorLocation());
		if (Distance > SlashRange)
		{
			UE_LOG(LogTemp, Display, TEXT("Target out of range for slash"));
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
			return;
		}
	}
	
	// Get the targeted body part
	EBodyPartType TargetedBodyPart = TargetingComp->GetCurrentBodyPart();
	
	// Play appropriate montage based on targeted body part
	UAnimMontage* MontageToPlay = nullptr;
	switch (TargetedBodyPart)
	{
		case EBodyPartType::UpperBody:
			MontageToPlay = UpperBodySlashMontage;
			break;
		case EBodyPartType::LowerBody:
			MontageToPlay = LowerBodySlashMontage;
			break;
		case EBodyPartType::LeftLeg:
			MontageToPlay = LeftLegSlashMontage;
			break;
		case EBodyPartType::RightLeg:
			MontageToPlay = RightLegSlashMontage;
			break;
		default:
			// Default to upper body
			MontageToPlay = UpperBodySlashMontage;
			break;
	}
	
	// Play the montage if available
	if (MontageToPlay)
	{
		ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
		if (Character)
		{
			UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
			if (AnimInstance)
			{
				// Set up a montage ended delegate to apply damage at the right time
				FOnMontageEnded MontageEndedDelegate;
				
				
				AnimInstance->Montage_Play(MontageToPlay, 1.0f);
				AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, MontageToPlay);
				
				UE_LOG(LogTemp, Display, TEXT("Playing slash montage for %s"), *UEnum::GetValueAsString(TargetedBodyPart));
			}
			else
			{
				// No anim instance, apply damage immediately
				ApplySlashDamage();
			}
		}
		else
		{
			// No character, apply damage immediately
			ApplySlashDamage();
		}
	}
	else
	{
		// No montage, apply damage immediately
		ApplySlashDamage();
	}
	
	// Apply cooldown
	if (CooldownTime > 0.0f)
	{
		UGameplayEffect* CooldownEffect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("SlashCooldown")));
		CooldownEffect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
		CooldownEffect->DurationMagnitude = FScalableFloat(CooldownTime);
		
		// Add the cooldown tag to the effect
		FGameplayTag CooldownTag = FGameplayTag::RequestGameplayTag(FName(TEXT("Ability.Slash.Cooldown")));
		CooldownEffect->InheritableOwnedTagsContainer.AddTag(CooldownTag);
		
		// Apply the cooldown effect
		FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
		GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectToSelf(CooldownEffect, 1.0f, EffectContext);
	}
}

void UGASSlashAbility::ApplySlashDamage()
{
	// Get the player character
	AGASPlayerCharacter* PlayerCharacter = Cast<AGASPlayerCharacter>(CurrentActorInfo->AvatarActor.Get());
	if (!PlayerCharacter)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}
	
	// Get the targeting component
	UGASTargetingComponent* TargetingComp = PlayerCharacter->GetTargetingComponent();
	if (!TargetingComp || !TargetingComp->HasTarget())
	{
		UE_LOG(LogTemp, Display, TEXT("No target for slash damage"));
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}
	
	// Get the target and targeted body part
	AGASCharacterBase* TargetCharacter = TargetingComp->GetCurrentTarget();
	EBodyPartType TargetedBodyPart = TargetingComp->GetCurrentBodyPart();
	
	if (TargetCharacter)
	{
		// Check if target is in range
		float Distance = FVector::Distance(PlayerCharacter->GetActorLocation(), TargetCharacter->GetActorLocation());
		if (Distance <= SlashRange)
		{
			// Check if the enemy can block or dodge the attack
			AGASEnemyCharacter* TargetEnemyCharacter = Cast<AGASEnemyCharacter>(TargetCharacter);
			bool bAttackHits = true;
			
			if (TargetEnemyCharacter)
			{
				// Check if the enemy can block and if the targeted body part is upper body
				if (TargetEnemyCharacter->bCanBlock && TargetedBodyPart == EBodyPartType::UpperBody)
				{
					// Check if the enemy has block charges left
					UAbilitySystemComponent* TargetASC = TargetCharacter->GetAbilitySystemComponent();
					if (TargetASC)
					{
						const UGASAttributeSet* AttributeSet = Cast<UGASAttributeSet>(
							TargetASC->GetAttributeSet(UGASAttributeSet::StaticClass()));
						if (AttributeSet && AttributeSet->GetBlockCharge() > 0.0f)
						{
							// Enemy blocks the attack
							UE_LOG(LogTemp, Display, TEXT("Enemy blocked the slash to upper body!"));
							bAttackHits = false;
							
							// Reduce block charge
							UGameplayEffect* BlockChargeEffect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("ReduceBlockCharge")));
							BlockChargeEffect->DurationPolicy = EGameplayEffectDurationType::Instant;
							
							int32 Idx = BlockChargeEffect->Modifiers.Num();
							BlockChargeEffect->Modifiers.SetNum(Idx + 1);
							FGameplayModifierInfo& ModifierInfo = BlockChargeEffect->Modifiers[Idx];
							
							ModifierInfo.ModifierMagnitude = FScalableFloat(-1.0f); // Reduce by 1
							ModifierInfo.ModifierOp = EGameplayModOp::Additive;
							ModifierInfo.Attribute = UGASAttributeSet::GetBlockChargeAttribute();
							
							// Apply the block charge reduction
							FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
							TargetASC->ApplyGameplayEffectToSelf(BlockChargeEffect, 1.0f, EffectContext);
						}
					}
				}
				// Check if the enemy can dodge and if the targeted body part is lower body
				else if (TargetEnemyCharacter->bCanDodge && 
					(TargetedBodyPart == EBodyPartType::LowerBody || 
					 TargetedBodyPart == EBodyPartType::LeftLeg || 
					 TargetedBodyPart == EBodyPartType::RightLeg))
				{
					// Check if the enemy has dodge charges left
					UAbilitySystemComponent* TargetASC = TargetCharacter->GetAbilitySystemComponent();
					if (TargetASC)
					{
						const UGASAttributeSet* AttributeSet = Cast<UGASAttributeSet>(
							TargetASC->GetAttributeSet(UGASAttributeSet::StaticClass()));
						if (AttributeSet && AttributeSet->GetDodgeCharge() > 0.0f)
						{
							// Enemy dodges the attack
							UE_LOG(LogTemp, Display, TEXT("Enemy dodged the slash to lower body!"));
							bAttackHits = false;
							
							// Reduce dodge charge
							UGameplayEffect* DodgeChargeEffect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("ReduceDodgeCharge")));
							DodgeChargeEffect->DurationPolicy = EGameplayEffectDurationType::Instant;
							
							int32 Idx = DodgeChargeEffect->Modifiers.Num();
							DodgeChargeEffect->Modifiers.SetNum(Idx + 1);
							FGameplayModifierInfo& ModifierInfo = DodgeChargeEffect->Modifiers[Idx];
							
							ModifierInfo.ModifierMagnitude = FScalableFloat(-1.0f); // Reduce by 1
							ModifierInfo.ModifierOp = EGameplayModOp::Additive;
							ModifierInfo.Attribute = UGASAttributeSet::GetDodgeChargeAttribute();
							
							// Apply the dodge charge reduction
							FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
							TargetASC->ApplyGameplayEffectToSelf(DodgeChargeEffect, 1.0f, EffectContext);
						}
					}
				}
			}
			
			// Apply damage if the attack hits
			if (bAttackHits)
			{
				// Calculate damage based on body part
				float FinalDamage = BaseDamage;
				
				// Apply bonus damage for specific body parts
				if (TargetedBodyPart == EBodyPartType::LeftLeg || TargetedBodyPart == EBodyPartType::RightLeg)
				{
					FinalDamage *= BodyPartDamageMultiplier;
					UE_LOG(LogTemp, Display, TEXT("Critical hit on leg! Damage increased to %f"), FinalDamage);
				}
				
				UE_LOG(LogTemp, Display, TEXT("Applying slash damage of %f to enemy"), FinalDamage);
				
				// Get the enemy's ability system component
				UAbilitySystemComponent* TargetASC = TargetCharacter->GetAbilitySystemComponent();
				if (TargetASC)
				{
					// Create a gameplay effect for damage
					UGameplayEffect* DamageEffect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("SlashDamage")));
					DamageEffect->DurationPolicy = EGameplayEffectDurationType::Instant;
					
					// Add a modifier to reduce Health
					int32 Idx = DamageEffect->Modifiers.Num();
					DamageEffect->Modifiers.SetNum(Idx + 1);
					FGameplayModifierInfo& ModifierInfo = DamageEffect->Modifiers[Idx];
					
					ModifierInfo.ModifierMagnitude = FScalableFloat(FinalDamage * -1); // Negative value to reduce attribute
					ModifierInfo.ModifierOp = EGameplayModOp::Additive;
					
					
					// Apply the damage effect
					FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
					TargetASC->ApplyGameplayEffectToSelf(DamageEffect, 1.0f, EffectContext);
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("Target out of range for slash damage"));
		}
	}
	
	// End the ability
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}