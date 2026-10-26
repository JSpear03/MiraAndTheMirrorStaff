// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "Hazards/MIR_HazardBASE.h"
#include "MIR_HazardLava.generated.h"

/**
 * A body of lava. Damages and respawns the player and deletes enemies.
 * No attacks can interact with it, so it adds nothing to the base behavior;
 * it exists as a distinct type for level design and future divergence.
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_HazardLava : public AMIR_HazardBASE
{
	GENERATED_BODY()
};
