// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MIR_ManaComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UMIR_ManaComponent::UMIR_ManaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMIR_ManaComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
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

	FTimerHandle CooldownTimerHandle;
	int32 ManaAmount = 1;

	// When called after timer reached a certain amount of seconds, a little bit of mana is recovered.. 
	const FTimerDelegate& CooldownDelegate = FTimerDelegate::CreateLambda([this, ManaAmount, &CooldownTimerHandle] 
		{
			if (ManaPower != TotalManaPower)
			{
				UE_LOG(LogTemp, Warning, TEXT("Recover Mana"));

				ManaRecovery(ManaAmount);
			}
			else 
			{
				UE_LOG(LogTemp, Warning, TEXT("Clear Timer"));

				ClearCooldownTimer(CooldownTimerHandle);
			}
		}
	);

	const UWorld* World = GetWorld();
	FTimerManager& TimerManager = World->GetTimerManager();

	// Repeats the delegate every second.
	TimerManager.SetTimer(CooldownTimerHandle, CooldownDelegate, 1, true);
}

void UMIR_ManaComponent::ClearCooldownTimer(FTimerHandle CurrentTimerHandle)
{
	const UWorld* World = GetWorld();
	FTimerManager& TimerManager = World->GetTimerManager();

	TimerManager.ClearTimer(CurrentTimerHandle);
}

void UMIR_ManaComponent::IncreaseTotalManaPower(int32 BonusManaPower)
{
	TotalManaPower = FMath::Clamp(TotalManaPower + BonusManaPower, 0, MaxManaPower);

	ManaPower = TotalManaPower;  // Puts mana at full power.
}

