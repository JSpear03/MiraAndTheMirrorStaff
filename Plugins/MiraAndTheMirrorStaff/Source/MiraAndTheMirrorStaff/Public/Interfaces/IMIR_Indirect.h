// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Characters/Player/MIR_PlayerChar.h"
#include "IMIR_Indirect.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UIMIR_Indirect : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MIRAANDTHEMIRRORSTAFF_API IIMIR_Indirect
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Sets to activate. 
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Activate")
		void ActivateIndirect(AMIR_PlayerChar* PlayerCharacter);
	
	// Sets to deactivate. 
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Deactivate")
		void DeactivateIndirect(AMIR_PlayerChar* PlayerCharacter);

};
