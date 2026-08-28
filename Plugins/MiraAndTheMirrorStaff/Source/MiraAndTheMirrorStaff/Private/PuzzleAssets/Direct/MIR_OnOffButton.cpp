// Fill out your copyright notice in the Description page of Project Settings.


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

	CapsuleCollision->OnComponentBeginOverlap.AddDynamic(this, &AMIR_OnOffButton::OnOverlap);
	CapsuleCollision->OnComponentEndOverlap.AddDynamic(this, &AMIR_OnOffButton::OnEndOverlap);
}

void AMIR_OnOffButton::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* CurrentCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AMIR_PlayerChar* PlayerCharacter = Cast<AMIR_PlayerChar>(CurrentCharacter);

	if (!OtherActor || !PlayerCharacter)
	{
		return;
	}

	bool bIsChildOfPlayer = OtherActor->GetClass()->IsChildOf<AMIR_PlayerChar>();

	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::White, TEXT("Actor Overlap"));

	if (!bIsChildOfPlayer)
	{
		return;
	}

	bIsNothingFired = true;

	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::White, TEXT("Found player"));

	ActivatePuzzleAsset();
	OnDirectActivated.Broadcast(this);
}

void AMIR_OnOffButton::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	ACharacter* CurrentCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AMIR_PlayerChar* PlayerCharacter = Cast<AMIR_PlayerChar>(CurrentCharacter);

	if (!OtherActor || !PlayerCharacter || bIsNothingFired)
	{
		return;
	}

	bool bIsChildOfPlayer = OtherActor->GetClass()->IsChildOf<AMIR_PlayerChar>();

	if (!bIsChildOfPlayer)
	{
		return;
	}

	DeactivatePuzzleAsset();
	OnDirectDeactivated.Broadcast(this);
}

// Called every frame
void AMIR_OnOffButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
