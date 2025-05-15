// copyright GASCyberSouls

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Game/GASCyberSoulsHUD.h"
#include "GameplayEffectTypes.h"
#include "GASCharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayAbility;

/**
 * Base Character class for GASCyberSouls game
 * This serves as the base for any character that will use the Gameplay Ability System
 */
UCLASS()
class GASCYBERSOULS_API AGASCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGASCharacterBase();

	// Implement IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Gameplay Ability System Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UAttributeSet* AttributeSet;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called during possession by a controller
	virtual void PossessedBy(AController* NewController) override;
	
	// Called when a player controller begins interacting with this pawn
	virtual void OnRep_PlayerState() override;
	
	// Initialize the ability system component
	virtual void InitializeAbilitySystem();
	
	// Add starting abilities to this character
	virtual void AddStartingAbilities();

	// Abilities to grant to this character when it spawns
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilitySystem")
	TArray<TSubclassOf<UGameplayAbility>> StartingAbilities;
};