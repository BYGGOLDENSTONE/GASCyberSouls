// copyright GASCyberSouls

#pragma once

#include "CoreMinimal.h"
#include "Character/GASCharacterBase.h"
#include "Character/GASTypes.h"
#include "GASEnemyCharacter.generated.h"

/**
 * Base enemy character class for GASCyberSouls
 */
UCLASS()
class GASCYBERSOULS_API AGASEnemyCharacter : public AGASCharacterBase
{
	GENERATED_BODY()
	
public:
	AGASEnemyCharacter();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Type of enemy
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	EEnemyType EnemyType;
	
	// Determine if this enemy can perform hacking
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	bool bCanHack;
	
	// Determine if this enemy can block attacks
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	bool bCanBlock;
	
	// Determine if this enemy can dodge attacks
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	bool bCanDodge;
	
	// Attack Range for this enemy
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	float AttackRange;
	
	// Hacking Range for this enemy
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	float HackRange;
	
	// Get the current targeted body part
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	EBodyPartType GetTargetedBodyPart() const;
	
	// Set the current targeted body part
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void SetTargetedBodyPart(EBodyPartType NewBodyPart);
	
protected:
	// Currently targeted body part (when player is targeting this enemy)
	UPROPERTY(ReplicatedUsing = OnRep_TargetedBodyPart)
	EBodyPartType TargetedBodyPart;
	
	// Replicated property for targeted body part
	UFUNCTION()
	void OnRep_TargetedBodyPart();
	
	// Called to try to perform an attack
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	virtual void TryAttack();
	
	// Called to try to perform a block
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	virtual void TryBlock();
	
	// Called to try to perform a dodge
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	virtual void TryDodge();
	
	// Called to try to perform a hack
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	virtual void TryHack();
	
	// Called to try to perform a quick hack
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	virtual void TryQuickHack(EQuickHackType QuickHackType);
	
	// Setup AI behavior
	virtual void SetupAIBehavior();
	
	// Initialize abilities specific to this enemy type
	virtual void AddEnemyTypeAbilities();
	
	// Override initialization
	virtual void InitializeAbilitySystem() override;
};