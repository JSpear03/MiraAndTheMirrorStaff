// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#include "Components/MIR_ManaComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"
#include "Save/MIR_SaveSubsystem.h"

// Sets default values for this component's properties
UMIR_ManaComponent::UMIR_ManaComponent()
{
	// Set this component to be initialized when the game starts and to be ticked every frame.
	// These features, if unneeded, may be turned off to improve performance.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

#if WITH_EDITOR
void UMIR_ManaComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// The name of the changed property.
	const FName PropertyName = PropertyChangedEvent.Property
		? PropertyChangedEvent.Property->GetFName()
		: NAME_None;
	
	// Clamp `TotalManaPower` and `ManaPower` as needed when either has been changed.
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UMIR_ManaComponent, ManaPower) ||
		PropertyName == GET_MEMBER_NAME_CHECKED(UMIR_ManaComponent, TotalManaPower) ||
		PropertyName == GET_MEMBER_NAME_CHECKED(UMIR_ManaComponent, MaxManaPower))
	{
		// `TotalManaPower` must be clamped to `MaxManaPower`.
		if (TotalManaPower > MaxManaPower) { SetTotalManaPower(TotalManaPower); }

		// `ManaPower` must be clamped to `TotalManaPower`.
		if (ManaPower > TotalManaPower) { SetManaPower(ManaPower); }
	}
}
#endif

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
void UMIR_ManaComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

/*********************************************************************************************
* MANA | MANA POWER
*********************************************************************************************/

void UMIR_ManaComponent::SetManaPower(int32 NewManaPower)
{
	ManaPower = FMath::Clamp(NewManaPower, 0, TotalManaPower);
}

void UMIR_ManaComponent::ManaRecovery(int32 Amount) 
{
	SetManaPower(ManaPower + Amount);
}

void UMIR_ManaComponent::ManaReduction(int32 Amount)
{
	SetManaPower(ManaPower - Amount);
}

/*********************************************************************************************
* MANA | TOTAL MANA POWER
*********************************************************************************************/

void UMIR_ManaComponent::SetTotalManaPower(int32 NewTotalMana)
{
	TotalManaPower = FMath::Clamp(NewTotalMana, 0, MaxManaPower);
}

void UMIR_ManaComponent::IncreaseTotalManaPower(int32 BonusManaPower)
{
	SetTotalManaPower(TotalManaPower + BonusManaPower);
	SetManaPower(TotalManaPower);  // Puts mana at full power.

	if (UGameInstance* GameInstance = GetWorld() ? GetWorld()->GetGameInstance() : nullptr)
	{
		if (UMIR_SaveSubsystem* SaveSubsystem = GameInstance->GetSubsystem<UMIR_SaveSubsystem>())
		{
			SaveSubsystem->SetTotalMana(TotalManaPower);
		}
	}
}

/*********************************************************************************************
* MANA | COOLDOWN
*********************************************************************************************/

void UMIR_ManaComponent::ManaCooldown() 
{
	// Cooldown should not execute if ManaPower is not zero.
	if (ManaPower > 0)
	{
		return;
	}

	// Guard against uninitialized or zero total mana (prevents division by zero)
	if (TotalManaPower <= 0)
	{
		return;
	}

	// Do not restart cooldown if it is already actively ticking
	if (IsCooldownActive())
	{
		return;
	}

	// The amount by which mana will be recovered per timer repetition.
	const int32 ManaAmount = 1;

	// The recovery rate of the Mana Gauge, which enables the gauge to recover at variable
	// rates so that the total recovery time is always exactly eight seconds.
	const float RecoveryRate = 8.0f / static_cast<float>(TotalManaPower);

	if (UWorld* World = GetWorld())
	{
		FTimerManager& TimerManager = World->GetTimerManager();

		// When called on every timer repetition, a set amount of mana is recovered.
		const FTimerDelegate CooldownDelegate = FTimerDelegate::CreateWeakLambda(this, [this, ManaAmount]()
		{
			if (ManaPower < TotalManaPower)
			{
				ManaRecovery(ManaAmount);

				if (ManaPower >= TotalManaPower)
				{
					ClearCooldownTimer();
				}
			}
			else 
			{
				ClearCooldownTimer();
			}
		});

		// Repeats the delegate per the value stored in RecoveryRate.
		TimerManager.SetTimer(CooldownHandle, CooldownDelegate, RecoveryRate, true);
	}
}

void UMIR_ManaComponent::ClearCooldownTimer()
{
	if (GetWorld())
	{
		// The TimerManager instance for the current world.
		FTimerManager& TimerManager = GetWorld()->GetTimerManager();

		if (TimerManager.IsTimerActive(CooldownHandle))
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
