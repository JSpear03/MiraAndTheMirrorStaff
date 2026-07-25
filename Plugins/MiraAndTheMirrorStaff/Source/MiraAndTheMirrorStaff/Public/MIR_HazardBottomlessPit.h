// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MIR_HazardBASE.h"
#include "MIR_HazardBottomlessPit.generated.h"

/**
 * A bottomless pit where no terrain exists. Damages and respawns the player
 * and deletes enemies. Place the collision box below floor level so actors
 * only overlap it after genuinely falling in.
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_HazardBottomlessPit : public AMIR_HazardBASE
{
	GENERATED_BODY()
};
