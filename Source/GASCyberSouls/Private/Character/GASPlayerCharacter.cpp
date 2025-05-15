// copyright GASCyberSouls

#include "Character/GASPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystemComponent.h"
#include "Attribute/GASAttributeSet.h"
#include "GAS/GASAbilitySystemComponent.h"
#include "Character/GASTargetingComponent.h"
#include "Character/GASTypes.h"

AGASPlayerCharacter::AGASPlayerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Set up character movement properties
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	
	// Create targeting component
	TargetingComponent = CreateDefaultSubobject<UGASTargetingComponent>(TEXT("TargetingComponent"));


}

void AGASPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AGASPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings using Enhanced Input system
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		// Movement
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGASPlayerCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGASPlayerCharacter::Look);

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		
		// Attack
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AGASPlayerCharacter::Attack);
		
		// Dodge
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &AGASPlayerCharacter::Dodge);
		
		// CyberSouls-specific inputs
		
		// Target Lock (Tab key)
		if (TargetLockAction)
		{
			EnhancedInputComponent->BindAction(TargetLockAction, ETriggerEvent::Triggered, this, &AGASPlayerCharacter::TargetLock);
		}
		
		// Target Left (Q key)
		if (TargetLeftAction)
		{
			EnhancedInputComponent->BindAction(TargetLeftAction, ETriggerEvent::Triggered, this, &AGASPlayerCharacter::TargetLeft);
		}
		
		// Target Right (E key)
		if (TargetRightAction)
		{
			EnhancedInputComponent->BindAction(TargetRightAction, ETriggerEvent::Triggered, this, &AGASPlayerCharacter::TargetRight);
		}
		
		// Target Part (mouse movement is handled in Look function)
		
		// QuickHack (general action)
		if (QuickHackAction)
		{
			EnhancedInputComponent->BindAction(QuickHackAction, ETriggerEvent::Triggered, this, &AGASPlayerCharacter::QuickHack);
		}
		
		// Specific QuickHacks
		if (QuickHack1Action)
		{
			EnhancedInputComponent->BindAction(QuickHack1Action, ETriggerEvent::Triggered, this, &AGASPlayerCharacter::QuickHack1);
		}
		
		if (QuickHack2Action)
		{
			EnhancedInputComponent->BindAction(QuickHack2Action, ETriggerEvent::Triggered, this, &AGASPlayerCharacter::QuickHack2);
		}
		
		if (QuickHack3Action)
		{
			EnhancedInputComponent->BindAction(QuickHack3Action, ETriggerEvent::Triggered, this, &AGASPlayerCharacter::QuickHack3);
		}
	}
}

void AGASPlayerCharacter::Move(const FInputActionValue& Value)
{
	// Get movement input vector
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AGASPlayerCharacter::Look(const FInputActionValue& Value)
{
	// Get look input vector
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
		
		// If we have a target, we can use mouse Y to cycle body parts
		if (TargetingComponent && TargetingComponent->HasTarget())
		{
			TargetingComponent->CycleBodyPart(LookAxisVector.Y);
		}
	}
}

void AGASPlayerCharacter::Attack(const FInputActionValue& Value)
{
	// Here we can activate attack abilities through GAS
	if (AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Display, TEXT("Attack action triggered!"));
		
		// Try to activate the Slash ability
		FGameplayTagContainer SlashTag;
		SlashTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Slash")));
		
		// Cast to our custom ASC
		UGASAbilitySystemComponent* GasASC = Cast<UGASAbilitySystemComponent>(AbilitySystemComponent);
		if (GasASC)
		{
			GasASC->TryActivateAbilityByTag(SlashTag);
		}
	}
}

void AGASPlayerCharacter::Dodge(const FInputActionValue& Value)
{
	// Here we can activate dodge abilities through GAS
	if (AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Display, TEXT("Dodge action triggered!"));
		
		// Try to activate the Dodge ability
		FGameplayTagContainer DodgeTag;
		DodgeTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Dodge")));
		
		// Cast to our custom ASC
		UGASAbilitySystemComponent* GasASC = Cast<UGASAbilitySystemComponent>(AbilitySystemComponent);
		if (GasASC)
		{
			GasASC->TryActivateAbilityByTag(DodgeTag);
		}
	}
}

void AGASPlayerCharacter::TargetLock(const FInputActionValue& Value)
{
	if (TargetingComponent)
	{
		if (TargetingComponent->HasTarget())
		{
			TargetingComponent->ReleaseTarget();
			UE_LOG(LogTemp, Display, TEXT("Target released"));
		}
		else
		{
			bool bSuccess = TargetingComponent->LockOnTarget();
			if (bSuccess)
			{
				UE_LOG(LogTemp, Display, TEXT("Target locked"));
			}
			else
			{
				UE_LOG(LogTemp, Display, TEXT("No valid target found"));
			}
		}
	}
}

void AGASPlayerCharacter::TargetLeft(const FInputActionValue& Value)
{
	if (TargetingComponent && TargetingComponent->HasTarget())
	{
		TargetingComponent->CycleTargetLeft();
		UE_LOG(LogTemp, Display, TEXT("Target cycled left"));
	}
}

void AGASPlayerCharacter::TargetRight(const FInputActionValue& Value)
{
	if (TargetingComponent && TargetingComponent->HasTarget())
	{
		TargetingComponent->CycleTargetRight();
		UE_LOG(LogTemp, Display, TEXT("Target cycled right"));
	}
}

void AGASPlayerCharacter::TargetPart(const FInputActionValue& Value)
{
	// This would be used if you have a specific input for targeting body parts
	// For now, we're handling body part targeting in the Look function
}

void AGASPlayerCharacter::QuickHack(const FInputActionValue& Value)
{
	// General QuickHack action, could open a UI menu or select a default hack
	UE_LOG(LogTemp, Display, TEXT("QuickHack action triggered!"));
}

void AGASPlayerCharacter::QuickHack1(const FInputActionValue& Value)
{
	// Interrupt Protocol
	if (AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Display, TEXT("Interrupt Protocol QuickHack triggered!"));
		
		// Try to activate the Interrupt Protocol ability
		FGameplayTagContainer QuickHackTag;
		QuickHackTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.QuickHack.InterruptProtocol")));
		
		// Cast to our custom ASC
		UGASAbilitySystemComponent* GasASC = Cast<UGASAbilitySystemComponent>(AbilitySystemComponent);
		if (GasASC)
		{
			GasASC->TryActivateAbilityByTag(QuickHackTag);
		}
	}
}

void AGASPlayerCharacter::QuickHack2(const FInputActionValue& Value)
{
	// System Freeze
	if (AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Display, TEXT("System Freeze QuickHack triggered!"));
		
		// Try to activate the System Freeze ability
		FGameplayTagContainer QuickHackTag;
		QuickHackTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.QuickHack.SystemFreeze")));
		
		// Cast to our custom ASC
		UGASAbilitySystemComponent* GasASC = Cast<UGASAbilitySystemComponent>(AbilitySystemComponent);
		if (GasASC)
		{
			GasASC->TryActivateAbilityByTag(QuickHackTag);
		}
	}
}

void AGASPlayerCharacter::QuickHack3(const FInputActionValue& Value)
{
	// Firewall Barrier
	if (AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Display, TEXT("Firewall Barrier QuickHack triggered!"));
		
		// Try to activate the Firewall Barrier ability
		FGameplayTagContainer QuickHackTag;
		QuickHackTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.QuickHack.FirewallBarrier")));
		
		// Cast to our custom ASC
		UGASAbilitySystemComponent* GasASC = Cast<UGASAbilitySystemComponent>(AbilitySystemComponent);
		if (GasASC)
		{
			GasASC->TryActivateAbilityByTag(QuickHackTag);
		}
	}
}

void AGASPlayerCharacter::InitializeAbilitySystem()
{
	// Call the parent implementation first
	Super::InitializeAbilitySystem();
	
	// Add any player-specific initialization here
	if (GetLocalRole() == ROLE_Authority && AbilitySystemComponent)
	{
		// Cast to our custom ASC
		UGASAbilitySystemComponent* GasASC = Cast<UGASAbilitySystemComponent>(AbilitySystemComponent);
		if (GasASC)
		{
			// In a real implementation, you'd use TSubclassOf<UGASGameplayAbility> variables
			// that would be set in the editor, rather than logging
			UE_LOG(LogTemp, Display, TEXT("Player granted Slash ability"));
			UE_LOG(LogTemp, Display, TEXT("Player granted QuickHack abilities"));
		}
	}
}