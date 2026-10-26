// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#include "GameModes/MIR_GameplayGameMode.h"

void AMIR_GameplayGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AMIR_GameplayGameMode::ResumeLevelIntroAfterLoading()
{
    return;
    //UWorld* World = GetWorld();
    //UDUA_GameSubsystem* Sub = GetGameSubsystem();
    //if (!bLevelIntroActive || !bLevelIntroWaitingForReveal || !IsValid(LevelIntroPlayer) ||
    //    bDirectorEndingPlay || !World || World->bIsTearingDown ||
    //    (Sub && (Sub->GetActiveDirector() != this || !Sub->CanAdvanceLevelIntroPlayback()))) return;

    //RefreshLevelIntroView();
    //LevelIntroPlayer->Play();
    //RefreshLevelIntroView();
    //if (LevelIntroPlayer->IsPlaying())
    //{
    //    bLevelIntroWaitingForReveal = false;
    //    if (Sub) Sub->MarkLevelIntroPlayed(GetLevel());
    //    const APlayerController* PC = World->GetFirstPlayerController();
    //    UE_LOG(LogTemp, Log, TEXT("DUA: Intro '%s' resumed after loading at %.2fs; camera '%s'."),
    //        *GetNameSafe(LevelStartSequence), LevelIntroPlayer->GetCurrentTime().AsSeconds(),
    //        *GetNameSafe(PC ? PC->GetViewTarget() : nullptr));
    //}
}

bool AMIR_GameplayGameMode::IsLevelIntroReadyToReveal() const
{
	return false;
    //if (!bLevelIntroActive  !IsValid(LevelIntroPlayer)
    //    (!LevelIntroPlayer->IsPlaying() && !LevelIntroPlayer->IsPaused())) return false;
    //const APlayerController* PC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;
    //const AActor* IntroView = GetLevelIntroViewTarget();
    //return PC && IntroView && PC->GetViewTarget() == IntroView
    //    && (!PC->PlayerCameraManager || !PC->PlayerCameraManager->PendingViewTarget.Target);
}

void AMIR_GameplayGameMode::TryStartLevelIntro()
{
    return;
}
