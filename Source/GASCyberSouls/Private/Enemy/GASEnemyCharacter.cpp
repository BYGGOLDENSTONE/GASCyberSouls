// copyright GASCyberSouls

#include "Enemy/GASEnemyCharacter.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemComponent.h"
#include "Ability/GASQuickHackAbility.h"
#include "GAS/GASAbilitySystemComponent.h"
#include "Attribute/GASAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"

AGASEnemyCharacter::AGASEnemyCharacter()
{
	// Set default values
	EnemyType = EEnemyType::Basic;
	bCanHack = false;
	bCanBlock = false;
	bCanDodge = false;
	AttackRange = 200.0f;
	HackRange = 800.0f;
	TargetedBodyPart = EBodyPartType::None;
	
	// Set this character to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;
	
	// Configure character movement
	UCharacterMovementComponent* CharMoveComp = GetCharacterMovement();
	if (CharMoveComp)
	{
		CharMoveComp->bOrientRotationToMovement = true;
		CharMoveComp->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
		CharMoveComp->MaxWalkSpeed = 300.0f;
	}
}

void AGASEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Setup AI behavior if we're the server
	if (GetLocalRole() == ROLE_Authority)
	{
		SetupAIBehavior();
	}
}

void AGASEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Add any tick logic here
}

void AGASEnemyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	// Replicate targeted body part
	DOREPLIFETIME(AGASEnemyCharacter, TargetedBodyPart);
}

EBodyPartType AGASEnemyCharacter::GetTargetedBodyPart() const
{
	return TargetedBodyPart;
}

void AGASEnemyCharacter::SetTargetedBodyPart(EBodyPartType NewBodyPart)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		TargetedBodyPart = NewBodyPart;
	}
}

void AGASEnemyCharacter::OnRep_TargetedBodyPart()
{
	// Handle visuals for targeted body part here
	// For example, highlight the body part that's being targeted
}

void AGASEnemyCharacter::TryAttack()
{
	if (AbilitySystemComponent)
	{
		// Try to activate attack ability
		FGameplayTagContainer AttackTag;
		AttackTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Attack")));
		
		// Cast to our custom ASC
		UGASAbilitySystemComponent* GasASC = Cast<UGASAbilitySystemComponent>(AbilitySystemComponent);
		if (GasASC)
		{
			GasASC->TryActivateAbilityByTag(AttackTag);
		}
	}
}

void AGASEnemyCharacter::TryBlock()
{
	if (!bCanBlock)
	{
		return;
	}
	
	if (AbilitySystemComponent)
	{
		// Try to activate block ability
		FGameplayTagContainer BlockTag;
		BlockTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Block")));
		
		// Cast to our custom ASC
		UGASAbilitySystemComponent* GasASC = Cast<UGASAbilitySystemComponent>(AbilitySystemComponent);
		if (GasASC)
		{
			GasASC->TryActivateAbilityByTag(BlockTag);
		}
	}
}

void AGASEnemyCharacter::TryDodge()
{
	if (!bCanDodge)
	{
		return;
	}
	
	if (AbilitySystemComponent)
	{
		// Try to activate dodge ability
		FGameplayTagContainer DodgeTag;
		DodgeTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Dodge")));
		
		// Cast to our custom ASC
		UGASAbilitySystemComponent* GasASC = Cast<UGASAbilitySystemComponent>(AbilitySystemComponent);
		if (GasASC)
		{
			GasASC->TryActivateAbilityByTag(DodgeTag);
		}
	}
}

void AGASEnemyCharacter::TryHack()
{
	if (!bCanHack)
	{
		return;
	}
	
	if (AbilitySystemComponent)
	{
		// Try to activate hack ability
		FGameplayTagContainer HackTag;
		HackTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Hack")));
		
		// Cast to our custom ASC
		UGASAbilitySystemComponent* GasASC = Cast<UGASAbilitySystemComponent>(AbilitySystemComponent);
		if (GasASC)
		{
			GasASC->TryActivateAbilityByTag(HackTag);
		}
	}
}

void AGASEnemyCharacter::TryQuickHack(EQuickHackType QuickHackType)
{
	// Only specific enemy types can use QuickHacks
	if (EnemyType != EEnemyType::BuffNetrunner && EnemyType != EEnemyType::DebuffNetrunner)
	{
		return;
	}
	
	if (AbilitySystemComponent)
	{
		// Try to activate the specific QuickHack ability
		FGameplayTagContainer QuickHackTag;
		
		switch (QuickHackType)
		{
			case EQuickHackType::InterruptProtocol:
				QuickHackTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.QuickHack.InterruptProtocol")));
				break;
			case EQuickHackType::SystemFreeze:
				QuickHackTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.QuickHack.SystemFreeze")));
				break;
			case EQuickHackType::FirewallBarrier:
				QuickHackTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.QuickHack.FirewallBarrier")));
				break;
			default:
				return;
		}
		
		// Cast to our custom ASC
		UGASAbilitySystemComponent* GasASC = Cast<UGASAbilitySystemComponent>(AbilitySystemComponent);
		if (GasASC)
		{
			GasASC->TryActivateAbilityByTag(QuickHackTag);
		}
	}
}

void AGASEnemyCharacter::SetupAIBehavior()
{
	// This would be implemented to set up behaviors specific to the enemy type
	// For now, we'll just log that the enemy was spawned
	UE_LOG(LogTemp, Display, TEXT("%s enemy spawned"), *UEnum::GetValueAsString(EnemyType));
	
	// In a real implementation, you'd configure the AI controller here
	// For example, setting up behavior trees, blackboards, etc.
}

void AGASEnemyCharacter::AddEnemyTypeAbilities()
{
	// Early out if not authority
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent)
	{
		return;
	}
	
	// Cast to our custom ASC
	UGASAbilitySystemComponent* GasASC = Cast<UGASAbilitySystemComponent>(AbilitySystemComponent);
	if (!GasASC)
	{
		return;
	}
	
	// Add basic attack ability to all enemy types
	// Note: In a real implementation, you'd use TSubclassOf<UGASGameplayAbility> variables
	// that would be set in the editor, rather than hardcoding class references
	
	// For now, we'll just log what abilities would be granted
	switch (EnemyType)
	{
		case EEnemyType::Basic:
			// Basic enemy has only attack ability
			UE_LOG(LogTemp, Display, TEXT("Basic enemy granted Attack ability"));
			break;
			
		case EEnemyType::Block:
			// Block enemy has attack and block abilities
			UE_LOG(LogTemp, Display, TEXT("Block enemy granted Attack and Block abilities"));
			bCanBlock = true;
			break;
			
		case EEnemyType::Dodge:
			// Dodge enemy has attack and dodge abilities
			UE_LOG(LogTemp, Display, TEXT("Dodge enemy granted Attack and Dodge abilities"));
			bCanDodge = true;
			break;
			
		case EEnemyType::Netrunner:
			// Netrunner has hack ability
			UE_LOG(LogTemp, Display, TEXT("Netrunner granted Hack ability"));
			bCanHack = true;
			break;
			
		case EEnemyType::BuffNetrunner:
			// Buff Netrunner has hack and FirewallBarrier abilities
			UE_LOG(LogTemp, Display, TEXT("Buff Netrunner granted Hack and FirewallBarrier abilities"));
			bCanHack = true;
			break;
			
		case EEnemyType::DebuffNetrunner:
			// Debuff Netrunner has hack and SystemFreeze abilities
			UE_LOG(LogTemp, Display, TEXT("Debuff Netrunner granted Hack and SystemFreeze abilities"));
			bCanHack = true;
			break;
			
		default:
			break;
	}
	
	// All netrunners and above can use InterruptProtocol
	if (bCanHack)
	{
		UE_LOG(LogTemp, Display, TEXT("Netrunner granted InterruptProtocol ability"));
	}
}

void AGASEnemyCharacter::InitializeAbilitySystem()
{
	// Call parent implementation
	Super::InitializeAbilitySystem();
	
	// Add enemy-type specific abilities
	AddEnemyTypeAbilities();
}