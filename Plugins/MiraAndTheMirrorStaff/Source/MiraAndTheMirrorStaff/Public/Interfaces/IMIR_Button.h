// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Characters/Player/MIR_PlayerChar.h"
#include "IMIR_Button.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UIMIR_Button : public UInterface
{
	GENERATED_BODY()
};

/**
 * UNUSED?
 * Interface for Buttons.
 */
class MIRAANDTHEMIRRORSTAFF_API IIMIR_Button
{
	GENERATED_BODY()

	// Add interface functions to this class.
	// This is the class that will be inherited to implement this interface.
public:
	// May or may not delete the Player Character parameters later,
	// depending on what we do in the functions.

	/**
	 * Sets button to ON state.
	 * 
	 * @param PlayerCharacter The player that stepped on the button.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "On")
		void TurnOnButton(AMIR_PlayerChar* PlayerCharacter);

	/**
	 * Sets button to OFF state.
	 * 
	 * @param PlayerCharacter The player that stepped off the button.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Off")
		void TurnOffButton(AMIR_PlayerChar* PlayerCharacter);

};
