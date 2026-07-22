// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MIR_HealthComponent.h"
#include "MIR_ElementComponent.h"
#include "MIR_CharacterBASE_M.generated.h"

UCLASS(Blueprintable)
class MIRAANDTHEMIRRORSTAFF_API AMIR_CharacterBASE_M : public ACharacter
{
	GENERATED_BODY()

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

	// Decreases health based on the calculated damage. 
	UFUNCTION(BlueprintCallable, Category = "Damage")
		void CalculateDamage(FString AttackerElement, FString RecieverElement, 
			int32 AttackerDamageValue);
};
