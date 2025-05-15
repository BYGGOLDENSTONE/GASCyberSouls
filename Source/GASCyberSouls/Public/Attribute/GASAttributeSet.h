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
	
	// Health attribute
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, Health);
	
	// Maximum Health attribute
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, MaxHealth);
	
	// Stamina attribute
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, Stamina);
	
	// Maximum Stamina attribute
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, MaxStamina);
	
	// Attack Power attribute
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, AttackPower);
	
	// Defense attribute
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Defense;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, Defense);
};