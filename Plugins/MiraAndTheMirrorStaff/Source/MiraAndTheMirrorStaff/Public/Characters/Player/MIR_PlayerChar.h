// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/MIR_CharacterBASE_M.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/MIR_ManaComponent.h"
#include "Magic/MIR_MagicBASE.h"
#include "Enums/EMIR_KeyItem.h"
#include "Enums/EMIR_AttackTypes.h"
#include "MIR_PlayerChar.generated.h"

class AMIR_RespawnPoint;
class UAnimSequenceBase;
class AMIR_CameraMovePoints;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRespawn);

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
	// Attack animation references configured per attack type.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Animations")
		TObjectPtr<class UAnimSequenceBase> MeleeAttackAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Animations")
		TObjectPtr<class UAnimSequenceBase> RangedAttackAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Animations")
		TObjectPtr<class UAnimSequenceBase> AOEAttackAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Animations")
		TObjectPtr<class UAnimSequenceBase> DaggerAttackAnim;

	// Helper to get the animation configured for a given attack type
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat|Animations")
		class UAnimSequenceBase* GetAttackAnimation(EAttackTypes AttackType) const;

	// Helper to get the dagger / sub-attack animation
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat|Animations")
		class UAnimSequenceBase* GetSubAttackAnimation() const;

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
	// TODO: Deprecate; the Bools are a better solution.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key Items",
			  meta = (ClampMin = 0, ClampMax = 4, UIMin = 0, UIMax = 4))
		int32 KeyGemstoneCount = 0;

	// Checks if gem is colored red.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key Items")
		bool bIsGemRed = false;

	// Checks if gem is colored blue.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key Items")
		bool bIsGemBlue = false;

	// Checks if gem is colored green.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key Items")
		bool bIsGemGreen = false;

	// Checks if gem is colored yellow.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key Items")
		bool bIsGemYellow = false;

	// Used as part of a timer to set the value of bCanGetHit. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Invincible Cooldown",
			  meta = (Units = "Seconds", ClampMin = 0.0f, ClampMax = 5.0f, UIMin = 0.0f, UIMax = 5.0f))
		float CooldownSeconds = 2.0f;

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

	// The CMP most recently overlapped with.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		TObjectPtr<AMIR_CameraMovePoints> CurrentCMP = nullptr;

	// Sets default values for Player Character's properties.
	AMIR_PlayerChar();

protected:
	// Sets default values
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when player contacts an object. 
	UFUNCTION()
		void HitEvent(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, 
			const FHitResult& Hit);

	// Called when player overlaps with incoming magic / projectiles.
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);

private:
	// Stars the timer for the hit event cooldown.
	void StartCooldownTimer();

	// Sets bCanGetHit to true or false.
	void SetCanGetHit(bool bGetHit);

public:
	// Loads the Game-Over Screen if the player's health is 0.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Game Over")
		void LoadingGameOver();

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
	
	// Sets new element based on the element name. 
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Elements")
		void CopyElement();

	// Delegate object to move camera to a move point nearest to the respawn point.
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Respawn")
		FOnRespawn OnRespawn;
};
