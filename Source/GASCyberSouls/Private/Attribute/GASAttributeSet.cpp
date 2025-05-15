// copyright GASCyberSouls

#include "Attribute/GASAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "Game/GASCyberSoulsHUD.h"
#include "Kismet/GameplayStatics.h"

UGASAttributeSet::UGASAttributeSet()
{
	
	// Initialize CyberSouls attribute values
	MaxIntegrity = 100.0f;
	Integrity = 100.0f;
	MaxHackProgress = 100.0f;
	HackProgress = 0.0f;
	MaxBlockCharge = 3.0f;
	BlockCharge = 3.0f;
	MaxDodgeCharge = 3.0f;
	DodgeCharge = 3.0f;
	QuickHackProgress = 0.0f;
	AttackSpeed = 1.0f;
	SlashSpeed = 1.0f;
	QuickHackSpeed = 1.0f;
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

	
	// Handle Integrity attribute changes
	else if (Data.EvaluatedData.Attribute == GetIntegrityAttribute())
	{
		// Clamp integrity to [0, MaxIntegrity]
		SetIntegrity(FMath::Clamp(GetIntegrity(), 0.0f, GetMaxIntegrity()));
		
		// Update the HUD
		AGASCyberSoulsHUD* HUD = AGASCyberSoulsHUD::GetInstance(GetWorld());
		if (HUD)
		{
			HUD->UpdateIntegrity(GetIntegrity(), GetMaxIntegrity());
			
			// Check for player death condition
			if (GetIntegrity() <= 0.0f)
			{
				// Handle player death here if needed
				UE_LOG(LogTemp, Warning, TEXT("INTEGRITY REACHED 0 - Player is dead!"));
			}
		}
	}
	// Handle HackProgress attribute changes
	else if (Data.EvaluatedData.Attribute == GetHackProgressAttribute())
	{
		// Clamp hack progress to [0, MaxHackProgress]
		SetHackProgress(FMath::Clamp(GetHackProgress(), 0.0f, GetMaxHackProgress()));
		
		// Update the HUD
		AGASCyberSoulsHUD* HUD = AGASCyberSoulsHUD::GetInstance(GetWorld());
		if (HUD)
		{
			HUD->UpdateHackProgress(GetHackProgress(), GetMaxHackProgress());
			
			// Check if hack is complete
			if (GetHackProgress() >= GetMaxHackProgress())
			{
				// Handle completed hack
				UE_LOG(LogTemp, Warning, TEXT("HACK PROGRESS REACHED 100% - Player is fully hacked!"));
			}
			
			// Show the hack progress bar when progress is greater than 0
			if (GetHackProgress() > 0.0f)
			{
				HUD->SetHackProgressVisible(true);
			}
			else
			{
				HUD->SetHackProgressVisible(false);
			}
		}
	}
	// Handle BlockCharge attribute changes
	else if (Data.EvaluatedData.Attribute == GetBlockChargeAttribute())
	{
		// Clamp block charge to [0, MaxBlockCharge]
		SetBlockCharge(FMath::Clamp(GetBlockCharge(), 0.0f, GetMaxBlockCharge()));
		
		// You could add HUD updates for block charge if needed
	}
	// Handle DodgeCharge attribute changes
	else if (Data.EvaluatedData.Attribute == GetDodgeChargeAttribute())
	{
		// Clamp dodge charge to [0, MaxDodgeCharge]
		SetDodgeCharge(FMath::Clamp(GetDodgeCharge(), 0.0f, GetMaxDodgeCharge()));
		
		// You could add HUD updates for dodge charge if needed
	}
	// Handle QuickHackProgress attribute changes
	else if (Data.EvaluatedData.Attribute == GetQuickHackProgressAttribute())
	{
		// You could add HUD updates for quickhack progress if needed
	}
}

void UGASAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate health and stamina
	
	// Replicate CyberSouls attributes
	DOREPLIFETIME(UGASAttributeSet, Integrity);
	DOREPLIFETIME(UGASAttributeSet, MaxIntegrity);
	DOREPLIFETIME(UGASAttributeSet, HackProgress);
	DOREPLIFETIME(UGASAttributeSet, MaxHackProgress);
	DOREPLIFETIME(UGASAttributeSet, BlockCharge);
	DOREPLIFETIME(UGASAttributeSet, MaxBlockCharge);
	DOREPLIFETIME(UGASAttributeSet, DodgeCharge);
	DOREPLIFETIME(UGASAttributeSet, MaxDodgeCharge);
	DOREPLIFETIME(UGASAttributeSet, QuickHackProgress);
	DOREPLIFETIME(UGASAttributeSet, AttackSpeed);
	DOREPLIFETIME(UGASAttributeSet, SlashSpeed);
	DOREPLIFETIME(UGASAttributeSet, QuickHackSpeed);
}



void UGASAttributeSet::OnRep_Integrity(const FGameplayAttributeData& OldIntegrity)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, Integrity, OldIntegrity);
	
	// Update the HUD when integrity changes on clients
	AGASCyberSoulsHUD* HUD = AGASCyberSoulsHUD::GetInstance(GetWorld());
	if (HUD)
	{
		HUD->UpdateIntegrity(GetIntegrity(), GetMaxIntegrity());
	}
}

void UGASAttributeSet::OnRep_MaxIntegrity(const FGameplayAttributeData& OldMaxIntegrity)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, MaxIntegrity, OldMaxIntegrity);
	
	// Update the HUD when max integrity changes on clients
	AGASCyberSoulsHUD* HUD = AGASCyberSoulsHUD::GetInstance(GetWorld());
	if (HUD)
	{
		HUD->UpdateIntegrity(GetIntegrity(), GetMaxIntegrity());
	}
}

void UGASAttributeSet::OnRep_HackProgress(const FGameplayAttributeData& OldHackProgress)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, HackProgress, OldHackProgress);
	
	// Update the HUD when hack progress changes on clients
	AGASCyberSoulsHUD* HUD = AGASCyberSoulsHUD::GetInstance(GetWorld());
	if (HUD)
	{
		HUD->UpdateHackProgress(GetHackProgress(), GetMaxHackProgress());
		
		// Show the hack progress bar when progress is greater than 0
		if (GetHackProgress() > 0.0f)
		{
			HUD->SetHackProgressVisible(true);
		}
		else
		{
			HUD->SetHackProgressVisible(false);
		}
	}
}

void UGASAttributeSet::OnRep_MaxHackProgress(const FGameplayAttributeData& OldMaxHackProgress)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, MaxHackProgress, OldMaxHackProgress);
	
	// Update the HUD when max hack progress changes on clients
	AGASCyberSoulsHUD* HUD = AGASCyberSoulsHUD::GetInstance(GetWorld());
	if (HUD)
	{
		HUD->UpdateHackProgress(GetHackProgress(), GetMaxHackProgress());
	}
}

void UGASAttributeSet::OnRep_BlockCharge(const FGameplayAttributeData& OldBlockCharge)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, BlockCharge, OldBlockCharge);
}

void UGASAttributeSet::OnRep_MaxBlockCharge(const FGameplayAttributeData& OldMaxBlockCharge)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, MaxBlockCharge, OldMaxBlockCharge);
}

void UGASAttributeSet::OnRep_DodgeCharge(const FGameplayAttributeData& OldDodgeCharge)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, DodgeCharge, OldDodgeCharge);
}

void UGASAttributeSet::OnRep_MaxDodgeCharge(const FGameplayAttributeData& OldMaxDodgeCharge)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, MaxDodgeCharge, OldMaxDodgeCharge);
}

void UGASAttributeSet::OnRep_QuickHackProgress(const FGameplayAttributeData& OldQuickHackProgress)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, QuickHackProgress, OldQuickHackProgress);
}

void UGASAttributeSet::OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, AttackSpeed, OldAttackSpeed);
}

void UGASAttributeSet::OnRep_SlashSpeed(const FGameplayAttributeData& OldSlashSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, SlashSpeed, OldSlashSpeed);
}

void UGASAttributeSet::OnRep_QuickHackSpeed(const FGameplayAttributeData& OldQuickHackSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, QuickHackSpeed, OldQuickHackSpeed);
}