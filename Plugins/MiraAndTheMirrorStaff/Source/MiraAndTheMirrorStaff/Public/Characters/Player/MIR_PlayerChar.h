// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/MIR_CharacterBASE_M.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/MIR_ManaComponent.h"
#include "Magic/MIR_MagicBASE.h"
#include "MIR_PlayerChar.generated.h"

class AMIR_RespawnPoint;

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

	// Represents whether the player is invincible (i.e., they cannot get hit).
	UPROPERTY()
		bool bCanGetHit = true;

	// The most recently activated respawn point. Damaging hazards teleport the player here.
	UPROPERTY()
		AMIR_RespawnPoint* LastRespawnPoint = nullptr;

private:
	// A class reference to the Dagger.
	UPROPERTY(EditAnywhere, Category = "Magic|Sub-Attack (Dagger)")
		TSubclassOf<class AMIR_MagicMelee> DaggerRef;

	// A class reference to the Melee Copy.
	UPROPERTY(EditAnywhere, Category = "Magic|Copy Attacks")
		TSubclassOf<class AMIR_MagicMelee> MeleeCopyRef;

	// A class reference to the Ranged Copy.
	UPROPERTY(EditAnywhere, Category = "Magic|Copy Attacks")
		TSubclassOf<class AMIR_MagicRanged> RangedCopyRef;

public:
	// Key item flags. Set to true when obtained so any logic gates
	// involving their inputs (Jump, Dash, etc.) can succeed.

	// Represents whether the player has the Mirror Staff,
	// which is needed for input events related to copying, etc.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key Items")
		bool bHasMirrorStaff = false;

	// Represents whether the player has the Skybound Shoes (and, thus, the Jump ability).
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key Items")
		bool bHasSkyboundShoes = false;

	// Represents whether the player has the Spirit Stole (and, thus, the Dash ability).
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key Items")
		bool bHasSpiritStole = false;

	// Number of Key Gemstones collected. All four unlock the grand door in Inner Sanctum.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key Items",
			  meta = (ClampMin = 0, ClampMax = 4, UIMin = 0, UIMax = 4))
		int32 KeyGemstoneCount = 0;

	// Used as part of a timer to set the value of bCanGetHit. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Invincible Cooldown",
			  meta = (Units = "Seconds", ClampMin = 0.0, ClampMax = 5.0, UIMin = 0.0, UIMax = 5.0))
		float CooldownSeconds = 2.0;

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

	// Spawn transform for Melee magic.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arrows")
		UArrowComponent* MeleeArrow;

	// Spawn transform for Ranged magic.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arrows")
		UArrowComponent* RangedArrow;

	// Spawn transform for AoE magic.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arrows")
		UArrowComponent* AOEArrow;

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
	// DaggerRef getter.
	UFUNCTION(BlueprintCallable, Category = "Magic|Sub-Attack (Dagger)")
		TSubclassOf<class AMIR_MagicMelee> GetDaggerRef() const { return DaggerRef; };

	// MeleeCopyRef getter.
	UFUNCTION(BlueprintCallable, Category = "Magic|Copy Attacks")
		TSubclassOf<class AMIR_MagicMelee> GetMeleeCopyRef() const { return MeleeCopyRef; };

	// RangedCopyRef getter.
	UFUNCTION(BlueprintCallable, Category = "Magic|Copy Attacks")
		TSubclassOf<class AMIR_MagicRanged> GetRangedCopyRef() const { return RangedCopyRef; };

	// Grants item from treasure.
	UFUNCTION(BlueprintCallable)
		void GrantKeyItem(EMIR_KeyItem KeyItem);

	// Reduces health from hazard contact, respecting the invincibility cooldown.
	UFUNCTION(BlueprintCallable, Category = "Hazard")
		void ApplyHazardDamage(int32 Damage);

	// Last Respawn Point setter. Called by a respawn point's trigger when overlapped.
	UFUNCTION(BlueprintCallable, Category = "Hazard")
		void SetLastRespawnPoint(AMIR_RespawnPoint* RespawnPoint);

	// Teleports the player to their last activated respawn point.
	UFUNCTION(BlueprintCallable, Category = "Hazard")
		void RespawnAtLastPoint();
};
