// copyright GASCyberSouls

#include "Ability/GASAttackAbility.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Character/GASCharacterBase.h"
#include "Attribute/GASAttributeSet.h"
#include "Character/GASPlayerCharacter.h"
#include "Game/GASGameplayTagsSetup.h"
#include "GameplayTags.h"
#include "Character/GASTargetingComponent.h"
#include "Enemy/GASEnemyCharacter.h"

UGASAttackAbility::UGASAttackAbility()
{
	// Default values
	BaseDamage = 10.0f;
	AttackRange = 200.0f;
	AttackRadius = 50.0f;
	CooldownTime = 1.0f;
	
	// Set the ability tags
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Attack")));
	
	// Set instancing policy
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
}

void UGASAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// Call parent implementation
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	// Apply damage
	ApplyDamage();
	
	// Apply cooldown
	if (CooldownTime > 0.0f)
	{
		UGameplayEffect* CooldownEffect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("AttackCooldown")));
		CooldownEffect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
		CooldownEffect->DurationMagnitude = FScalableFloat(CooldownTime);
		
		// Add the cooldown tag to the effect
		FGameplayTag CooldownTag = FGameplayTag::RequestGameplayTag(FName(TEXT("Ability.Attack.Cooldown")));
		CooldownEffect->InheritableOwnedTagsContainer.AddTag(CooldownTag);
		
		// Apply the cooldown effect
		FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
		GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectToSelf(CooldownEffect, 1.0f, EffectContext);
	}
	
	// End the ability
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UGASAttackAbility::ApplyDamage()
{

	if (!CurrentActorInfo || !CurrentActorInfo->AvatarActor.IsValid())
	{
		return;
	}
	
	AActor* SourceActor = CurrentActorInfo->AvatarActor.Get();
	
	// If this is an enemy attacking the player, find the player in range
	AGASEnemyCharacter* EnemyCharacter = Cast<AGASEnemyCharacter>(SourceActor);
	if (EnemyCharacter)
	{
		// Find player in range
		TArray<AActor*> OverlappingActors;
		SourceActor->GetOverlappingActors(OverlappingActors, AGASCharacterBase::StaticClass());
		
		for (AActor* OverlappingActor : OverlappingActors)
		{
			// Skip self
			if (OverlappingActor == SourceActor)
			{
				continue;
			}
			
			// Check if it's the player
			AGASCharacterBase* TargetCharacter = Cast<AGASCharacterBase>(OverlappingActor);
			if (TargetCharacter && !Cast<AGASEnemyCharacter>(TargetCharacter))
			{
				// Check if in range
				float Distance = FVector::Distance(SourceActor->GetActorLocation(), TargetCharacter->GetActorLocation());
				if (Distance <= AttackRange)
				{
					// Apply damage to the player's Integrity
					UE_LOG(LogTemp, Display, TEXT("Enemy attacking player, reducing Integrity by %f"), BaseDamage);
					
					// Get the player's ability system component
					UAbilitySystemComponent* TargetASC = TargetCharacter->GetAbilitySystemComponent();
					if (TargetASC)
					{
						// Create a gameplay effect for damage
						UGameplayEffect* DamageEffect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("EnemyAttackDamage")));
						DamageEffect->DurationPolicy = EGameplayEffectDurationType::Instant;
						
						// Add a modifier to reduce Integrity
						int32 Idx = DamageEffect->Modifiers.Num();
						DamageEffect->Modifiers.SetNum(Idx + 1);
						FGameplayModifierInfo& ModifierInfo = DamageEffect->Modifiers[Idx];
						
						ModifierInfo.ModifierMagnitude = FScalableFloat(BaseDamage * -1); // Negative value to reduce attribute
						ModifierInfo.ModifierOp = EGameplayModOp::Additive;
						ModifierInfo.Attribute = UGASAttributeSet::GetIntegrityAttribute();
						
						// Apply the damage effect
						FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
						TargetASC->ApplyGameplayEffectToSelf(DamageEffect, 1.0f, EffectContext);
					}
					
					// Only apply to the first valid target
					break;
				}
			}
		}
	}
	else
	{
		// This is the player attacking an enemy
		// Get the targeted enemy from the player's targeting component
		AGASPlayerCharacter* PlayerCharacter = Cast<AGASPlayerCharacter>(SourceActor);
		if (PlayerCharacter)
		{
			UGASTargetingComponent* TargetingComp = PlayerCharacter->GetTargetingComponent();
			if (TargetingComp && TargetingComp->HasTarget())
			{
				AGASCharacterBase* TargetCharacter = TargetingComp->GetCurrentTarget();
				if (TargetCharacter)
				{
					// Check if in range
					float Distance = FVector::Distance(SourceActor->GetActorLocation(), TargetCharacter->GetActorLocation());
					if (Distance <= AttackRange)
					{
						// Check if the enemy can block or dodge the attack
						AGASEnemyCharacter* TargetEnemyCharacter = Cast<AGASEnemyCharacter>(TargetCharacter);
						bool bAttackHits = true;
						
						if (TargetEnemyCharacter)
						{
							// Check which body part is targeted
							EBodyPartType TargetedBodyPart = TargetingComp->GetCurrentBodyPart();
							
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
										UE_LOG(LogTemp, Display, TEXT("Enemy blocked the attack to upper body!"));
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
										UE_LOG(LogTemp, Display, TEXT("Enemy dodged the attack to lower body!"));
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
							UE_LOG(LogTemp, Display, TEXT("Player attacking enemy, reducing Health by %f"), BaseDamage);
							
							// Get the enemy's ability system component
							UAbilitySystemComponent* TargetASC = TargetCharacter->GetAbilitySystemComponent();
							if (TargetASC)
							{
								// Create a gameplay effect for damage
								UGameplayEffect* DamageEffect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("PlayerAttackDamage")));
								DamageEffect->DurationPolicy = EGameplayEffectDurationType::Instant;
								
								// Add a modifier to reduce Health
								int32 Idx = DamageEffect->Modifiers.Num();
								DamageEffect->Modifiers.SetNum(Idx + 1);
								FGameplayModifierInfo& ModifierInfo = DamageEffect->Modifiers[Idx];
								
								ModifierInfo.ModifierMagnitude = FScalableFloat(BaseDamage * -1); // Negative value to reduce attribute
								ModifierInfo.ModifierOp = EGameplayModOp::Additive;
								
								
								// Apply the damage effect
								FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
								TargetASC->ApplyGameplayEffectToSelf(DamageEffect, 1.0f, EffectContext);
							}
						}
					}
					else
					{
						UE_LOG(LogTemp, Display, TEXT("Target out of range for attack"));
					}
				}
			}
			else
			{
				UE_LOG(LogTemp, Display, TEXT("No target selected for attack"));
			}
		}
	}
}
