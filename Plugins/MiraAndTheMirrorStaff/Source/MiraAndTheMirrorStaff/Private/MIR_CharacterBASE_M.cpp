// Fill out your copyright notice in the Description page of Project Settings.


#include "MIR_CharacterBASE_M.h"
#include "MIR_HealthComponent.h"

// Sets default values
AMIR_CharacterBASE_M::AMIR_CharacterBASE_M()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Creates Health Component
	HealthComponent = CreateDefaultSubobject<UMIR_HealthComponent>(TEXT("HealthComponent"));


}

// Called when the game starts or when spawned
void AMIR_CharacterBASE_M::BeginPlay()
{
	Super::BeginPlay();

	/*if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Default: %f"), HealthComponent->GetHealth()));
		HealthComponent->SetHealth(-1);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, FString::Printf(TEXT("After: %f"), HealthComponent->GetHealth()));
	}*/
	
}

// Called every frame
void AMIR_CharacterBASE_M::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMIR_CharacterBASE_M::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

