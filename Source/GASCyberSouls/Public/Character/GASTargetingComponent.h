// copyright GASCyberSouls

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GASTypes.h"
#include "GASTargetingComponent.generated.h"

class AGASCharacterBase;

/**
 * Component that handles targeting functionality for CyberSouls
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GASCYBERSOULS_API UGASTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGASTargetingComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Target lock functionality
	UFUNCTION(BlueprintCallable, Category = "GAS|Targeting")
	bool LockOnTarget();
	
	// Release current target
	UFUNCTION(BlueprintCallable, Category = "GAS|Targeting")
	void ReleaseTarget();
	
	// Change to next target (to the left)
	UFUNCTION(BlueprintCallable, Category = "GAS|Targeting")
	bool CycleTargetLeft();
	
	// Change to next target (to the right)
	UFUNCTION(BlueprintCallable, Category = "GAS|Targeting")
	bool CycleTargetRight();
	
	// Change to an enemy's specific body part
	UFUNCTION(BlueprintCallable, Category = "GAS|Targeting")
	bool SetTargetBodyPart(EBodyPartType BodyPart);
	
	// Cycle through body parts based on mouse movement
	UFUNCTION(BlueprintCallable, Category = "GAS|Targeting")
	bool CycleBodyPart(float MouseY);
	
	// Get current target actor
	UFUNCTION(BlueprintCallable, Category = "GAS|Targeting")
	AGASCharacterBase* GetCurrentTarget() const { return CurrentTarget; }
	
	// Get current targeted body part
	UFUNCTION(BlueprintCallable, Category = "GAS|Targeting")
	EBodyPartType GetCurrentBodyPart() const { return CurrentBodyPart; }
	
	// Check if we currently have a target
	UFUNCTION(BlueprintCallable, Category = "GAS|Targeting")
	bool HasTarget() const { return CurrentTarget != nullptr; }

private:
	// Find potential targets in range
	void FindTargetsInRange();
	
	// Find the best target from the available targets
	AGASCharacterBase* FindBestTarget();
	
	// The currently targeted character
	UPROPERTY()
	AGASCharacterBase* CurrentTarget;
	
	// The currently targeted body part
	UPROPERTY()
	EBodyPartType CurrentBodyPart;
	
	// List of potential targets
	UPROPERTY()
	TArray<AGASCharacterBase*> PotentialTargets;
	
	// Maximum distance for targeting
	UPROPERTY(EditDefaultsOnly, Category = "GAS|Targeting")
	float MaxTargetingDistance = 1000.0f;
	
	// Angle in degrees for targeting
	UPROPERTY(EditDefaultsOnly, Category = "GAS|Targeting")
	float MaxTargetingAngle = 45.0f;
	
	// Debug drawing
	UPROPERTY(EditDefaultsOnly, Category = "GAS|Targeting|Debug")
	bool bDrawDebug = false;
};