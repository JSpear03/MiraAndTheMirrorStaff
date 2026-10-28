// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MIR_ManaComponent.generated.h"

/**
 * A Component which stores the amount of Mana Power that an actor has.
 * Mana Power is expressed in multiples of two, where two units of Mana Power
 * represent one full Mana Gem in the Gameplay UI.
 */
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIRAANDTHEMIRRORSTAFF_API UMIR_ManaComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	// Timer handle for the cooldown timer.
	FTimerHandle CooldownHandle;

public:	
	// Current Mana Power. Should remain less than or equal to `TotalManaPower`.
	// In the Gameplay UI, it determines how many Mana Gems are colored in.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana",
			  meta = (ClampMin = 0, ClampMax = 16, UIMin = 0, UIMax = 16))
		int32 ManaPower = 8;

	// The total Mana Power available. Should remain less than or equal to `MaxManaPower`.
	// In the Gameplay UI, it determines how many Mana Gems are displayed.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana",
			  meta = (ClampMin = 0, ClampMax = 16, UIMin = 0, UIMax = 16))
		int32 TotalManaPower = 8;

	// Maximum amount of Mana Power as a cap.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mana",
			  meta = (ClampMin = 0, UIMin = 0))
		int32 MaxManaPower = 16;

	// Sets default values for this component's properties.
	UMIR_ManaComponent();

#if WITH_EDITOR
	/**
	 * Called when a property is changed in the editor.
	 * 
	 * @param PropertyChangedEvent Contains information about the changed property.
	 */
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	// Called when the game starts.
	virtual void BeginPlay() override;

	// Called when the component ends.
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame.
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, 
		FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * Manually sets `ManaPower`, clamping between 0 and `TotalManaPower`.
	 * To increase or decrease this variable via items, etc., see `ManaRecovery()`
	 * and `ManaReduction()`.
	 *
	 * @param NewManaPower The new value for `ManaPower`.
	 */
	UFUNCTION(BlueprintCallable, Category = "Mana|Mana Power")
		void SetManaPower(int32 NewManaPower);

	/**
	 * Increases `ManaPower`, clamping between 0 and `TotalManaPower`.
	 * 
	 * @param Amount The amount by which `ManaPower` will increase.
	 */
	UFUNCTION(BlueprintCallable, Category = "Mana|Mana Power")
		void ManaRecovery(int32 Amount);

	/**
	 * Decreases `ManaPower`, clamping between 0 and `TotalManaPower`.
	 *
	 * @param Amount The amount by which `ManaPower` will decrease.
	 */
	UFUNCTION(BlueprintCallable, Category = "Mana|Mana Power")
		void ManaReduction(int32 Amount);

	/**
	 * Manually sets `TotalManaPower`, clamping between 0 and `MaxManaPower`.
	 * To increase this variable via items, etc., see `IncreaseTotalManaPower()`.
	 *
	 * @param NewTotalMana The new value for `TotalManaPower`.
	 */
	UFUNCTION(BlueprintCallable, Category = "Mana|Total Mana Power")
		void SetTotalManaPower(int32 NewTotalMana);

	/**
	 * Increases `TotalManaPower`, clamping between 0 and `MaxManaPower`.
	 * Then, it sets `ManaPower` to `TotalManaPower`.
	 *
	 * @param BonusManaPower The amount received from the ManaUp item.
	 */
	UFUNCTION(BlueprintCallable, Category = "Mana|Total Mana Power")
		void IncreaseTotalManaPower(int32 BonusManaPower);

	// Sets up timer for cooldown.
	// TODO: Call in `ManaReduction()` instead.
	UFUNCTION(BlueprintCallable, Category = "Mana|Cooldown")
		void ManaCooldown();

	/**
	 * Determines whether the mana-cooldown timer is actively ticking (mana is recovering).
	 * 
	 * @returns Whether said timer is active.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Mana|Cooldown",
			  meta = (ReturnDisplayName = "Active?"))
		bool IsCooldownActive() const;

	// Clears the cooldown timer. Must be called during actor cleanup on `EndPlay()`.
	void ClearCooldownTimer();
};
