// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MIR_HealthComponent.generated.h"

/**
 * A Component which stores the amount of health that an actor has.
 * Health is expressed in multiples of two, where two units of health
 * represent one full Heart Gem in the Gameplay UI.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIRAANDTHEMIRRORSTAFF_API UMIR_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Current health. Should remain less than or equal to `TotalHealth`.
	// In the Gameplay UI, it determines how many Heart Gems are colored in.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Stats",
			  meta = (ClampMin = 0, UIMin = 0))
		int32 Health = 8;

	// The total health available. Should remain less than or equal to `MaxHealth`.
	// In the Gameplay UI, it determines how many Health Gems are displayed.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Stats",
		meta = (ClampMin = 0, UIMin = 0))
		int32 TotalHealth = 8;

	/**
	 * Maximum amount of health as a cap.
	 * For bosses, etc., this variable should be increased so that `TotalHealth`
	 * isn't clamped to an unexpectedly low value. For example, if a boss' `TotalHealth`
	 * is 24 and `MaxHealth` is 16, any damage taken (less than eight) would result in
	 * `TotalHealth` being clamped to 16.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health Stats",
			  meta = (ClampMin = 0, UIMin = 0))
		int32 MaxHealth = 16;

	// Sets default values for this component's properties.
	UMIR_HealthComponent();

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

public:	
	// Called every frame.
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, 
		FActorComponentTickFunction* ThisTickFunction) override;
	
	/**
	 * Gets the owner's `Health` variable.
	 */
	UFUNCTION(BlueprintCallable, Category = "Health Stats|Health",
			  meta = (KeyWords = "Get", ReturnDisplayName = "Health"))
		int32 GetHealth() const;

	/**
	 * Sets the owner's `Health` variable.
	 * The result will be clamped between 0 (min) and 100 (max).
	 * 
	 * @param NewHealth The value to be assigned to the `Health` variable.
	 * Subject to clamping after being passed.
	 */
	UFUNCTION(BlueprintCallable, Category = "Health Stats|Health")
		void SetHealth(int32 NewHealth);

	/**
	 * Increases `Health`, clamping between 0 and `TotalHealth`.
	 * 
	 * @param Amount The amount by which `Health` will recover.
	 */
	UFUNCTION(BlueprintCallable, Category = "Health Stats|Health")
		void HealthRecovery(int32 Amount);

	/**
	 * Decreases `Health`, clamping between 0 and `TotalHealth`.
	 *
	 * @param Amount The amount by which `Health` will be reduced.
	 */
	UFUNCTION(BlueprintCallable, Category = "Health Stats|Health")
		void HealthReduction(int32 Amount);

	/**
	 * Manually sets `TotalHealth`, clamping between 0 and `MaxHealth`.
	 * To increase this variable via items, etc., see `IncreaseTotalHealth()`.
	 *
	 * @param NewTotalMana The new value for `TotalHealth`.
	 */
	UFUNCTION(BlueprintCallable, Category = "Health Stats|Total Health")
		void SetTotalHealth(int32 NewTotalHealth);

	/**
	 * Increases the amount of hearts in viewport.
	 * 
	 * @param BonusHealth The amount recieved from the HealthUp item.
	 */
	UFUNCTION(BlueprintCallable, Category = "Health Stats|Total Health")
		void IncreaseTotalHealth(int32 BonusHealth);
};
