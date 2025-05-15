// copyright GASCyberSouls

#include "Character/GASTargetingComponent.h"
#include "Character/GASCharacterBase.h"
#include "Enemy/GASEnemyCharacter.h"
#include "Game/GASCyberSoulsHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"

UGASTargetingComponent::UGASTargetingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	CurrentTarget = nullptr;
	CurrentBodyPart = EBodyPartType::None;
	MaxTargetingDistance = 1000.0f;
	MaxTargetingAngle = 45.0f;
	bDrawDebug = false;
}

void UGASTargetingComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UGASTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Update the target list regularly
	if (GetOwner()->GetLocalRole() == ROLE_Authority || GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		FindTargetsInRange();
	}
	
	// Debug drawing
	if (bDrawDebug && CurrentTarget)
	{
		// Draw a line to the current target
		AActor* Owner = GetOwner();
		if (Owner)
		{
			DrawDebugLine(
				GetWorld(),
				Owner->GetActorLocation(),
				CurrentTarget->GetActorLocation(),
				FColor::Red,
				false,
				0.0f,
				0,
				2.0f
			);
			
			// Draw a sphere at body part position
			FVector BodyPartLocation = CurrentTarget->GetActorLocation();
			switch (CurrentBodyPart)
			{
				case EBodyPartType::UpperBody:
					BodyPartLocation.Z += 50.0f;
					break;
				case EBodyPartType::LowerBody:
					BodyPartLocation.Z -= 50.0f;
					break;
				case EBodyPartType::RightLeg:
					BodyPartLocation.Z -= 50.0f;
					BodyPartLocation.Y += 20.0f;
					break;
				case EBodyPartType::LeftLeg:
					BodyPartLocation.Z -= 50.0f;
					BodyPartLocation.Y -= 20.0f;
					break;
				default:
					break;
			}
			
			DrawDebugSphere(
				GetWorld(),
				BodyPartLocation,
				20.0f,
				16,
				FColor::Green,
				false,
				0.0f,
				0,
				1.0f
			);
		}
	}
}

bool UGASTargetingComponent::LockOnTarget()
{
	// If we already have a target, release it first
	if (CurrentTarget)
	{
		ReleaseTarget();
	}
	
	// Find the best target to lock onto
	CurrentTarget = FindBestTarget();
	
	// If we found a target, default to targeting the upper body
	if (CurrentTarget)
	{
		CurrentBodyPart = EBodyPartType::UpperBody;
		
		// Update the HUD
		AGASCyberSoulsHUD* HUD = AGASCyberSoulsHUD::GetInstance(GetWorld());
		if (HUD)
		{
			HUD->SetTargetingReticleVisible(true);
			HUD->UpdateTargetingReticle(CurrentTarget, CurrentBodyPart);
		}
		
		return true;
	}
	
	return false;
}

void UGASTargetingComponent::ReleaseTarget()
{
	// Update the HUD
	AGASCyberSoulsHUD* HUD = AGASCyberSoulsHUD::GetInstance(GetWorld());
	if (HUD)
	{
		HUD->SetTargetingReticleVisible(false);
	}
	
	CurrentTarget = nullptr;
	CurrentBodyPart = EBodyPartType::None;
}

bool UGASTargetingComponent::CycleTargetLeft()
{
	if (PotentialTargets.Num() <= 1)
	{
		return false;
	}
	
	// If we don't have a current target, just select the best one
	if (!CurrentTarget)
	{
		return LockOnTarget();
	}
	
	// Find the current target index
	int32 CurrentIndex = PotentialTargets.Find(CurrentTarget);
	if (CurrentIndex == INDEX_NONE)
	{
		// Our current target is no longer valid, find a new one
		return LockOnTarget();
	}
	
	// Cycle to the previous target
	int32 NewIndex = (CurrentIndex - 1 + PotentialTargets.Num()) % PotentialTargets.Num();
	CurrentTarget = PotentialTargets[NewIndex];
	CurrentBodyPart = EBodyPartType::UpperBody; // Reset to upper body when changing targets
	
	// Update the HUD
	AGASCyberSoulsHUD* HUD = AGASCyberSoulsHUD::GetInstance(GetWorld());
	if (HUD)
	{
		HUD->UpdateTargetingReticle(CurrentTarget, CurrentBodyPart);
	}
	
	return true;
}

bool UGASTargetingComponent::CycleTargetRight()
{
	if (PotentialTargets.Num() <= 1)
	{
		return false;
	}
	
	// If we don't have a current target, just select the best one
	if (!CurrentTarget)
	{
		return LockOnTarget();
	}
	
	// Find the current target index
	int32 CurrentIndex = PotentialTargets.Find(CurrentTarget);
	if (CurrentIndex == INDEX_NONE)
	{
		// Our current target is no longer valid, find a new one
		return LockOnTarget();
	}
	
	// Cycle to the next target
	int32 NewIndex = (CurrentIndex + 1) % PotentialTargets.Num();
	CurrentTarget = PotentialTargets[NewIndex];
	CurrentBodyPart = EBodyPartType::UpperBody; // Reset to upper body when changing targets
	
	// Update the HUD
	AGASCyberSoulsHUD* HUD = AGASCyberSoulsHUD::GetInstance(GetWorld());
	if (HUD)
	{
		HUD->UpdateTargetingReticle(CurrentTarget, CurrentBodyPart);
	}
	
	return true;
}

bool UGASTargetingComponent::SetTargetBodyPart(EBodyPartType BodyPart)
{
	if (!CurrentTarget)
	{
		return false;
	}
	
	CurrentBodyPart = BodyPart;
	
	// Update the HUD
	AGASCyberSoulsHUD* HUD = AGASCyberSoulsHUD::GetInstance(GetWorld());
	if (HUD)
	{
		HUD->UpdateTargetingReticle(CurrentTarget, CurrentBodyPart);
	}
	
	return true;
}

bool UGASTargetingComponent::CycleBodyPart(float MouseY)
{
	if (!CurrentTarget)
	{
		return false;
	}
	
	// Use mouse Y movement to cycle between upper and lower body
	if (MouseY > 0.5f)
	{
		// Mouse moved down, target lower body
		if (CurrentBodyPart == EBodyPartType::UpperBody)
		{
			CurrentBodyPart = EBodyPartType::LowerBody;
		}
		else if (CurrentBodyPart == EBodyPartType::LowerBody)
		{
			// Already at lower body, choose a specific leg
			CurrentBodyPart = (FMath::RandBool()) ? EBodyPartType::RightLeg : EBodyPartType::LeftLeg;
		}
	}
	else if (MouseY < -0.5f)
	{
		// Mouse moved up, target upper body
		if (CurrentBodyPart == EBodyPartType::LowerBody || 
			CurrentBodyPart == EBodyPartType::RightLeg || 
			CurrentBodyPart == EBodyPartType::LeftLeg)
		{
			CurrentBodyPart = EBodyPartType::UpperBody;
		}
	}
	
	// Update the HUD
	AGASCyberSoulsHUD* HUD = AGASCyberSoulsHUD::GetInstance(GetWorld());
	if (HUD)
	{
		HUD->UpdateTargetingReticle(CurrentTarget, CurrentBodyPart);
	}
	
	return true;
}

void UGASTargetingComponent::FindTargetsInRange()
{
	PotentialTargets.Empty();
	
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}
	
	// Get all characters in the world
	TArray<AActor*> Characters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGASCharacterBase::StaticClass(), Characters);
	
	// Get owner location and forward vector
	FVector OwnerLocation = Owner->GetActorLocation();
	FVector OwnerForward = Owner->GetActorForwardVector();
	
	// Filter for valid targets within range and angle
	for (AActor* Character : Characters)
	{
		// Skip self
		if (Character == Owner)
		{
			continue;
		}
		
		// Check if character is within targeting distance
		FVector DirectionToTarget = Character->GetActorLocation() - OwnerLocation;
		float DistanceToTarget = DirectionToTarget.Size();
		
		if (DistanceToTarget <= MaxTargetingDistance)
		{
			// Check if character is within targeting angle
			DirectionToTarget.Normalize();
			float DotProduct = FVector::DotProduct(OwnerForward, DirectionToTarget);
			float AngleToTarget = FMath::Acos(DotProduct) * 180.0f / PI;
			
			if (AngleToTarget <= MaxTargetingAngle)
			{
				// Add to potential targets
				AGASCharacterBase* TargetCharacter = Cast<AGASCharacterBase>(Character);
				if (TargetCharacter)
				{
					PotentialTargets.Add(TargetCharacter);
				}
			}
		}
	}
	
	// Check if current target is still valid
	if (CurrentTarget && !PotentialTargets.Contains(CurrentTarget))
	{
		ReleaseTarget();
	}
}

AGASCharacterBase* UGASTargetingComponent::FindBestTarget()
{
	if (PotentialTargets.Num() == 0)
	{
		return nullptr;
	}
	
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return nullptr;
	}
	
	AGASCharacterBase* BestTarget = nullptr;
	float BestScore = -1.0f;
	
	FVector OwnerLocation = Owner->GetActorLocation();
	FVector OwnerForward = Owner->GetActorForwardVector();
	
	// Find the target with the best combination of distance and angle
	for (AGASCharacterBase* Target : PotentialTargets)
	{
		FVector DirectionToTarget = Target->GetActorLocation() - OwnerLocation;
		float DistanceToTarget = DirectionToTarget.Size();
		
		DirectionToTarget.Normalize();
		float DotProduct = FVector::DotProduct(OwnerForward, DirectionToTarget);
		
		// Score is higher for targets that are closer and more in front of the player
		float DistanceScore = 1.0f - (DistanceToTarget / MaxTargetingDistance);
		float AngleScore = (DotProduct + 1.0f) / 2.0f; // Convert from [-1, 1] to [0, 1]
		
		// Combine scores - weight angle more than distance
		float Score = AngleScore * 0.7f + DistanceScore * 0.3f;
		
		if (Score > BestScore)
		{
			BestScore = Score;
			BestTarget = Target;
		}
	}
	
	return BestTarget;
}