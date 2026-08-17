// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/MIR_HealthComponent.h"
#include "Components/MIR_ElementComponent.h"
#include "Magic/MIR_MagicBASE.h"
#include "MIR_CharacterBASE_M.generated.h"

UCLASS(Blueprintable)
class MIRAANDTHEMIRRORSTAFF_API AMIR_CharacterBASE_M : public ACharacter
{
	GENERATED_BODY()

	// The currently usable magic attack.
	UPROPERTY(EditAnywhere, Category = "Magic|Main Attack")
		TSubclassOf<class AMIR_MagicBASE> StoredMagicType;

public:
	// Sets default values for this character's properties
	AMIR_CharacterBASE_M();

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

	// TODO: Update with new Element and AttackType Component changes.
	// Decreases health based on the calculated damage. 
	UFUNCTION(BlueprintCallable, Category = "Damage")
		void CalculateDamage(FString AttackerElement, FString RecieverElement, 
			int32 AttackerDamageValue);

	// Stored Magic setter.
	UFUNCTION(BlueprintCallable, Category = "Magic|Main Attack")
		void SetStoredMagic(TSubclassOf<class AMIR_MagicBASE> MagicType);

	// Stored Magic getter.
	UFUNCTION(BlueprintCallable, Category = "Magic|Main Attack")
		TSubclassOf<class AMIR_MagicBASE> GetStoredMagic() const;
};
