// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "ArcadeGameMode.h"
#include "MIR_GameplayGameMode.generated.h"

/**
 * CURRENTLY UNUSED.
 * The game mode used for this game.
 */
UCLASS(Blueprintable)
class MIRAANDTHEMIRRORSTAFF_API AMIR_GameplayGameMode : public AArcadeGameMode
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:

	void ResumeLevelIntroAfterLoading();

	bool IsLevelIntroReadyToReveal() const;

	void TryStartLevelIntro();
};
