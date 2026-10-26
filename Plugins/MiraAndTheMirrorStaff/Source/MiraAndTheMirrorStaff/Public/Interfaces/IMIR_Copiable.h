// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Magic/MIR_MagicBASE.h"
#include "IMIR_Copiable.generated.h"

class AMIR_PlayerChar;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UIMIR_Copiable : public UInterface
{
	GENERATED_BODY()
};

/**
 *  Interface for the Copiable Component to check if player can copy magic. 
 */
class MIRAANDTHEMIRRORSTAFF_API IIMIR_Copiable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// When called, the function stores copied magic for the player.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Copy Magic")
		void CopyMagic(AMIR_PlayerChar* CurrentPlayer, TSubclassOf<AMIR_MagicBASE> MagicToCopy);
};
