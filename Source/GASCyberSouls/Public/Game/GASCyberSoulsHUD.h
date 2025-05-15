// copyright GASCyberSouls

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Character/GASTypes.h"
#include "GASCyberSoulsHUD.generated.h"

class AGASCharacterBase;
class UTexture2D;

/**
 * Pure C++ HUD for GASCyberSouls
 * No widget blueprints required
 */
UCLASS()
class GASCYBERSOULS_API AGASCyberSoulsHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	AGASCyberSoulsHUD();
	
	// Called every frame to draw the HUD
	virtual void DrawHUD() override;
	
	// Update the targeting reticle
	void UpdateTargetingReticle(AGASCharacterBase* Target, EBodyPartType TargetedBodyPart);
	
	// Update the hack progress display
	void UpdateHackProgress(float CurrentProgress, float MaxProgress);
	
	// Update the integrity display
	void UpdateIntegrity(float CurrentIntegrity, float MaxIntegrity);
	
	// Show/hide the targeting reticle
	void SetTargetingReticleVisible(bool bVisible);
	
	// Show/hide the hack progress bar
	void SetHackProgressVisible(bool bVisible);
	
	// Show a QuickHack notification
	void ShowQuickHackNotification(EQuickHackType QuickHackType, float Duration = 3.0f);
	
	// Get the instance of the HUD
	static AGASCyberSoulsHUD* GetInstance(UWorld* World);

protected:
	// Textures for the HUD elements
	UPROPERTY(EditDefaultsOnly, Category = "GAS|HUD|Textures")
	UTexture2D* ReticleTexture;
	
	UPROPERTY(EditDefaultsOnly, Category = "GAS|HUD|Textures")
	UTexture2D* UpperBodyTexture;
	
	UPROPERTY(EditDefaultsOnly, Category = "GAS|HUD|Textures")
	UTexture2D* LowerBodyTexture;
	
	UPROPERTY(EditDefaultsOnly, Category = "GAS|HUD|Textures")
	UTexture2D* LeftLegTexture;
	
	UPROPERTY(EditDefaultsOnly, Category = "GAS|HUD|Textures")
	UTexture2D* RightLegTexture;
	
	// Default textures for fallback
	UTexture2D* DefaultReticleTexture;
	UTexture2D* DefaultBodyPartTexture;
	
	// Current state
	bool bTargetingReticleVisible;
	bool bHackProgressVisible;
	bool bQuickHackNotificationVisible;
	
	AGASCharacterBase* CurrentTarget;
	EBodyPartType CurrentTargetedBodyPart;
	
	float HackProgressValue;
	float MaxHackProgressValue;
	
	float IntegrityValue;
	float MaxIntegrityValue;
	
	FString QuickHackNotificationText;
	FLinearColor QuickHackNotificationColor;
	
	// Timer handle for notifications
	FTimerHandle QuickHackNotificationTimer;
	
	// Hide the notification
	void HideQuickHackNotification();
	
	// Helper functions to draw HUD elements
	void DrawTargetingReticle();
	void DrawBodyPartIndicators();
	void DrawHackProgressBar();
	void DrawIntegrityBar();
	void DrawQuickHackNotification();
	
	// Helper to draw a progress bar
	void DrawProgressBar(float X, float Y, float Width, float Height, float Progress, 
	                     const FLinearColor& Color, const FString& Text);
	
	// Helper to create default textures if needed
	void EnsureDefaultTextures();
	
	// Static instance for easy access
	static AGASCyberSoulsHUD* Instance;
};