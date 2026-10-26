// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#include "PuzzleAssets/Direct/MIR_OnOffButton.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMIR_OnOffButton::AMIR_OnOffButton()
{
	// Set this actor to call Tick() every frame.  
	// You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false; 
}

// Called when the game starts or when spawned
void AMIR_OnOffButton::BeginPlay()
{
	Super::BeginPlay();
}

void AMIR_OnOffButton::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMIR_PlayerChar* PlayerCharacter = Cast<AMIR_PlayerChar>(OtherActor);

	if (!OtherActor || !PlayerCharacter)
	{
		return;
	}

	bIsNothingFired = true;

	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::White, TEXT("Found player"));

	ActivatePuzzleAsset();
	OnDirectActivated.Broadcast(this);
}

void AMIR_OnOffButton::OnCapsuleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	AMIR_PlayerChar* PlayerCharacter = Cast<AMIR_PlayerChar>(OtherActor);

	if (!OtherActor || !PlayerCharacter)
	{
		return;
	}

	bIsNothingFired = true;

	DeactivatePuzzleAsset();
	OnDirectDeactivated.Broadcast(this);
}

// Called every frame
void AMIR_OnOffButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
