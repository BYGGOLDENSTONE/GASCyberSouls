// copyright GASCyberSouls

#include "Game/GASCyberSoulsHUD.h"
#include "Character/GASCharacterBase.h"
#include "Enemy/GASEnemyCharacter.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

// Initialize static instance
AGASCyberSoulsHUD* AGASCyberSoulsHUD::Instance = nullptr;

AGASCyberSoulsHUD::AGASCyberSoulsHUD()
{
	// Initialize state
	bTargetingReticleVisible = false;
	bHackProgressVisible = false;
	bQuickHackNotificationVisible = false;
	
	CurrentTarget = nullptr;
	CurrentTargetedBodyPart = EBodyPartType::None;
	
	HackProgressValue = 0.0f;
	MaxHackProgressValue = 100.0f;
	
	IntegrityValue = 100.0f;
	MaxIntegrityValue = 100.0f;
	
	QuickHackNotificationText = "";
	QuickHackNotificationColor = FLinearColor(0.0f, 1.0f, 1.0f, 1.0f); // Cyan
	
	// Create default textures on first use
	DefaultReticleTexture = nullptr;
	DefaultBodyPartTexture = nullptr;
}

void AGASCyberSoulsHUD::DrawHUD()
{
	Super::DrawHUD();
	
	// Store this as the active instance
	Instance = this;
	
	// Ensure we have textures (even fallbacks)
	EnsureDefaultTextures();
	
	// Draw HUD elements
	if (bTargetingReticleVisible && CurrentTarget)
	{
		DrawTargetingReticle();
		DrawBodyPartIndicators();
	}
	
	if (bHackProgressVisible)
	{
		DrawHackProgressBar();
	}
	
	// Always draw integrity
	DrawIntegrityBar();
	
	if (bQuickHackNotificationVisible)
	{
		DrawQuickHackNotification();
	}
}

void AGASCyberSoulsHUD::EnsureDefaultTextures()
{
	// Create default reticle texture if needed
	if (!DefaultReticleTexture)
	{
		DefaultReticleTexture = UTexture2D::CreateTransient(64, 64);
		
		// Fill with white pixels
		uint8* Pixels = new uint8[64 * 64 * 4];
		for (int32 i = 0; i < 64 * 64; i++)
		{
			int32 X = i % 64;
			int32 Y = i / 64;
			
			// Create a simple crosshair
			bool bInCrosshair = (X == 32 || Y == 32) || 
				(X >= 30 && X <= 34 && Y >= 30 && Y <= 34);
			
			Pixels[i * 4] = bInCrosshair ? 255 : 0;     // R
			Pixels[i * 4 + 1] = bInCrosshair ? 255 : 0; // G
			Pixels[i * 4 + 2] = bInCrosshair ? 255 : 0; // B
			Pixels[i * 4 + 3] = bInCrosshair ? 255 : 0; // A
		}
		
		// Update the texture
		FTexture2DMipMap& Mip = DefaultReticleTexture->PlatformData->Mips[0];
		void* Data = Mip.BulkData.Lock(LOCK_READ_WRITE);
		FMemory::Memcpy(Data, Pixels, 64 * 64 * 4);
		Mip.BulkData.Unlock();
		DefaultReticleTexture->UpdateResource();
		
		delete[] Pixels;
	}
	
	// Create default body part texture if needed
	if (!DefaultBodyPartTexture)
	{
		DefaultBodyPartTexture = UTexture2D::CreateTransient(32, 32);
		
		// Fill with white pixels
		uint8* Pixels = new uint8[32 * 32 * 4];
		for (int32 i = 0; i < 32 * 32; i++)
		{
			int32 X = i % 32;
			int32 Y = i / 32;
			
			// Create a simple circle
			float CenterX = 16.0f;
			float CenterY = 16.0f;
			float Radius = 12.0f;
			
			float Distance = FMath::Sqrt(FMath::Square(X - CenterX) + FMath::Square(Y - CenterY));
			bool bInCircle = Distance <= Radius;
			
			Pixels[i * 4] = bInCircle ? 255 : 0;     // R
			Pixels[i * 4 + 1] = bInCircle ? 255 : 0; // G
			Pixels[i * 4 + 2] = bInCircle ? 255 : 0; // B
			Pixels[i * 4 + 3] = bInCircle ? 255 : 0; // A
		}
		
		// Update the texture
		FTexture2DMipMap& Mip = DefaultBodyPartTexture->PlatformData->Mips[0];
		void* Data = Mip.BulkData.Lock(LOCK_READ_WRITE);
		FMemory::Memcpy(Data, Pixels, 32 * 32 * 4);
		Mip.BulkData.Unlock();
		DefaultBodyPartTexture->UpdateResource();
		
		delete[] Pixels;
	}
}

AGASCyberSoulsHUD* AGASCyberSoulsHUD::GetInstance(UWorld* World)
{
	// If we already have an instance, return it
	if (Instance)
	{
		return Instance;
	}
	
	// Otherwise, get the HUD from the player controller
	APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
	if (PC)
	{
		return Cast<AGASCyberSoulsHUD>(PC->GetHUD());
	}
	
	return nullptr;
}

void AGASCyberSoulsHUD::UpdateTargetingReticle(AGASCharacterBase* Target, EBodyPartType TargetedBodyPart)
{
	CurrentTarget = Target;
	CurrentTargetedBodyPart = TargetedBodyPart;
	
	// Update the enemy's targeted body part
	AGASEnemyCharacter* EnemyCharacter = Cast<AGASEnemyCharacter>(Target);
	if (EnemyCharacter)
	{
		EnemyCharacter->SetTargetedBodyPart(TargetedBodyPart);
	}
}

void AGASCyberSoulsHUD::UpdateHackProgress(float CurrentProgress, float MaxProgress)
{
	HackProgressValue = CurrentProgress;
	MaxHackProgressValue = MaxProgress;
}

void AGASCyberSoulsHUD::UpdateIntegrity(float CurrentIntegrity, float MaxIntegrity)
{
	IntegrityValue = CurrentIntegrity;
	MaxIntegrityValue = MaxIntegrity;
}

void AGASCyberSoulsHUD::SetTargetingReticleVisible(bool bVisible)
{
	bTargetingReticleVisible = bVisible;
	
	// If hiding the reticle, clear the target
	if (!bVisible)
	{
		CurrentTarget = nullptr;
		CurrentTargetedBodyPart = EBodyPartType::None;
	}
}

void AGASCyberSoulsHUD::SetHackProgressVisible(bool bVisible)
{
	bHackProgressVisible = bVisible;
}

void AGASCyberSoulsHUD::ShowQuickHackNotification(EQuickHackType QuickHackType, float Duration)
{
	// Set the text based on the QuickHack type
	switch (QuickHackType)
	{
		case EQuickHackType::InterruptProtocol:
			QuickHackNotificationText = TEXT("QUICKHACK: Interrupt Protocol");
			break;
		case EQuickHackType::SystemFreeze:
			QuickHackNotificationText = TEXT("QUICKHACK: System Freeze");
			break;
		case EQuickHackType::FirewallBarrier:
			QuickHackNotificationText = TEXT("QUICKHACK: Firewall Barrier");
			break;
		default:
			QuickHackNotificationText = TEXT("QUICKHACK Activated");
			break;
	}
	
	// Show the notification
	bQuickHackNotificationVisible = true;
	
	// Set timer to hide the notification
	GetWorldTimerManager().ClearTimer(QuickHackNotificationTimer);
	GetWorldTimerManager().SetTimer(
		QuickHackNotificationTimer,
		this,
		&AGASCyberSoulsHUD::HideQuickHackNotification,
		Duration,
		false
	);
}

void AGASCyberSoulsHUD::HideQuickHackNotification()
{
	bQuickHackNotificationVisible = false;
}

void AGASCyberSoulsHUD::DrawTargetingReticle()
{
	if (!CurrentTarget || !Canvas)
	{
		return;
	}
	
	// Get the screen position of the target
	FVector TargetLocation = CurrentTarget->GetActorLocation();
	FVector2D ScreenPosition;
	
	// Project the 3D position to 2D screen position
	if (ProjectWorldLocationToScreen(TargetLocation, ScreenPosition))
	{
		// Use the texture or fallback to default
		UTexture2D* TextureToUse = ReticleTexture ? ReticleTexture : DefaultReticleTexture;
		
		// Draw the reticle
		DrawTexture(
			TextureToUse,
			ScreenPosition.X - 32.0f, // Center the texture
			ScreenPosition.Y - 32.0f,
			64.0f, // Width
			64.0f, // Height
			0.0f, 0.0f, 1.0f, 1.0f,
			FLinearColor(1.0f, 1.0f, 1.0f, 1.0f)
		);
	}
}

void AGASCyberSoulsHUD::DrawBodyPartIndicators()
{
	if (!CurrentTarget || !Canvas || CurrentTargetedBodyPart == EBodyPartType::None)
	{
		return;
	}
	
	// Get the screen position of the target
	FVector TargetLocation = CurrentTarget->GetActorLocation();
	FVector2D ScreenPosition;
	
	// Project the 3D position to 2D screen position
	if (ProjectWorldLocationToScreen(TargetLocation, ScreenPosition))
	{
		// Determine which body part indicator to show
		UTexture2D* TextureToUse = nullptr;
		FVector2D Offset(0.0f, 0.0f);
		
		switch (CurrentTargetedBodyPart)
		{
			case EBodyPartType::UpperBody:
				TextureToUse = UpperBodyTexture ? UpperBodyTexture : DefaultBodyPartTexture;
				Offset = FVector2D(0.0f, -50.0f);
				break;
			case EBodyPartType::LowerBody:
				TextureToUse = LowerBodyTexture ? LowerBodyTexture : DefaultBodyPartTexture;
				Offset = FVector2D(0.0f, 50.0f);
				break;
			case EBodyPartType::LeftLeg:
				TextureToUse = LeftLegTexture ? LeftLegTexture : DefaultBodyPartTexture;
				Offset = FVector2D(-30.0f, 70.0f);
				break;
			case EBodyPartType::RightLeg:
				TextureToUse = RightLegTexture ? RightLegTexture : DefaultBodyPartTexture;
				Offset = FVector2D(30.0f, 70.0f);
				break;
			default:
				break;
		}
		
		// Draw the body part indicator
		if (TextureToUse)
		{
			DrawTexture(
				TextureToUse,
				ScreenPosition.X + Offset.X - 16.0f, // Center the texture
				ScreenPosition.Y + Offset.Y - 16.0f,
				32.0f, // Width
				32.0f, // Height
				0.0f, 0.0f, 1.0f, 1.0f,
				FLinearColor(1.0f, 0.0f, 0.0f, 1.0f) // Red for better visibility
			);
		}
	}
}

void AGASCyberSoulsHUD::DrawHackProgressBar()
{
	if (!Canvas)
	{
		return;
	}
	
	// Calculate the progress percentage
	float ProgressPercent = (MaxHackProgressValue > 0.0f) ? (HackProgressValue / MaxHackProgressValue) : 0.0f;
	
	// Determine color based on progress
	FLinearColor Color;
	if (ProgressPercent < 0.5f)
	{
		Color = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f); // Red
	}
	else if (ProgressPercent < 0.75f)
	{
		Color = FLinearColor(1.0f, 1.0f, 0.0f, 1.0f); // Yellow
	}
	else
	{
		Color = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f); // Red (danger zone)
	}
	
	// Draw the progress bar at the top center of the screen
	FString Text = FString::Printf(TEXT("Hack: %d%%"), FMath::RoundToInt(ProgressPercent * 100.0f));
	DrawProgressBar(Canvas->SizeX * 0.5f - 100.0f, 50.0f, 200.0f, 20.0f, ProgressPercent, Color, Text);
}

void AGASCyberSoulsHUD::DrawIntegrityBar()
{
	if (!Canvas)
	{
		return;
	}
	
	// Calculate the integrity percentage
	float IntegrityPercent = (MaxIntegrityValue > 0.0f) ? (IntegrityValue / MaxIntegrityValue) : 0.0f;
	
	// Determine color based on integrity level
	FLinearColor Color;
	if (IntegrityPercent < 0.25f)
	{
		Color = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f); // Red
	}
	else if (IntegrityPercent < 0.5f)
	{
		Color = FLinearColor(1.0f, 0.5f, 0.0f, 1.0f); // Orange
	}
	else if (IntegrityPercent < 0.75f)
	{
		Color = FLinearColor(1.0f, 1.0f, 0.0f, 1.0f); // Yellow
	}
	else
	{
		Color = FLinearColor(0.0f, 1.0f, 0.0f, 1.0f); // Green
	}
	
	// Draw the integrity bar at the bottom left of the screen
	FString Text = FString::Printf(TEXT("Integrity: %d%%"), FMath::RoundToInt(IntegrityPercent * 100.0f));
	DrawProgressBar(50.0f, Canvas->SizeY - 50.0f, 200.0f, 20.0f, IntegrityPercent, Color, Text);
}

void AGASCyberSoulsHUD::DrawQuickHackNotification()
{
	if (!Canvas || QuickHackNotificationText.IsEmpty())
	{
		return;
	}
	
	// Draw the notification at the center of the screen
	DrawText(
		QuickHackNotificationText,
		QuickHackNotificationColor,
		Canvas->SizeX * 0.5f,
		Canvas->SizeY * 0.3f,
		GetOwningPlayerController()->GetPawnOrSpectator()->GetDefaultFontForText(),
		1.5f, // Scale
		true   // Center X
	);
}

void AGASCyberSoulsHUD::DrawProgressBar(float X, float Y, float Width, float Height, float Progress, 
                                        const FLinearColor& Color, const FString& Text)
{
	// Draw background
	DrawRect(
		FLinearColor(0.0f, 0.0f, 0.0f, 0.5f),
		X,
		Y,
		Width,
		Height
	);
	
	// Draw progress
	DrawRect(
		Color,
		X,
		Y,
		Width * Progress,
		Height
	);
	
	// Draw border
	DrawRect(
		FLinearColor(1.0f, 1.0f, 1.0f, 0.8f),
		X,
		Y,
		Width,
		1.0f
	);
	DrawRect(
		FLinearColor(1.0f, 1.0f, 1.0f, 0.8f),
		X,
		Y + Height - 1.0f,
		Width,
		1.0f
	);
	DrawRect(
		FLinearColor(1.0f, 1.0f, 1.0f, 0.8f),
		X,
		Y,
		1.0f,
		Height
	);
	DrawRect(
		FLinearColor(1.0f, 1.0f, 1.0f, 0.8f),
		X + Width - 1.0f,
		Y,
		1.0f,
		Height
	);
	
	// Draw text
	DrawText(
		Text,
		FLinearColor(1.0f, 1.0f, 1.0f, 1.0f),
		X + Width * 0.5f,
		Y + 2.0f,
		GetOwningPlayerController()->GetPawnOrSpectator()->GetDefaultFontForText(),
		1.0f, // Scale
		true   // Center X
	);
}