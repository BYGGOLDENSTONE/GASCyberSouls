// copyright GASCyberSouls

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GASAttributeSet.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * Base Attribute Set for GASCyberSouls game
 * Contains basic attributes like Health, Stamina, etc.
 */
UCLASS()
class GASCYBERSOULS_API UGASAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UGASAttributeSet();

	// Called after attribute change
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
	void OnRep_MaxHackProgress(const FGameplayAttributeData& OldMaxHackProgress);
	void OnRep_BlockCharge(const FGameplayAttributeData& OldBlockCharge);
	void OnRep_MaxBlockCharge(const FGameplayAttributeData& OldMaxBlockCharge);
	void OnRep_DodgeCharge(const FGameplayAttributeData& OldDodgeCharge);
	void OnRep_MaxDodgeCharge(const FGameplayAttributeData& OldMaxDodgeCharge);
	void OnRep_QuickHackProgress(const FGameplayAttributeData& OldQuickHackProgress);
	void OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed);
	void OnRep_SlashSpeed(const FGameplayAttributeData& OldSlashSpeed);
	void OnRep_QuickHackSpeed(const FGameplayAttributeData& OldQuickHackSpeed);


	// Integrity attribute (player's health in CyberSouls)
	UPROPERTY(BlueprintReadOnly, Category = "CyberSouls|Attributes")
	FGameplayAttributeData Integrity;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, Integrity);
	
	// Max Integrity attribute
	UPROPERTY(BlueprintReadOnly, Category = "CyberSouls|Attributes")
	FGameplayAttributeData MaxIntegrity;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, MaxIntegrity);
	
	// HackProgress attribute (0-100)
	UPROPERTY(BlueprintReadOnly, Category = "CyberSouls|Attributes")
	FGameplayAttributeData HackProgress;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, HackProgress);
	
	// Max HackProgress attribute
	UPROPERTY(BlueprintReadOnly, Category = "CyberSouls|Attributes")
	FGameplayAttributeData MaxHackProgress;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, MaxHackProgress);
	
	// BlockCharge attribute (for enemies with block ability)
	UPROPERTY(BlueprintReadOnly, Category = "CyberSouls|Attributes")
	FGameplayAttributeData BlockCharge;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, BlockCharge);
	
	// MaxBlockCharge attribute
	UPROPERTY(BlueprintReadOnly, Category = "CyberSouls|Attributes")
	FGameplayAttributeData MaxBlockCharge;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, MaxBlockCharge);
	
	// DodgeCharge attribute (for enemies with dodge ability)
	UPROPERTY(BlueprintReadOnly, Category = "CyberSouls|Attributes")
	FGameplayAttributeData DodgeCharge;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, DodgeCharge);
	
	// MaxDodgeCharge attribute
	UPROPERTY(BlueprintReadOnly, Category = "CyberSouls|Attributes")
	FGameplayAttributeData MaxDodgeCharge;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, MaxDodgeCharge);
	
	// QuickHackProgress attribute (for tracking casting progress)
	UPROPERTY(BlueprintReadOnly, Category = "CyberSouls|Attributes")
	FGameplayAttributeData QuickHackProgress;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, QuickHackProgress);
	
	// AttackSpeed attribute (cooldown for Attack ability)
	UPROPERTY(BlueprintReadOnly, Category = "CyberSouls|Attributes")
	FGameplayAttributeData AttackSpeed;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, AttackSpeed);
	
	// SlashSpeed attribute (cooldown for Slash ability)
	UPROPERTY(BlueprintReadOnly, Category = "CyberSouls|Attributes")
	FGameplayAttributeData SlashSpeed;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, SlashSpeed);
	
	// QuickHackSpeed attribute (cooldown for QuickHack abilities)
	UPROPERTY(BlueprintReadOnly, Category = "CyberSouls|Attributes")
	FGameplayAttributeData QuickHackSpeed;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, QuickHackSpeed);
};