// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#include "Components/MIR_HealthComponent.h"
#include "Save/MIR_SaveSubsystem.h"

// Sets default values for this component's properties
UMIR_HealthComponent::UMIR_HealthComponent()
{
	// Set this component to be initialized when the game starts and to be ticked every frame.  
	// You can turn these features off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

#if WITH_EDITOR
void UMIR_HealthComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// The name of the changed property.
	const FName PropertyName = PropertyChangedEvent.Property
		? PropertyChangedEvent.Property->GetFName()
		: NAME_None;

	// Clamp `MaxHealth`, `TotalHealth`, and `Health` as needed when one has been changed.
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UMIR_HealthComponent, Health) ||
		PropertyName == GET_MEMBER_NAME_CHECKED(UMIR_HealthComponent, TotalHealth) ||
		PropertyName == GET_MEMBER_NAME_CHECKED(UMIR_HealthComponent, MaxHealth))
	{
		// `Health` must be clamped to `TotalHealth`.
		if (Health > TotalHealth) { SetHealth(Health); }
	}
}
#endif

// Called when the game starts
void UMIR_HealthComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UMIR_HealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

/*********************************************************************************************
* HEALTH STATS | HEALTH
*********************************************************************************************/

int32 UMIR_HealthComponent::GetHealth() const
{
	return Health;
}

void UMIR_HealthComponent::SetHealth(int32 NewHealth)
{
	Health = FMath::Clamp(NewHealth, 0, TotalHealth);
}

void UMIR_HealthComponent::HealthRecovery(int32 Amount)
{
	SetHealth(Health + Amount);
}

void UMIR_HealthComponent::HealthReduction(int32 Amount)
{
	SetHealth(Health - Amount);
}

/*********************************************************************************************
* HEALTH STATS | TOTAL HEALTH
*********************************************************************************************/

void UMIR_HealthComponent::SetTotalHealth(int32 NewTotalHealth)
{
	TotalHealth = FMath::Clamp(NewTotalHealth, 0, MaxHealth);
}

void UMIR_HealthComponent::IncreaseTotalHealth(int32 BonusHealth)
{
	SetTotalHealth(TotalHealth + BonusHealth);
	SetHealth(TotalHealth);  // Sets current health to full health. 

	if (UGameInstance* GameInstance = GetWorld() ? GetWorld()->GetGameInstance() : nullptr)
	{
		if (UMIR_SaveSubsystem* SaveSubsystem = GameInstance->GetSubsystem<UMIR_SaveSubsystem>())
		{
			SaveSubsystem->SetTotalHealth(TotalHealth);
		}
	}
}
