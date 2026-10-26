// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#include "Interactables/MIR_LoreTapestry.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/Player/MIR_PlayerChar.h"

// Sets default values.
AMIR_LoreTapestry::AMIR_LoreTapestry()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create default subobjects.
	TapestryMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TapestryMesh"));
	InteractCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractCollision"));

	// Set up attachment hierarchy.
	RootComponent = TapestryMesh;
	InteractCollision->SetupAttachment(RootComponent);

	// Set initial values.
	TapestryMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	InteractCollision->SetRelativeLocation(FVector(0.0, 0.0, 450.0));
	InteractCollision->SetBoxExtent(FVector(550.0, 550.0, 450.0));
	InteractCollision->SetMobility(EComponentMobility::Static);
}

// Called when game starts.
void AMIR_LoreTapestry::BeginPlay()
{
	Super::BeginPlay();

	InteractCollision->OnComponentBeginOverlap.AddDynamic(this, 
		&AMIR_LoreTapestry::OnInteractableBeginOverlap);
	InteractCollision->OnComponentEndOverlap.AddDynamic(this, 
		&AMIR_LoreTapestry::OnInteractableEndOverlap);
}

void AMIR_LoreTapestry::OnInteract_Implementation(AMIR_PlayerChar* CurrentPlayer)
{
	OnLoreOpened();
}

void AMIR_LoreTapestry::OnInteractableBeginOverlap(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& SweepResult)
{
	// The currently possessed player character.
	AMIR_PlayerChar* PlayerCharacter = Cast<AMIR_PlayerChar>(UGameplayStatics::
		GetPlayerCharacter(GetWorld(), 0));

	if (!OtherActor || !PlayerCharacter)  // Are NEITHER valid?
	{
		return;
	}

	bool bIsChildOfPlayer = OtherActor->GetClass()->IsChildOf<AMIR_PlayerChar>();

	// Only the player should enter the collision box.
	if (bIsChildOfPlayer)
	{
		OpenInteractUI();
	}
}

void AMIR_LoreTapestry::OnInteractableEndOverlap(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	// The currently possessed player character.
	AMIR_PlayerChar* PlayerCharacter = Cast<AMIR_PlayerChar>(UGameplayStatics::
		GetPlayerCharacter(GetWorld(), 0));

	if (!OtherActor || !PlayerCharacter)  // Are NEITHER valid?
	{
		return;
	}

	bool bIsChildOfPlayer = OtherActor->GetClass()->IsChildOf<AMIR_PlayerChar>();

	// Only the player should exit the collision box.
	if (bIsChildOfPlayer)
	{
		CloseInteractUI();
	}
}
