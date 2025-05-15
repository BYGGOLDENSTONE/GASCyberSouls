// copyright GASCyberSouls


// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/GASCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "GameplayAbilitySpec.h"

// Sets default values
AGASCharacterBase::AGASCharacterBase()
{
 	// Set this character to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;

	// Create ability system component
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	
	// Create attribute set
	AttributeSet = CreateDefaultSubobject<UAttributeSet>(TEXT("AttributeSet"));
}

// Returns the ability system component
UAbilitySystemComponent* AGASCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void AGASCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called to bind functionality to input
void AGASCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// Called when possessed by controller - used by player characters
void AGASCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Initialize ability system for server
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		InitializeAbilitySystem();
	}
}

// Called when player state is replicated - used by client characters
void AGASCharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Initialize ability system for client
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		InitializeAbilitySystem();
	}
}

// Initialize ability system and grant starting abilities
void AGASCharacterBase::InitializeAbilitySystem()
{
	if (GetLocalRole() == ROLE_Authority && AbilitySystemComponent)
	{
		// Grant starting abilities
		AddStartingAbilities();
	}
}

// Add the starting abilities defined in the blueprint
void AGASCharacterBase::AddStartingAbilities()
{
	if (!AbilitySystemComponent || GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	// Grant abilities
	for (TSubclassOf<UGameplayAbility>& StartingAbility : StartingAbilities)
	{
		if (StartingAbility)
		{
			AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(StartingAbility, 1, INDEX_NONE, this));
		}
	}
}