// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MIR_MiniDragon_AIController.generated.h"

/**
 * AI Controller for the Mini-Dragon enemy type.
 *
 * Per the GDD, the Mini-Dragon is a small, rotund-looking dragon that spits
 * projectiles from its mouth. Its behaviors are:
 *   - Tries to align itself with the player on either the x- or y-axis
 *     (whichever is more distant from the player).
 *   - Once aligned, it shoots a Ranged projectile.
 *   - Also does contact damage (half a heart).
 *
 * This C++ controller mirrors the pattern established by MIR_Skeleton_AIController.
 * The Blueprint AI Controller (AIC_MIR_MiniDragon) should be reparented to this
 * class so that any future C++ logic (perception, custom pathfinding overrides,
 * etc.) is automatically inherited.
 */
UCLASS()
class MIRAANDTHEMIRRORSTAFF_API AMIR_MiniDragon_AIController : public AAIController
{
	GENERATED_BODY()

};
