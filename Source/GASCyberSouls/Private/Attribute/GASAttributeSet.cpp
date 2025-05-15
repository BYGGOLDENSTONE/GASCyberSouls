// copyright GASCyberSouls

#include "Attribute/GASAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UGASAttributeSet::UGASAttributeSet()
{
	// Initialize base attribute values
	MaxHealth = 100.0f;
	Health = 100.0f;
	MaxStamina = 100.0f;
	Stamina = 100.0f;
	AttackPower = 10.0f;
	Defense = 5.0f;
}

void UGASAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// Get the Target actor
	AActor* TargetActor = nullptr;
	AController* TargetController = nullptr;
	
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
	}

	// Get the Source actor
	AActor* SourceActor = nullptr;
	AController* SourceController = nullptr;
	
	if (Data.EffectSpec.GetContext().GetInstigator())
	{
		SourceActor = Data.EffectSpec.GetContext().GetInstigator();
		
		if (SourceActor)
		{
			SourceController = Cast<AController>(SourceActor);
			if (!SourceController && SourceActor->GetInstigatorController())
			{
				SourceController = SourceActor->GetInstigatorController();
			}
		}
	}

	// Handle Health attribute changes
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// Clamp health to [0, MaxHealth]
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
		
		// Check for death condition
		if (GetHealth() <= 0.0f)
		{
			// Handle character death here if needed
		}
	}
	// Handle Stamina attribute changes
	else if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		// Clamp stamina to [0, MaxStamina]
		SetStamina(FMath::Clamp(GetStamina(), 0.0f, GetMaxStamina()));
	}
}