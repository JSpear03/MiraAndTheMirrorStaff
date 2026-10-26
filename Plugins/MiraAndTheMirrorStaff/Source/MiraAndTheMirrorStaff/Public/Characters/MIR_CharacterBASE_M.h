// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/MIR_HealthComponent.h"
#include "Components/MIR_ElementComponent.h"
#include "Magic/MIR_MagicBASE.h"
#include "Enums/EMIR_Elements.h"
#include "MIR_CharacterBASE_M.generated.h"

UCLASS(Blueprintable)
class MIRAANDTHEMIRRORSTAFF_API AMIR_CharacterBASE_M : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMIR_CharacterBASE_M();

	// The currently usable magic attack.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magic|Main Attack")
		TSubclassOf<class AMIR_MagicBASE> StoredMagicType;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// The character's Health Component, which handles health calculations.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
		UMIR_HealthComponent* HealthComponent;

	// The Base Character's Element component, which represents the element system. 
	// Useful for calculcating the character's elemental weakenesses and strengths. 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Element")
		UMIR_ElementComponent* ElementComponent; 

	// Decreases health based on the calculated damage. 
	UFUNCTION(BlueprintCallable, Category = "Damage")
		void CalculateDamage(EElements AttackerElement, EElements RecieverElement,
			int32 AttackerDamageValue);

	// Stored Magic setter.
	UFUNCTION(BlueprintCallable, Category = "Magic|Main Attack")
		virtual void SetStoredMagic(TSubclassOf<class AMIR_MagicBASE> MagicType);

	// Stored Magic getter.
	UFUNCTION(BlueprintCallable, Category = "Magic|Main Attack")
		TSubclassOf<class AMIR_MagicBASE> GetStoredMagic() const;
};

