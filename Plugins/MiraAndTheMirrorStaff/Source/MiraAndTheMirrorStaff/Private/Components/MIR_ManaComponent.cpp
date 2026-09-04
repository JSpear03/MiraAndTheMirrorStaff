// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MIR_ManaComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"

// Sets default values for this component's properties
UMIR_ManaComponent::UMIR_ManaComponent()
{
	// Set this component to be initialized when the game starts and to be ticked every frame.
	// These features, if unneeded, may be turned off to improve performance.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMIR_ManaComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void UMIR_ManaComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ClearCooldownTimer();  // Clean up cooldown timer if the player is prematurely destroyed.

	Super::EndPlay(EndPlayReason);
}

// Called every frame
void UMIR_ManaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMIR_ManaComponent::ManaRecovery(int32 Amount) 
{
	ManaPower = FMath::Clamp(ManaPower + Amount, 0, TotalManaPower);
}

void UMIR_ManaComponent::ManaReduction(int32 Amount)
{
	ManaPower = FMath::Clamp(ManaPower - Amount, 0, TotalManaPower);
}

void UMIR_ManaComponent::ManaCooldown() 
{
	if (ManaPower > 0)
	{
		return;
	}

	// The amount by which mana will be recovered per timer repetition.
	const int32 ManaAmount = 1;

	// The TimerManager instance for the current world.
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	// When called on every timer repetition, a set amount of mana is recovered.
	const FTimerDelegate& CooldownDelegate = FTimerDelegate::CreateLambda([this, ManaAmount]
		{
			if (ManaPower != TotalManaPower)
			{
				// Not equal, so recover.
				UE_LOG(LogTemp, Warning, TEXT("Recover Mana"));
				ManaRecovery(ManaAmount);

				if (ManaPower == TotalManaPower)
				{
					// Are equal post-recovery, so clear timer.
					UE_LOG(LogTemp, Warning, TEXT("Clear Timer Post-Recovery"));
					ClearCooldownTimer();
					return;
				}
			}
			else 
			{
				// Are equal, so clear timer.
				UE_LOG(LogTemp, Warning, TEXT("Clear Timer"));
				ClearCooldownTimer();
				return;
			}
		}
	);

	// Repeats the delegate every second.
	TimerManager.SetTimer(CooldownHandle, CooldownDelegate, 1.0f, true);
}

void UMIR_ManaComponent::ClearCooldownTimer()
{
	if (GetWorld())  // Validate
	{
		// The TimerManager instance for the current world.
		FTimerManager& TimerManager = GetWorld()->GetTimerManager();

		if (TimerManager.IsTimerActive(CooldownHandle))  // Validate
		{
			// Perform cleanup by clearing timer and invalidating the handle.
			TimerManager.ClearTimer(CooldownHandle);
			CooldownHandle.Invalidate();
		}
	}
}

bool UMIR_ManaComponent::IsCooldownActive() const
{
	if (const UWorld* World = GetWorld())
	{
		return World->GetTimerManager().IsTimerActive(CooldownHandle);
	}
	return false;
}

void UMIR_ManaComponent::IncreaseTotalManaPower(int32 BonusManaPower)
{
	TotalManaPower = FMath::Clamp(TotalManaPower + BonusManaPower, 0, MaxManaPower);

	ManaPower = TotalManaPower;  // Puts mana at full power.
}

