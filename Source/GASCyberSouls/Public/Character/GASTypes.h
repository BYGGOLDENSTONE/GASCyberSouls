// copyright GASCyberSouls

#pragma once

#include "CoreMinimal.h"
#include "GASTypes.generated.h"

UENUM(BlueprintType)
enum class EBodyPartType : uint8
{
	None UMETA(DisplayName = "None"),
	UpperBody UMETA(DisplayName = "Upper Body"),
	LowerBody UMETA(DisplayName = "Lower Body"),
	RightLeg UMETA(DisplayName = "Right Leg"),
	LeftLeg UMETA(DisplayName = "Left Leg")
};

UENUM(BlueprintType)
enum class EQuickHackType : uint8
{
	None UMETA(DisplayName = "None"),
	InterruptProtocol UMETA(DisplayName = "Interrupt Protocol"),
	SystemFreeze UMETA(DisplayName = "System Freeze"),
	FirewallBarrier UMETA(DisplayName = "Firewall Barrier")
};

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	None UMETA(DisplayName = "None"),
	// Physical enemies
	Basic UMETA(DisplayName = "Basic Enemy"),
	Block UMETA(DisplayName = "Block Enemy"),
	Dodge UMETA(DisplayName = "Dodge Enemy"),
	// Hacking enemies
	Netrunner UMETA(DisplayName = "Netrunner"),
	BuffNetrunner UMETA(DisplayName = "Buff Netrunner"),
	DebuffNetrunner UMETA(DisplayName = "Debuff Netrunner")
};

// Struct to store QuickHack details
USTRUCT(BlueprintType)
struct FQuickHackData
{
	GENERATED_BODY()
	
	// Type of quickhack
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EQuickHackType Type = EQuickHackType::None;
	
	// Cast time in seconds
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CastTime = 0.0f;
	
	// Cooldown in seconds
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Cooldown = 0.0f;
	
	// Duration of the effect
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration = 0.0f;
	
	FQuickHackData() {}
	
	FQuickHackData(EQuickHackType InType, float InCastTime, float InCooldown, float InDuration)
		: Type(InType)
		, CastTime(InCastTime)
		, Cooldown(InCooldown)
		, Duration(InDuration)
	{}
};