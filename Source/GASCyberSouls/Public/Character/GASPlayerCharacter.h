// copyright GASCyberSouls

#pragma once

#include "CoreMinimal.h"
#include "GASCharacterBase.h"
#include "GASPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UGASTargetingComponent;

/**
 * Player Character class for GASCyberSouls game
 * Extends the base GAS character with player-specific functionality
 */
UCLASS()
class GASCYBERSOULS_API AGASPlayerCharacter : public AGASCharacterBase
{
	GENERATED_BODY()
	
public:
	AGASPlayerCharacter();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Returns Camera boom component
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	
	// Returns Follow camera component
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
	// Returns Targeting component
	FORCEINLINE class UGASTargetingComponent* GetTargetingComponent() const { return TargetingComponent; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* FollowCamera;
	
	/** Targeting component for CyberSouls gameplay */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay)
	UGASTargetingComponent* TargetingComponent;
	
	/** Enhanced Input System Components */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* JumpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* AttackAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* DodgeAction;
	
	// New CyberSouls-specific input actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|CyberSouls")
	UInputAction* TargetLockAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|CyberSouls")
	UInputAction* TargetLeftAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|CyberSouls")
	UInputAction* TargetRightAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|CyberSouls")
	UInputAction* QuickHackAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|CyberSouls")
	UInputAction* QuickHack1Action;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|CyberSouls")
	UInputAction* QuickHack2Action;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|CyberSouls")
	UInputAction* QuickHack3Action;
	
	// Input callback functions
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);
	void Dodge(const FInputActionValue& Value);
	
	// New CyberSouls-specific input functions
	void TargetLock(const FInputActionValue& Value);
	void TargetLeft(const FInputActionValue& Value);
	void TargetRight(const FInputActionValue& Value);
	void TargetPart(const FInputActionValue& Value);
	void QuickHack(const FInputActionValue& Value);
	void QuickHack1(const FInputActionValue& Value);
	void QuickHack2(const FInputActionValue& Value);
	void QuickHack3(const FInputActionValue& Value);
	
	// Override to add player-specific ability system initialization
	virtual void InitializeAbilitySystem() override;
};