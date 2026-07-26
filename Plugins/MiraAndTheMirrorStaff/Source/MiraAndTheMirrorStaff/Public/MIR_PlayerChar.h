// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MIR_CharacterBASE_M.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "MIR_ManaComponent.h"
#include "MIR_PlayerChar.generated.h"

UENUM(BlueprintType)
enum class EMIR_KeyItem : uint8
{
	MirrorStaff,
	SkyboundShoes,
	SpiritStole,
	KeyGemstone
};

/**
 * The character that the player will control in this game.
 * Implements gameplay-context inputs, etc.
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_PlayerChar : public AMIR_CharacterBASE_M
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Copied Magic")
		FString StoredMagicType;

	UPROPERTY()
		bool bCanGetHit = true;

public:
	// Number of Key Gemstones collected. All four unlock the grand door in Inner Sanctum.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key Items")
		int32 KeyGemstoneCount = 0;

	// Used as part of a timer to set the value of bCanGetHit. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Invincible Cooldown")
		int32 CooldownSeconds = 2;

	// Used for the position of the camera. 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
		USpringArmComponent* SpringArm;

	// Displays the gameplay. 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
		UCameraComponent* CameraComponent;

	// Represents the game's Mana system.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mana")
		UMIR_ManaComponent* ManaComponent;

	// Checks if player can jump with Skybound Shoes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
		bool bCanJump = false;

	// Checks if player can dash with Spirit Stole. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
		bool bCanDash = false;

	// Sets default values for Player Character's properties.
	AMIR_PlayerChar();

protected:
	// Sets default values
	virtual void BeginPlay() override;

	// Called when player contacts an object. 
	UFUNCTION()
		void HitEvent(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, 
			const FHitResult& Hit);

private:
	// Stars the timer for the hit event cooldown.
	void StartCooldownTimer();

	// Sets bCanGetHit to true or false.
	void SetCanGetHit(bool bGetHit);

public:
	// Stored Magic setter.
	UFUNCTION(BlueprintCallable)
		void SetStoredMagic(FString MagicType);

	// Stored Magic getter.
	UFUNCTION(BlueprintCallable)
		FString GetStoredMagic();

	// Grants item from treasure.
	UFUNCTION(BlueprintCallable)
		void GrantKeyItem(EMIR_KeyItem KeyItem);
};
